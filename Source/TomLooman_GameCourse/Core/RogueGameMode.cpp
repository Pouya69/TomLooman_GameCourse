// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueGameMode.h"

#include "EngineUtils.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "AI/RogueAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Player/RoguePlayerController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RoguePlayerCharacter.h"
#include "Player/RoguePlayerState.h"
#include "PowerUps/RoguePowerupBase.h"

static TAutoConsoleVariable<bool> CVar_ShouldBotsSpawn(TEXT("game.ai.ShouldBotsSpawn"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


ARogueGameMode::ARogueGameMode()
{
	PlayerControllerClass = ARoguePlayerController::StaticClass();
	PlayerStateClass = ARoguePlayerState::StaticClass();
	
	SpawnMinionsEverySeconds = 2.0f;
	NumMinionsInLevel = 0;
	
	NumPowerUpsToSpawnOnStart = 7;
	
	PlayerRespawnTimerInSeconds = 2.0f;
}

void ARogueGameMode::StartPlay()
{
	Super::StartPlay();
	
	GetWorldTimerManager().SetTimer(MinionSpawnTimerHandle, this, &ARogueGameMode::SpawnMinionTimerElapsed, SpawnMinionsEverySeconds, true);
	
	if (NumMinionsInLevel == 0)
	{
		// Count the minions in level.
		for (TActorIterator<ARogueAICharacter> It(GetWorld()); It; ++It)
		{
			const URogueActionSystemComponent* AIActionSystemComp = URogueActionSystemComponent::GetActionSystemComponent(*It);
			
			if (AIActionSystemComp->IsAlive())
				NumMinionsInLevel++;
		}
	}
	
	// To make sure this is always counted correctly.
	ensureMsgf(NumMinionsInLevel <= GetMaxMinionsBasedOnDifficulty(0.f), 
		TEXT("Minions Currently in level are more than difficulty curve. fix: Change the UCurveFloat assigned."));
	
	SpawnPowerUpsRandom();
}


void ARogueGameMode::KillAllAI()
{
	for (TActorIterator<ARogueAICharacter> It(GetWorld()); It; ++It)
	{
		URogueActionSystemComponent* AIActionSystemComp = URogueActionSystemComponent::GetActionSystemComponent(*It);
			
		if (AIActionSystemComp->IsAlive())
			AIActionSystemComp->Kill(this);  // @FIXME: Or player killed. For kill credit.
	}
}

void ARogueGameMode::KillPlayer(const uint8 PlayerIndex)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, PlayerIndex);
	
	if (Player)
	{
		URogueActionSystemComponent* ActionSystemComponent = URogueActionSystemComponent::GetActionSystemComponent(Player);
		if (ActionSystemComponent->IsAlive())
			ActionSystemComponent->Kill(this);
	}
}

void ARogueGameMode::OnActorKilled_Implementation(AActor* VictimActor, AActor* KillerActor)
{
	if (ARogueAICharacter* AICharacterDied = Cast<ARogueAICharacter>(VictimActor))
	{
		EnemyDied(AICharacterDied, KillerActor);
	}
	else if (ARoguePlayerCharacter* PlayerCharacter = Cast<ARoguePlayerCharacter>(VictimActor))
	{
		// We make this a local variable due to multiple players.
		FTimerHandle TimerHandleRespawnDelay;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "RespawnPlayerTimerElapsed", PlayerCharacter->GetController());
		
		GetWorldTimerManager().SetTimer(TimerHandleRespawnDelay, TimerDelegate, PlayerRespawnTimerInSeconds, false);
	}
}

void ARogueGameMode::RespawnPlayerTimerElapsed(AController* PlayerController)
{
	if (ensure(PlayerController))
	{
		// Look at the RestartPlayer(). If we don't unposses, it will only teleport
		// It will not restart the player stats etc. without it.
		APawn* PawnUnpossessing = PlayerController->GetPawn();
		PlayerController->UnPossess();
		PawnUnpossessing->Destroy();
		
		RestartPlayer(PlayerController);
	}
}

void ARogueGameMode::SpawnPowerUpsRandom()
{
	ensure(!PowerUpsToSpawn.IsEmpty());
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(GetWorld(), PowerUPSpawn_EQS, this, EEnvQueryRunMode::AllMatching, nullptr);
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARogueGameMode::OnSpawnPowerUpsQueryCompleted);
}

void ARogueGameMode::OnSpawnPowerUpsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Requested EQS was not successful!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Requested EQS gave no locations!"));
		return;
	}
	if (!Locations.IsValidIndex(NumPowerUpsToSpawnOnStart))
	{
		UE_LOG(LogTemp, Warning, TEXT("Requested EQS results. EQS: %d, requested: %d"), Locations.Num(), NumPowerUpsToSpawnOnStart);
		return;
	}
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = 0; i < NumPowerUpsToSpawnOnStart; ++i)
	{
		const int RandomIndex = FMath::RandRange(0, PowerUpsToSpawn.Num()-1);
		const int RandomIndexLocations = FMath::RandRange(0, NumPowerUpsToSpawnOnStart-1);
		GetWorld()->SpawnActor<AActor>(PowerUpsToSpawn[RandomIndex], Locations[RandomIndexLocations], FRotator::ZeroRotator, SpawnInfo);
	}
}

void ARogueGameMode::EnemyDied(ARogueAICharacter* EnemyDied, AActor* Killer)
{
	NumMinionsInLevel--;
	
	if (ARoguePlayerCharacter* PlayerCharacter = Cast<ARoguePlayerCharacter>(Killer))
	{
		// Player was responsible.
		PlayerCharacter->GetPlayerState<ARoguePlayerState>()->AddCredits(EnemyDied->GetCreditsForDeath());
	}
}

void ARogueGameMode::SpawnMinionTimerElapsed()
{
	// We will not spawn minions if > Max allowed.
	
	if (!CVar_ShouldBotsSpawn.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("Bots will not spawn. CVar_ShouldBotsSpawn is false."));
		return;
	}
	
	const int MaxBotsAllowedRightNow = GetMaxMinionsBasedOnDifficulty(GetWorld()->GetTimeSeconds());
	if (NumMinionsInLevel >= MaxBotsAllowedRightNow)
	{
		UE_LOG(LogTemp, Log, TEXT("At Maximum Capacity of %d Minions. Not Spawning..."), MaxBotsAllowedRightNow);
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, EQS_BotSpawnTemplate, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARogueGameMode::OnMinionSpawnQueryCompleted);
	}
}

void ARogueGameMode::OnMinionSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Requested EQS was not successful!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Requested EQS gave no locations!"));
		return;
	}
	
	const FRotator SpawnRotation = FRotator(0.f, FMath::RandRange(0.f, 180.f), 0.f);
	GetWorld()->SpawnActor<AActor>(MinionCharacterClass, Locations[0], SpawnRotation);
	NumMinionsInLevel++;
	UE_LOG(LogTemp, Log, TEXT("Spawned a minion. Current Minions: %d."), NumMinionsInLevel);
	
}

int32 ARogueGameMode::GetMaxMinionsBasedOnDifficulty(const float Time) const
{
	check(DifficultyCurve);
	
	return DifficultyCurve->GetFloatValue(Time);
}
