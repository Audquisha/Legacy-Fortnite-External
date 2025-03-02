#pragma once

namespace offsets
{
	namespace UWorld
	{
		constexpr auto PersistentLevel = 0x30; // Size: 8, Type: struct ULevel*
		constexpr auto NetDriver = 0x38; // Size: 8, Type: struct UNetDriver*
		constexpr auto LineBatcher = 0x40; // Size: 8, Type: struct ULineBatchComponent*
		constexpr auto PersistentLineBatcher = 0x48; // Size: 8, Type: struct ULineBatchComponent*
		constexpr auto ForegroundLineBatcher = 0x50; // Size: 8, Type: struct ULineBatchComponent*
		constexpr auto NetworkManager = 0x58; // Size: 8, Type: struct AGameNetworkManager*
		constexpr auto PhysicsCollisionHandler = 0x60; // Size: 8, Type: struct UPhysicsCollisionHandler*
		constexpr auto ExtraReferencedObjects = 0x68; // Size: 16, Type: struct TArray<struct UObject*>
		constexpr auto PerModuleDataObjects = 0x78; // Size: 16, Type: struct TArray<struct UObject*>
		constexpr auto StreamingLevels = 0x88; // Size: 16, Type: struct TArray<struct ULevelStreaming*>
		constexpr auto StreamingLevelsToConsider = 0x98; // Size: 40, Type: struct FStreamingLevelsToConsider
		constexpr auto StreamingLevelsPrefix = 0xc0; // Size: 16, Type: struct FString
		constexpr auto CurrentLevelPendingVisibility = 0xd0; // Size: 8, Type: struct ULevel*
		constexpr auto CurrentLevelPendingInvisibility = 0xd8; // Size: 8, Type: struct ULevel*
		constexpr auto DemoNetDriver = 0xe0; // Size: 8, Type: struct UDemoNetDriver*
		constexpr auto MyParticleEventManager = 0xe8; // Size: 8, Type: struct AParticleEventManager*
		constexpr auto DefaultPhysicsVolume = 0xf0; // Size: 8, Type: struct APhysicsVolume*
		constexpr auto bAreConstraintsDirty = 0x10e; // Size: 1, Type: char
		constexpr auto NavigationSystem = 0x110; // Size: 8, Type: struct UNavigationSystemBase*
		constexpr auto AuthorityGameMode = 0x118; // Size: 8, Type: struct AGameModeBase*
		constexpr auto GameState = 0x120; // Size: 8, Type: struct AGameStateBase*
		constexpr auto AISystem = 0x128; // Size: 8, Type: struct UAISystemBase*
		constexpr auto AvoidanceManager = 0x130; // Size: 8, Type: struct UAvoidanceManager*
		constexpr auto Levels = 0x138; // Size: 16, Type: struct TArray<struct ULevel*>
		constexpr auto LevelCollections = 0x148; // Size: 16, Type: struct TArray<struct FLevelCollection>
		constexpr auto OwningGameInstance = 0x160; // Size: 8, Type: struct UGameInstance*
		constexpr auto ParameterCollectionInstances = 0x168; // Size: 16, Type: struct TArray<struct UMaterialParameterCollectionInstance*>
	
	}

