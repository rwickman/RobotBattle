// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidgetComponent.h"


UHealthWidgetComponent::UHealthWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	static ConstructorHelpers::FClassFinder<UUserWidget> widgetAsset(TEXT("/Game/Blueprints/Widgets/HealthBarHUD"));
	if (widgetAsset.Succeeded())
	{
		
		SetWidgetClass(widgetAsset.Class);
		
	}
}

// Called when the game starts or when spawned
void UHealthWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	currentWidget = Cast<UHealthWidget>(Widget);
	SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));

	// Get the player camera manager
	if (!playerCamManager)
	{
		playerCamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	}
}

// Called every frame
void UHealthWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateWidgetRotation();
	
	//healthBarWidget->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f));
}

void UHealthWidgetComponent::UpdateWidgetRotation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "UpdatingWidget");
	if (playerCamManager)
	{
		FRotator camRot = playerCamManager->GetCameraRotation();
		FRotator healthRot = GetComponentRotation();
		
		FRotator updatedRot(healthRot.Pitch, camRot.Yaw - 180, healthRot.Roll);
		SetWorldRotation(updatedRot);
	}
}

void UHealthWidgetComponent::SetHealthPercentage(float healthProgress)
{
	if (currentWidget)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Updating health progress!");
		}
		currentWidget->healthProgress = healthProgress <= 0.0f ? 0.0f : healthProgress;
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Can't SetHealthPercentage because no currentWidget!");
		}
	}
	
}