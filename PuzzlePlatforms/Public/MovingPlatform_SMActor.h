// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform_SMActor.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform_SMActor : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	AMovingPlatform_SMActor();


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
		float MoveSpeed = 5.f;

	// FVector를 에디터에 위젯으로 나오게함
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
		FVector TargetLocation;		// 반환점 목표

public:
	void AddActiveTrigger();
	void RemoveActiveTrigger();
private:
	FVector GlobalTargetLocation;	// 도착 위치
	FVector GlobalStartLocation;	// 시작 위치

	UPROPERTY(EditAnywhere)
		int ActiveTrigger = 1;
};
