// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LM_MyAICharacter.generated.h"

UCLASS()
class LOCALMULTIBASECPP_API ALM_MyAICharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ALM_MyAICharacter();

protected:
    virtual void BeginPlay() override;

    // Capsule 컴포넌트 겹침 시작
    UFUNCTION()
    void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    // Capsule 컴포넌트 겹침 종료
    UFUNCTION()
    void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    // Capsule 컴포넌트 물리 충돌
    UFUNCTION()
    void OnCapsuleHit(UPrimitiveComponent* HitComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit);

    // 이 함수를 오버라이드합니다.
    virtual void NotifyHit(
        UPrimitiveComponent* MyComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        bool bSelfMoved,
        FVector HitLocation,
        FVector HitNormal,
        FVector NormalImpulse,
        const FHitResult& Hit
    ) override;


    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveInDirection(float Scale);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ComputeMoveDirection();

private:
    FVector myDirection = FVector(0.f, 0.f, 0.f);

    UPROPERTY(EditAnywhere, Category = "Movement")
    float myMoveSpeed = 100.f;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
