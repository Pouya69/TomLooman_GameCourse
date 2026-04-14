// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RogueGameMode.generated.h"


class ARoguePowerupBase;
class ARogueAICharacter;

namespace EEnvQueryStatus
{
	enum Type : int;
}


class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class AController;


UCLASS(Abstract)
class TOMLOOMAN_GAMECOURSE_API ARogueGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARogueGameMode();
	
	virtual void StartPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay | AI")
		void EnemyDied(ARogueAICharacter* EnemyDied, AActor* Killer);
	
	UFUNCTION(Exec)
		void KillAllAI();
	
	UFUNCTION(Exec)
		void KillPlayer(const uint8 PlayerIndex = 0);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
		void OnActorKilled(AActor* VictimActor, AActor* KillerActor);
	virtual void OnActorKilled_Implementation(AActor* VictimActor, AActor* KillerActor);
	
protected:
	FTimerHandle MinionSpawnTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | AI")
		float SpawnMinionsEverySeconds;
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | AI")
		TObjectPtr<UEnvQuery> EQS_BotSpawnTemplate;
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | AI")
		TSubclassOf<AActor> MinionCharacterClass;
	
	// Only gets called every 'SpawnMinionsEverySeconds' Seconds using EQS. Avoid using manually.
	UFUNCTION(BlueprintCallable, Category="Gameplay | AI")
		void SpawnMinionTimerElapsed();
	
	// This is after the EQS call once it is finished.
	UFUNCTION()
		void OnMinionSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	// If not 0, it will use this as starting amount of minions.
	// If 0, it will count them O(N)
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | AI")
		int32 NumMinionsInLevel;
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | AI")
		TObjectPtr<UCurveFloat> DifficultyCurve;
	
	// Goes up over time with the UCurveFloat we have.
	UFUNCTION(BlueprintCallable, Category="Gameplay | AI")
		int32 GetMaxMinionsBasedOnDifficulty(const float Time) const;
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | Player")
		float PlayerRespawnTimerInSeconds;
	
	UFUNCTION(BlueprintCallable, Category="Gameplay | Player")
		void RespawnPlayerTimerElapsed(AController* PlayerController);
		
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | Environment")
		int NumPowerUpsToSpawnOnStart;
	
	// For the game start.
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | Environment")
		TArray<TSubclassOf<ARoguePowerupBase>> PowerUpsToSpawn;
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay | Environment")
		TObjectPtr<UEnvQuery> PowerUPSpawn_EQS;
	
	UFUNCTION(BlueprintCallable, Category="Gameplay | Environment")
		void SpawnPowerUpsRandom();
	
	UFUNCTION()
	void OnSpawnPowerUpsQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
};
