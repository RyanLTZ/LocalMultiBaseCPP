// Fill out your copyright notice in the Description page of Project Settings.


#include "LM_MyAICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "Pawn/LMPawnPlayer.h"

// Sets default values
ALM_MyAICharacter::ALM_MyAICharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // �⺻ CapsuleComponent�� �����ͼ� �̺�Ʈ ���ε�
    UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
    CapsuleComp->SetGenerateOverlapEvents(true);
    CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ALM_MyAICharacter::OnCapsuleBeginOverlap);
    CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &ALM_MyAICharacter::OnCapsuleEndOverlap);

    CapsuleComp->SetNotifyRigidBodyCollision(true);
    CapsuleComp->OnComponentHit.AddDynamic(this, &ALM_MyAICharacter::OnCapsuleHit);

}

// Called when the game starts or when spawned
void ALM_MyAICharacter::BeginPlay()
{
    Super::BeginPlay();

    myDirection = FVector(1.0f, 0.f, 0.f);
}

// Called every frame
void ALM_MyAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


    MoveInDirection(DeltaTime);
}

// Called to bind functionality to input
void ALM_MyAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ALM_MyAICharacter::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Log, TEXT("%s began overlap with %s"),
            *GetName(), *OtherActor->GetName());        
        
        ALMPawnPlayer* Player = Cast<ALMPawnPlayer>(OtherActor);
        if (Player)
        {
            Player->DoDie();
        }
    }
}

void ALM_MyAICharacter::ComputeMoveDirection()
{
    // 1) 4���� ���� �迭 ����
    TArray<FVector> Directions = {
        FVector(1.f,  0.f, 0.f),
        FVector(-1.f,  0.f, 0.f),
        FVector(0.f,  1.f, 0.f),
        FVector(0.f, -1.f, 0.f)
    };

    // 2) ���� myDirection�� ���� ��� ����
    //    ��Ȯ ��ġ�� ���ϸ� �̷���, ���� ����Ϸ��� Ŀ���� �� ���� ���
    Directions.Remove(myDirection);

    // 3) ���� ��� �� �ϳ��� �������� ��� myDirection�� �Ҵ�
    if (Directions.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, Directions.Num() - 1);
        myDirection = Directions[Index];
    }
}

void ALM_MyAICharacter::MoveInDirection(float Scale)
{
    if (Scale == 0.f || !Controller)
    {
        UE_LOG(LogTemp, Log, TEXT("Controller is null"));
        return;
    }

    // 1) ���޵� Direction�� ����ȭ
    FVector Dir = myDirection.GetSafeNormal();
    Dir *= myMoveSpeed;
    if (Dir.IsNearlyZero())
    {
        UE_LOG(LogTemp, Log, TEXT("Diretion is nearly zero"));

        return;
    }


    // 2) AddMovementInput ȣ��
    //    - �� ��° ���ڴ� �󸶳� ���ϰ� �̵����� (0~1)
    AddMovementInput(Dir, Scale);
}


void ALM_MyAICharacter::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Log, TEXT("%s ended overlap with %s"),
            *GetName(), *OtherActor->GetName());
    }
}

void ALM_MyAICharacter::OnCapsuleHit(UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        ComputeMoveDirection();

        UE_LOG(LogTemp, Log, TEXT("%s hit %s at %s"),
            *GetName(),
            *OtherActor->GetName(),
            *Hit.ImpactPoint.ToString());
    }
}

void ALM_MyAICharacter::NotifyHit(
    UPrimitiveComponent* MyComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    bool bSelfMoved,
    FVector HitLocation,
    FVector HitNormal,
    FVector NormalImpulse,
    const FHitResult& Hit
)
{
    Super::NotifyHit(MyComp, OtherActor, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    if (OtherActor && OtherActor != this)
    {
        // ���̳� ��ֹ��� ����� �� �� ���� ���
        ComputeMoveDirection();
        UE_LOG(LogTemp, Log, TEXT("%s NotifyHit with %s at %s"),
            *GetName(),
            *OtherActor->GetName(),
            *HitLocation.ToString());
    }
}

