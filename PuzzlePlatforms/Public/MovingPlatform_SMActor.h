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

	// FVector�� �����Ϳ� �������� ��������
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
		FVector TargetLocation;		// ��ȯ�� ��ǥ

public:
	void AddActiveTrigger();
	void RemoveActiveTrigger();
private:
	FVector GlobalTargetLocation;	// ���� ��ġ
	FVector GlobalStartLocation;	// ���� ��ġ

	UPROPERTY(EditAnywhere)
		int ActiveTrigger = 1;
};
