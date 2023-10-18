


#include "Character/PstCharacter.h"
#include "Character/PstCharacterSetting.h"
#include "Character/PstCharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/PstPlayerState.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Input/PstInputComponent.h"
#include "Input/PstInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbility/PstAbilitySystemComponent.h"
#include "GameplayAbility/PstAttributeSet_Character.h"
#include "AI/PstAIController.h"
#include "PstGameplayTags.h"
#include "Weapon/PstWeaponComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstCharacter)

// Sets default values
APstCharacter::APstCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPstCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.2f;

	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		FRotator(0.0f, -90.0f, 0.0f));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->MaxAcceleration = 1024.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.5f;
	GetCharacterMovement()->BrakingDecelerationWalking = 512.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = false;
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
	GetCharacterMovement()->JumpZVelocity = 520.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->SocketOffset = FVector(0.0f, 40.0f, 65.0f);
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 24.0f;
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	WeaponComponent = CreateDefaultSubobject<UPstWeaponComponent>(TEXT("WeaponComponent"));

	// Perception
	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));

	AIControllerClass = APstAIController::StaticClass();

	bIsCombatMode = false;
	ClearCombatModeTime = 4.0f;
}

void APstCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APstCharacter, DissolveOpacity);
}

// Called when the game starts or when spawned
void APstCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
}

void APstCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (APstPlayerState* PS = GetPlayerState<APstPlayerState>())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = PS->GetAbilitySystemComponent())
		{
			AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		}
	}
}

void APstCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APstPlayerState* PstPlayerState = GetPlayerState<APstPlayerState>())
	{
		if (CharacterSetting)
		{
			PstPlayerState->SetCharacterSetting(CharacterSetting);
		}

		if (UPstAbilitySystemComponent* AbilitySystemComponent = PstPlayerState->GetAbilitySystemComponent<UPstAbilitySystemComponent>())
		{
			AbilitySystemComponent->InitAbilityActorInfo(PstPlayerState, this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPstAttributeSet_Character::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);
		}
	}
}

// Called every frame
void APstCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void APstCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PlayerController = GetController<APlayerController>();
	check(PlayerController);

	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->GetLocalPlayer());
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (CharacterSetting)
	{
		if (const UPstInputConfig* InputConfig = CharacterSetting->InputConfig)
		{
			for (const UInputMappingContext* InputMappingContext : InputMappings)
			{
				FModifyContextOptions Options = {};
				Options.bIgnoreAllPressedKeysUntilRelease = false;
				Subsystem->AddMappingContext(InputMappingContext, 0, Options);
			}

			if (UPstInputComponent* PstInputComponent = Cast<UPstInputComponent>(PlayerInputComponent))
			{
				PstInputComponent->AddInputMappings(InputConfig, Subsystem);

				TArray<uint32> BindHandles;
				PstInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased, BindHandles);

				PstInputComponent->BindNativeAction(InputConfig, PstGameplayTags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
				PstInputComponent->BindNativeAction(InputConfig, PstGameplayTags::Input_Look, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
			}
		}
	}
}

UAbilitySystemComponent* APstCharacter::GetAbilitySystemComponent() const
{
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetPlayerState());
	if (ASI)
	{
		return ASI->GetAbilitySystemComponent();
	}

	return nullptr;
}

void APstCharacter::MoveInput(const FInputActionValue& InputActionValue)
{
	AController* ControllerLocal = GetController();
	if (ControllerLocal)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, ControllerLocal->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void APstCharacter::LookInput(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		AddControllerPitchInput(Value.Y);
	}
}

void APstCharacter::AbilityInputPressed(FGameplayTag InputTag)
{
	if (UPstAbilitySystemComponent* PstASC = GetAbilitySystemComponent<UPstAbilitySystemComponent>())
	{
		PstASC->AbilityInputTagPressed(InputTag);
	}
}

void APstCharacter::AbilityInputReleased(FGameplayTag InputTag)
{
	if (UPstAbilitySystemComponent* PstASC = GetAbilitySystemComponent<UPstAbilitySystemComponent>())
	{
		PstASC->AbilityInputTagReleased(InputTag);
	}
}

void APstCharacter::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.0f)
	{
		BeginDeath();
	}
}