	namespace AActor
	{
		constexpr auto PrimaryActorTick = 0x28; // Size: 48, Type: struct FActorTickFunction
		constexpr auto bNetTemporary = 0x58; // Size: 1, Type: char
		constexpr auto bNetStartup = 0x58; // Size: 1, Type: char
		constexpr auto bOnlyRelevantToOwner = 0x58; // Size: 1, Type: char
		constexpr auto bAlwaysRelevant = 0x58; // Size: 1, Type: char
		constexpr auto bReplicateMovement = 0x58; // Size: 1, Type: char
		constexpr auto bHidden = 0x58; // Size: 1, Type: char
		constexpr auto bTearOff = 0x58; // Size: 1, Type: char
		constexpr auto bExchangedRoles = 0x58; // Size: 1, Type: char
		constexpr auto bNetLoadOnClient = 0x59; // Size: 1, Type: char
		constexpr auto bNetUseOwnerRelevancy = 0x59; // Size: 1, Type: char
		constexpr auto bRelevantForNetworkReplays = 0x59; // Size: 1, Type: char
		constexpr auto bRelevantForLevelBounds = 0x59; // Size: 1, Type: char
		constexpr auto bReplayRewindable = 0x59; // Size: 1, Type: char
		constexpr auto bAllowTickBeforeBeginPlay = 0x59; // Size: 1, Type: char
		constexpr auto bAutoDestroyWhenFinished = 0x59; // Size: 1, Type: char
		constexpr auto bCanBeDamaged = 0x59; // Size: 1, Type: char
		constexpr auto bBlockInput = 0x5a; // Size: 1, Type: char
		constexpr auto bCollideWhenPlacing = 0x5a; // Size: 1, Type: char
		constexpr auto bFindCameraComponentWhenViewTarget = 0x5a; // Size: 1, Type: char
		constexpr auto bGenerateOverlapEventsDuringLevelStreaming = 0x5a; // Size: 1, Type: char
		constexpr auto bIgnoresOriginShifting = 0x5a; // Size: 1, Type: char
		constexpr auto bEnableAutoLODGeneration = 0x5a; // Size: 1, Type: char
		constexpr auto bIsEditorOnlyActor = 0x5a; // Size: 1, Type: char
		constexpr auto bActorSeamlessTraveled = 0x5a; // Size: 1, Type: char
		constexpr auto bReplicates = 0x5b; // Siz	e: 1, Type: char
		constexpr auto bCanBeInCluster = 0x5b; // Size: 1, Type: char
		constexpr auto bAllowReceiveTickEventOnDedicatedServer = 0x5b; // Size: 1, Type: char
		constexpr auto bActorEnableCollision = 0x5c; // Size: 1, Type: char
		constexpr auto bActorIsBeingDestroyed = 0x5c; // Size: 1, Type: char
		constexpr auto UpdateOverlapsMethodDuringLevelStreaming = 0x5d; // Size: 1, Type: enum class EActorUpdateOverlapsMethod
		constexpr auto DefaultUpdateOverlapsMethodDuringLevelStreaming = 0x5e; // Size: 1, Type: enum class EActorUpdateOverlapsMethod
		constexpr auto RemoteRole = 0x5f; // Size: 1, Type: enum class ENetRole
		constexpr auto ReplicatedMovement = 0x60; // Size: 52, Type: struct FRepMovement
		constexpr auto InitialLifeSpan = 0x94; // Size: 4, Type: float
		constexpr auto CustomTimeDilation = 0x98; // Size: 4, Type: float
		constexpr auto AttachmentReplication = 0xa0; // Size: 64, Type: struct FRepAttachment
		constexpr auto Owner = 0x108; // Size: 8, Type: struct AActor*
		constexpr auto NetDriverName = 0x110; // Size: 8, Type: struct FName
		constexpr auto Role = 0x118; // Size: 1, Type: enum class ENetRole
		constexpr auto NetDormancy = 0x119; // Size: 1, Type: enum class ENetDormancy
		constexpr auto SpawnCollisionHandlingMethod = 0x11A; // Size: 1, Type: enum class ESpawnActorCollisionHandlingMethod
		constexpr auto AutoReceiveInput = 0x11B; // Size: 1, Type: enum class EAutoReceiveInput
		constexpr auto InputPriority = 0x11C; // Size: 4, Type: int32_t
		constexpr auto InputComponent = 0x120; // Size: 8, Type: struct UInputComponent*
		constexpr auto NetCullDistanceSquared = 0x128; // Size: 4, Type: float
		constexpr auto NetTag = 0x12C; // Size: 4, Type: int32_t
		constexpr auto NetUpdateFrequency = 0x130; // Size: 4, Type: float
		constexpr auto MinNetUpdateFrequency = 0x134; // Size: 4, Type: float
		constexpr auto NetPriority = 0x138; // Size: 4, Type: float
		constexpr auto Instigator = 0x140; // Size: 8, Type: struct APawn*
		constexpr auto Children = 0x148; // Size: 16, Type: struct TArray<struct AActor*>
		constexpr auto RootComponent = 0x158; // Size: 8, Type: struct USceneComponent*
		constexpr auto ControllingMatineeActors = 0x160; // Size: 16, Type: struct TArray<struct AMatineeActor*>
		constexpr auto Layers = 0x178; // Size: 16, Type: struct TArray<struct FName>
		constexpr auto ParentComponent = 0x188; // Size: 8, Type: struct TWeakObjectPtr<UChildActorComponent>
		constexpr auto Tags = 0x190; // Size: 16, Type: struct TArray<struct FName>
		
	}

	namespace USceneComponent
	{
		constexpr auto PhysicsVolume = 0x100; // Size: 8, Type: struct TWeakObjectPtr<APhysicsVolume>
		constexpr auto AttachParent = 0x108; // Size: 8, Type: struct USceneComponent*
		constexpr auto AttachSocketName = 0x110; // Size: 8, Type: struct FName
		constexpr auto AttachChildren = 0x118; // Size: 16, Type: struct TArray<struct USceneComponent*>
		constexpr auto ClientAttachedChildren = 0x128; // Size: 16, Type: struct TArray<struct USceneComponent*>
		constexpr auto RelativeLocation = 0x164; // Size: 12, Type: struct FVector
		constexpr auto RelativeRotation = 0x170; // Size: 12, Type: struct FRotator
		constexpr auto RelativeScale3D = 0x17C; // Size: 12, Type: struct FVector
		constexpr auto ComponentVelocity = 0x1C0; // Size: 12, Type: struct FVector

	}





}
	