// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform_SMActor.h"

AMovingPlatform_SMActor::AMovingPlatform_SMActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform_SMActor::BeginPlay()
{
	Super::BeginPlay();

	// 서버는 권위적이다 따라서 서버의 값이 맞다고 생각하며 클라이언트에서 값을 변동하더라도
	// 서버의 값과 다르다면 서버의 값이 맞다고 판단한다.
	if(HasAuthority())
	{
		// 이 액터를 클라이언트에 복제한다.
		SetReplicates(true);
		// 이 액터의 움직임을 클라이언트에 복제한다.
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform_SMActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// true 라면 서버 false 클라
	if (HasAuthority())
	{
		FVector location = GetActorLocation();

		// 총 길이
		float journeyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		// 이동한 길이
		float journeyTravelled = (location - GlobalStartLocation).Size();
		// 이동한 길이기 총 길이보다 크거나 같다면 목표를 넘은 것이다 따라서 방향 바꾸기
		if(journeyTravelled >= journeyLength)
		{
			FVector temp = GlobalTargetLocation;
			GlobalTargetLocation = GlobalStartLocation;
			GlobalStartLocation = temp;
		}

		// 월드 좌표로 치환
		FVector dir = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		location += dir * MoveSpeed * DeltaSeconds;

		SetActorLocation(location);
	}
}