void APstCharacter::BeginDeath()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->StopMovementImmediately();

	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent())
	{
		AbilitySystemComponent->CancelAllAbilities();
	}

	if (APstAIController* AIController = Cast<APstAIController>(GetController()))
	{
		AIController->StopAI();
	}

	// Start dissolve
	FTimerDelegate DissolveTimerDelegate = FTimerDelegate::CreateLambda([this]() -> void
		{
			float NewDissolveOpacity = DissolveOpacity - 0.05f;
			SetDissolveOpacity(NewDissolveOpacity);

			if (NewDissolveOpacity < KINDA_SMALL_NUMBER)
			{
				FinishDeath();
			}
		});
	GetWorldTimerManager().SetTimer(DissolveTimerHandle, DissolveTimerDelegate, 0.05f, true);
}

void APstCharacter::FinishDeath()
{
	GetWorldTimerManager().ClearTimer(DissolveTimerHandle);
	GetMesh()->SetVisibility(false, true);

	Destroy();
}

FGenericTeamId APstCharacter::GetGenericTeamId() const
{
	if (IGenericTeamAgentInterface* GenericTeamAgentInterface = Cast<IGenericTeamAgentInterface>(GetController()))
	{
		return GenericTeamAgentInterface->GetGenericTeamId();
	}

	return FGenericTeamId::NoTeam;
}

bool APstCharacter::IsCombatMode() const
{
	return bIsCombatMode;
}

void APstCharacter::SetCombatMode(bool NewCombatMode, bool bAutoDisable)
{
	if (GetWorldTimerManager().IsTimerActive(ClearCombatModeTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(ClearCombatModeTimerHandle);
	}

	if (NewCombatMode == true)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;

		if (bAutoDisable)
		{
			GetWorldTimerManager().SetTimer(ClearCombatModeTimerHandle, FTimerDelegate::CreateLambda(
				[this]() -> void
				{
					if (IsValid(this))
					{
						SetCombatMode(false);
					}
				}), ClearCombatModeTime, false);
		}
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	bIsCombatMode = NewCombatMode;
}

void APstCharacter::FlickHitFeedbackMaterial()
{
	GetMesh()->SetOverlayMaterial(HitFeedbackMaterial);

	FSimpleDelegate FlickHitFeedbackMaterialDelegate = FTimerDelegate::CreateLambda([this]() -> void
		{
			GetMesh()->SetOverlayMaterial(nullptr);
		});

	GetWorldTimerManager().SetTimer(FlickHitFeedbackMaterialTimerHandle, FlickHitFeedbackMaterialDelegate, FlickHitFeedbackMaterialTime, false);
}

float APstCharacter::GetDamageMultiplierByName_Implementation(const FName BoneName) const
{
	return 1.0f;
}

bool APstCharacter::IsWeakPointBody_Implementation(const FName BoneName) const
{
	return false;
}

void APstCharacter::SetDissolveOpacity(float NewDissolveOpacity)
{
	DissolveOpacity = NewDissolveOpacity;

	TArray<UMaterialInterface*> MeshMaterials = GetMesh()->GetMaterials();
	for (int32 MaterialIndex = 0; MaterialIndex < MeshMaterials.Num(); MaterialIndex++)
	{
		if (IsValid(MeshMaterials[MaterialIndex]))
		{
			UMaterialInstanceDynamic* MeshMaterialInstanceDynamic = GetMesh()->CreateDynamicMaterialInstance(MaterialIndex);
			MeshMaterialInstanceDynamic->SetScalarParameterValue(TEXT("DissolveOpacity"), DissolveOpacity);
		}
	}
}

void APstCharacter::OnRep_DissolveOpacity()
{
	TArray<UMaterialInterface*> MeshMaterials = GetMesh()->GetMaterials();
	for (int32 MaterialIndex = 0; MaterialIndex < MeshMaterials.Num(); MaterialIndex++)
	{
		if (IsValid(MeshMaterials[MaterialIndex]))
		{
			UMaterialInstanceDynamic* MeshMaterialInstanceDynamic = GetMesh()->CreateDynamicMaterialInstance(MaterialIndex);
			MeshMaterialInstanceDynamic->SetScalarParameterValue(TEXT("DissolveOpacity"), DissolveOpacity);
		}
	}
}