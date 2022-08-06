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

	// ������ �������̴� ���� ������ ���� �´ٰ� �����ϸ� Ŭ���̾�Ʈ���� ���� �����ϴ���
	// ������ ���� �ٸ��ٸ� ������ ���� �´ٰ� �Ǵ��Ѵ�.
	if(HasAuthority())
	{
		// �� ���͸� Ŭ���̾�Ʈ�� �����Ѵ�.
		SetReplicates(true);
		// �� ������ �������� Ŭ���̾�Ʈ�� �����Ѵ�.
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform_SMActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// true ��� ���� false Ŭ��
	if (HasAuthority())
	{
		FVector location = GetActorLocation();

		// �� ����
		float journeyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		// �̵��� ����
		float journeyTravelled = (location - GlobalStartLocation).Size();
		// �̵��� ���̱� �� ���̺��� ũ�ų� ���ٸ� ��ǥ�� ���� ���̴� ���� ���� �ٲٱ�
		if(journeyTravelled >= journeyLength)
		{
			FVector temp = GlobalTargetLocation;
			GlobalTargetLocation = GlobalStartLocation;
			GlobalStartLocation = temp;
		}

		// ���� ��ǥ�� ġȯ
		FVector dir = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		location += dir * MoveSpeed * DeltaSeconds;

		SetActorLocation(location);
	}
}
