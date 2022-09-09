// Fill out your copyright notice in the Description page of Project Settings.PickUp/


#include "Voxel/Chunks/VoxelChunk.h"

#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Ability/PickUp/AbilityPickUpBase.h"
#include "Ability/PickUp/AbilityPickUpEquip.h"
#include "Ability/PickUp/AbilityPickUpProp.h"
#include "Ability/PickUp/AbilityPickUpVoxel.h"
#include "Ability/PickUp/AbilityPickUpSkill.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Audio/AudioModuleBPLibrary.h"
#include "Character/Base/CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathBPLibrary.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Agent/VoxelAgentInterface.h"
#include "Voxel/Datas/VoxelData.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Voxels/Voxel.h"
#include "Voxel/Voxels/Auxiliary/VoxelAuxiliary.h"
#include "ObjectPool/ObjectPoolModuleBPLibrary.h"
#include "Voxel/VoxelModuleBPLibrary.h"

// Sets default values
AVoxelChunk::AVoxelChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SolidMesh = CreateDefaultSubobject<UVoxelMeshComponent>(TEXT("SolidMesh"));
	SolidMesh->SetupAttachment(RootComponent);
	SolidMesh->Initialize(EVoxelMeshNature::Chunk, EVoxelTransparency::Solid);
	SolidMesh->OnComponentHit.AddDynamic(this, &AVoxelChunk::OnCollision);

	SemiMesh = CreateDefaultSubobject<UVoxelMeshComponent>(TEXT("SemiMesh"));
	SemiMesh->SetupAttachment(RootComponent);
	SemiMesh->Initialize(EVoxelMeshNature::Chunk, EVoxelTransparency::SemiTransparent);
	SemiMesh->OnComponentHit.AddDynamic(this, &AVoxelChunk::OnCollision);

	TransMesh = CreateDefaultSubobject<UVoxelMeshComponent>(TEXT("TransMesh"));
	TransMesh->SetupAttachment(RootComponent);
	TransMesh->Initialize(EVoxelMeshNature::Chunk, EVoxelTransparency::Transparent);
	TransMesh->OnComponentBeginOverlap.AddDynamic(this, &AVoxelChunk::OnBeginOverlap);
	TransMesh->OnComponentEndOverlap.AddDynamic(this, &AVoxelChunk::OnEndOverlap);

	Batch = -1;
	Module = nullptr;
	Index = FVector();
	bGenerated = false;
	bActorsGenerated = false;
	VoxelMap = TMap<FIndex, FVoxelItem>();
	Neighbors = TMap<EDirection, AVoxelChunk*>();
	DIRECTION_ITERATOR(Iter, Neighbors.Add(Iter); )
	PickUps = TArray<AAbilityPickUpBase*>();
}

void AVoxelChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AVoxelChunk::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	auto& SaveData = InSaveData->CastRef<FVoxelChunkSaveData>();
	for(auto& Iter : SaveData.VoxelDatas)
	{
		const FVoxelItem VoxelItem = FVoxelItem(Iter);
		SetVoxelSample(VoxelItem.Index, VoxelItem);
	}
}

FSaveData* AVoxelChunk::ToData()
{
	static FVoxelChunkSaveData SaveData;
	SaveData = FVoxelChunkSaveData();

	SaveData.Index = Index;

	for(auto& Iter : VoxelMap)
	{
		SaveData.VoxelDatas.Add(Iter.Value.ToSaveData(true));
	}

	for(auto& Iter : PickUps)
	{
		SaveData.PickUpDatas.Add(Iter->ToSaveDataRef<FPickUpSaveData>());
	}

	return &SaveData;
}

void AVoxelChunk::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor && OtherActor->IsA<ACharacterBase>())
	{
		if(IVoxelAgentInterface* voxelAgent = Cast<IVoxelAgentInterface>(OtherActor))
		{
			const FVoxelItem& voxelItem = GetVoxelItem(LocationToIndex(Hit.ImpactPoint - AVoxelModule::Get()->GetWorldData().GetBlockSizedNormal(Hit.ImpactNormal)), true);
			if(voxelItem.IsValid())
			{
				voxelItem.GetVoxel().OnAgentHit(voxelAgent, FVoxelHitResult(voxelItem, Hit.ImpactPoint, Hit.ImpactNormal));
			}
		}
	}
}

void AVoxelChunk::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AVoxelChunk::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AVoxelChunk::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
}

void AVoxelChunk::OnDespawn_Implementation()
{
	for(auto& Iter : VoxelMap)
	{
		DestroyAuxiliary(Iter.Key);
	}
	
	DestroyActors();

	BreakNeighbors();

	Module = nullptr;
	Index = FIndex::ZeroIndex;
	Batch = -1;
	bGenerated = false;
	bActorsGenerated = false;

	SolidMesh->ClearData();
	SemiMesh->ClearData();
	TransMesh->ClearData();

	VoxelMap.Empty();

	SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);

	Execute_SetActorVisible(this, false);
}

void AVoxelChunk::SetActorVisible_Implementation(bool bNewVisible)
{
	Super::SetActorVisible_Implementation(bNewVisible);

	SolidMesh->SetCollisionEnabled(bNewVisible);
	SemiMesh->SetCollisionEnabled(bNewVisible);
	TransMesh->SetCollisionEnabled(bNewVisible);
}

void AVoxelChunk::Initialize(AVoxelModule* InModule, FIndex InIndex, int32 InBatch)
{
	Module = InModule;
	Index = InIndex;
	Batch = InBatch;
	UpdateNeighbors();
}

void AVoxelChunk::Generate(bool bBuildMesh, bool bForceMode)
{
	if(bBuildMesh)
	{
		BuildMesh();
	}

	if(!bGenerated)
	{
		Execute_SetActorVisible(this, true);
	}

	SolidMesh->CreateMesh();
	SemiMesh->CreateMesh();
	TransMesh->CreateMesh();

	if(!bGenerated)
	{
		bGenerated = true;
		for(auto& Iter : VoxelMap)
		{
			Iter.Value.OnGenerate();
		}
		if(bForceMode)
		{
			SpawnActors();
		}
	}
}

void AVoxelChunk::BuildMap(int32 InStage)
{
	const auto& worldData = AVoxelModule::Get()->GetWorldData();
	switch (InStage)
	{
		case 0:
		{
			INDEX_ITERATOR(voxelIndex, FVector(worldData.ChunkSize), false,
				switch(const EVoxelType voxelType = UVoxelModuleBPLibrary::GetNoiseVoxelType(LocalIndexToWorld(voxelIndex)))
				{
					// Tree
					case EVoxelType::Oak:
					case EVoxelType::Birch:
					{
						if(!CheckVoxelNeighbors(voxelIndex, EVoxelType::Oak, FVector(3.f), true) && !CheckVoxelNeighbors(voxelIndex, EVoxelType::Birch, FVector(3.f), true))
						{
							SetVoxelSample(voxelIndex, voxelType);
						}
						break;
					}
					default:
					{
						SetVoxelSample(voxelIndex, voxelType);
						break;
					}
					case EVoxelType::Empty: break;
				}
			)
			break;
		}
		case 1:
		{
			const FRandomStream& randomStream = worldData.RandomStream;
			const TMap<FIndex, FVoxelItem> voxelMap = VoxelMap;
			for(auto iter : voxelMap)
			{
				const FIndex& voxelIndex = iter.Key;
				switch(const EVoxelType& voxelType = iter.Value.GetVoxelType())
				{
					// Tree
					case EVoxelType::Oak:
					case EVoxelType::Birch:
					{
						if(!HasVoxel(voxelIndex + FIndex(0, 0, 1)))
						{
							const EVoxelType leavesType = (voxelType == EVoxelType::Oak ? EVoxelType::Oak_Leaves : EVoxelType::Birch_Leaves);
							const int32 treeHeight = randomStream.RandRange(4, 5);
							const int32 leavesHeight = 2;
							const int32 leavesWidth = randomStream.FRand() < 0.5f ? 3 : 5;
							DON(i, treeHeight - 1,
								SetVoxelComplex(voxelIndex + FIndex(0, 0, i + 1), voxelType);
							)
							for(int32 offsetZ = treeHeight - leavesHeight / 2; offsetZ <= treeHeight + leavesHeight / 2; offsetZ++)
							{
								for(int32 offsetX = -leavesWidth / 2; offsetX <= leavesWidth / 2; offsetX++)
								{
									for(int32 offsetY = -leavesWidth / 2; offsetY <= leavesWidth / 2; offsetY++)
									{
										SetVoxelComplex(voxelIndex + FIndex(offsetX, offsetY, offsetZ - 1), leavesType);
									}
								}
							}
						}
						break;
					}
					default: break;
				}
			}
			break;
		}
		default: break;
	}
}

void AVoxelChunk::BuildMesh()
{
	for(auto iter = VoxelMap.CreateConstIterator(); iter; ++iter)
	{
		const FVoxelItem& voxelItem = iter->Value;
		if(voxelItem.IsValid())
		{
			switch(voxelItem.GetVoxelData().Transparency)
			{
				case EVoxelTransparency::Solid:
				{
					SolidMesh->BuildVoxel(voxelItem);
					break;
				}
				case EVoxelTransparency::SemiTransparent:
				{
					SemiMesh->BuildVoxel(voxelItem);
					break;
				}
				case EVoxelTransparency::Transparent:
				{
					TransMesh->BuildVoxel(voxelItem);
					break;
				}
				default: break;
			}
		}
	}
}


void AVoxelChunk::SpawnActors()
{
	auto& worldData = AVoxelModule::Get()->GetWorldData();
	if(worldData.IsExistChunkData(Index))
	{
		LoadActors(worldData.GetChunkData(Index));
	}
	else if(!bActorsGenerated && Module->IsChunkGenerated(Index, true))
	{
		GenerateActors();
	}
}

void AVoxelChunk::LoadActors(FSaveData* InSaveData)
{
	bActorsGenerated = true;

	auto& SaveData = InSaveData->CastRef<FVoxelChunkSaveData>();
	for(int32 i = 0; i < SaveData.PickUpDatas.Num(); i++)
	{
		UAbilityModuleBPLibrary::SpawnPickUp(&SaveData.PickUpDatas[i], this);
	}
}

void AVoxelChunk::GenerateActors()
{
	for(auto Iter : Module->GetVerticalChunks(Index))
	{
		Iter->bActorsGenerated = true;
	}
}

void AVoxelChunk::DestroyActors()
{
	while(PickUps.Num() > 0)
	{
		UObjectPoolModuleBPLibrary::DespawnObject(PickUps[0]);
	}
	PickUps.Empty();
}

void AVoxelChunk::GenerateNeighbors(FIndex InIndex, bool bForceMode)
{
	GenerateNeighbors(InIndex.X, InIndex.Y, InIndex.Z, bForceMode);
}

void AVoxelChunk::GenerateNeighbors(int32 InX, int32 InY, int32 InZ, bool bForceMode)
{
	if(InX <= 0 && GetNeighbor(EDirection::Backward))
	{
		GetNeighbor(EDirection::Backward)->Generate(true, bForceMode);
	}
	else if(InX >= AVoxelModule::Get()->GetWorldData().ChunkSize - 1 && GetNeighbor(EDirection::Forward))
	{
		GetNeighbor(EDirection::Forward)->Generate(true, bForceMode);
	}
	if(InY <= 0 && GetNeighbor(EDirection::Left))
	{
		GetNeighbor(EDirection::Left)->Generate(true, bForceMode);
	}
	else if(InY >= AVoxelModule::Get()->GetWorldData().ChunkSize - 1 && GetNeighbor(EDirection::Right))
	{
		GetNeighbor(EDirection::Right)->Generate(true, bForceMode);
	}
	if(InZ <= 0 && GetNeighbor(EDirection::Down))
	{
		GetNeighbor(EDirection::Down)->Generate(true, bForceMode);
	}
	else if(InZ >= AVoxelModule::Get()->GetWorldData().ChunkSize - 1 && GetNeighbor(EDirection::Up))
	{
		GetNeighbor(EDirection::Up)->Generate(true, bForceMode);
	}
}

void AVoxelChunk::UpdateNeighbors()
{
	DIRECTION_ITERATOR(Direction,
		Neighbors[Direction] = Module->FindChunkByIndex(Index + UMathBPLibrary::DirectionToIndex(Direction));
		if(Neighbors[Direction])
		{
			Neighbors[Direction]->Neighbors[UMathBPLibrary::InvertDirection((EDirection)Direction)] = this;
		}
	)
}

void AVoxelChunk::BreakNeighbors()
{
	DIRECTION_ITERATOR(Direction,
		if(Neighbors[Direction])
		{
			Neighbors[Direction]->Neighbors[UMathBPLibrary::InvertDirection(Direction)] = nullptr;
			Neighbors[Direction] = nullptr;
		}
	)
}

bool AVoxelChunk::IsOnTheChunk(FIndex InIndex) const
{
	return InIndex.X >= 0 && InIndex.X < AVoxelModule::Get()->GetWorldData().ChunkSize &&
		InIndex.Y >= 0 && InIndex.Y < AVoxelModule::Get()->GetWorldData().ChunkSize &&
		InIndex.Z >= 0 && InIndex.Z < AVoxelModule::Get()->GetWorldData().ChunkSize;
}

bool AVoxelChunk::IsOnTheChunk(FVector InLocation) const
{
	return InLocation.X >= GetChunkLocation().X && InLocation.X < GetChunkLocation().X + AVoxelModule::Get()->GetWorldData().GetChunkLength() &&
		InLocation.Y >= GetChunkLocation().Y && InLocation.Y < GetChunkLocation().Y + AVoxelModule::Get()->GetWorldData().GetChunkLength() &&
		InLocation.Z >= GetChunkLocation().Z && InLocation.Z < GetChunkLocation().Z + AVoxelModule::Get()->GetWorldData().GetChunkLength();
}

FIndex AVoxelChunk::LocationToIndex(FVector InLocation, bool bWorldSpace /*= true*/) const
{
	InLocation = (InLocation - (bWorldSpace ? GetChunkLocation() : FVector::ZeroVector)) / AVoxelModule::Get()->GetWorldData().BlockSize;
	return FIndex(FMath::FloorToInt(InLocation.X), FMath::FloorToInt(InLocation.Y), FMath::FloorToInt(InLocation.Z));
}

FVector AVoxelChunk::IndexToLocation(FIndex InIndex, bool bWorldSpace /*= true*/) const
{
	return InIndex.ToVector() * AVoxelModule::Get()->GetWorldData().BlockSize + (bWorldSpace ? GetChunkLocation() : FVector::ZeroVector);
}

FIndex AVoxelChunk::LocalIndexToWorld(FIndex InIndex) const
{
	return InIndex + Index * AVoxelModule::Get()->GetWorldData().ChunkSize;
}

FIndex AVoxelChunk::WorldIndexToLocal(FIndex InIndex) const
{
	return InIndex - Index * AVoxelModule::Get()->GetWorldData().ChunkSize;
}

bool AVoxelChunk::HasVoxel(FIndex InIndex)
{
	return HasVoxel(InIndex.X, InIndex.Y, InIndex.Z);
}

bool AVoxelChunk::HasVoxel(int32 InX, int32 InY, int32 InZ)
{
	if(InX < 0)
	{
		if(GetNeighbor(EDirection::Backward))
		{
			return GetNeighbor(EDirection::Backward)->HasVoxel(InX + AVoxelModule::Get()->GetWorldData().ChunkSize, InY, InZ);
		}
	}
	else if(InX >= AVoxelModule::Get()->GetWorldData().ChunkSize)
	{
		if(GetNeighbor(EDirection::Forward))
		{
			return GetNeighbor(EDirection::Forward)->HasVoxel(InX - AVoxelModule::Get()->GetWorldData().ChunkSize, InY, InZ);
		}
	}
	else if(InY < 0)
	{
		if(GetNeighbor(EDirection::Left))
		{
			return GetNeighbor(EDirection::Left)->HasVoxel(InX, InY + AVoxelModule::Get()->GetWorldData().ChunkSize, InZ);
		}
	}
	else if(InY >= AVoxelModule::Get()->GetWorldData().ChunkSize)
	{
		if(GetNeighbor(EDirection::Right))
		{
			return GetNeighbor(EDirection::Right)->HasVoxel(InX, InY - AVoxelModule::Get()->GetWorldData().ChunkSize, InZ);
		}
	}
	else if(InZ < 0)
	{
		if(GetNeighbor(EDirection::Down))
		{
			return GetNeighbor(EDirection::Down)->HasVoxel(InX, InY, InZ + AVoxelModule::Get()->GetWorldData().ChunkSize);
		}
	}
	else if(InZ >= AVoxelModule::Get()->GetWorldData().ChunkSize)
	{
		if(GetNeighbor(EDirection::Up))
		{
			return GetNeighbor(EDirection::Up)->HasVoxel(InX, InY, InZ - AVoxelModule::Get()->GetWorldData().ChunkSize);
		}
	}
	return VoxelMap.Contains(FIndex(InX, InY, InZ));
}

UVoxel& AVoxelChunk::GetVoxel(FIndex InIndex, bool bMainPart)
{
	return GetVoxel(InIndex.X, InIndex.Y, InIndex.Z, bMainPart);
}

UVoxel& AVoxelChunk::GetVoxel(int32 InX, int32 InY, int32 InZ, bool bMainPart)
{
	return GetVoxelItem(InX, InY, InZ, bMainPart).GetVoxel();
}

FVoxelItem& AVoxelChunk::GetVoxelItem(FIndex InIndex, bool bMainPart)
{
	return GetVoxelItem(InIndex.X, InIndex.Y, InIndex.Z, bMainPart);
}

FVoxelItem& AVoxelChunk::GetVoxelItem(int32 InX, int32 InY, int32 InZ, bool bMainPart)
{
	if(InX < 0)
	{
		if(GetNeighbor(EDirection::Backward))
		{
			return GetNeighbor(EDirection::Backward)->GetVoxelItem(InX + AVoxelModule::Get()->GetWorldData().ChunkSize, InY, InZ, bMainPart);
		}
		return FVoxelItem::Unknown;
	}
	else if(InX >= AVoxelModule::Get()->GetWorldData().ChunkSize)
	{
		if(GetNeighbor(EDirection::Forward))
		{
			return GetNeighbor(EDirection::Forward)->GetVoxelItem(InX - AVoxelModule::Get()->GetWorldData().ChunkSize, InY, InZ, bMainPart);
		}
		return FVoxelItem::Unknown;
	}
	else if(InY < 0)
	{
		if(GetNeighbor(EDirection::Left))
		{
			return GetNeighbor(EDirection::Left)->GetVoxelItem(InX, InY + AVoxelModule::Get()->GetWorldData().ChunkSize, InZ, bMainPart);
		}
		return FVoxelItem::Unknown;
	}
	else if(InY >= AVoxelModule::Get()->GetWorldData().ChunkSize)
	{
		if(GetNeighbor(EDirection::Right))
		{
			return GetNeighbor(EDirection::Right)->GetVoxelItem(InX, InY - AVoxelModule::Get()->GetWorldData().ChunkSize, InZ, bMainPart);
		}
		return FVoxelItem::Unknown;
	}
	else if(InZ < 0)
	{
		if(GetNeighbor(EDirection::Down))
		{
			return GetNeighbor(EDirection::Down)->GetVoxelItem(InX, InY, InZ + AVoxelModule::Get()->GetWorldData().ChunkSize, bMainPart);
		}
		else if(Index.Z > 0)
		{
			return FVoxelItem::Unknown;
		}
	}
	else if(InZ >= AVoxelModule::Get()->GetWorldData().ChunkSize)
	{
		if(GetNeighbor(EDirection::Up))
		{
			return GetNeighbor(EDirection::Up)->GetVoxelItem(InX, InY, InZ - AVoxelModule::Get()->GetWorldData().ChunkSize, bMainPart);
		}
		else if(Index.Z < AVoxelModule::Get()->GetWorldData().ChunkHeightRange)
		{
			return FVoxelItem::Unknown;
		}
	}
	else if(HasVoxel(InX, InY, InZ))
	{
		const FIndex voxelIndex = FIndex(InX, InY, InZ);
		if(bMainPart)
		{
			const UVoxelData& voxelData = VoxelMap[voxelIndex].GetVoxelData();
			if(voxelData.PartType != EVoxelPartType::Main)
			{
				return GetVoxelItem(voxelIndex - voxelData.PartIndex);
			}
		}
		return VoxelMap[voxelIndex];
	}
	return FVoxelItem::Empty;
}

bool AVoxelChunk::CheckVoxel(FIndex InIndex, const FVoxelItem& InVoxelItem, FVector InRange/* = FVector::OneVector*/)
{
	INDEX_ITERATOR(Iter, InRange, false,
		if(!GetVoxelItem(InIndex + Iter).IsReplaceable(InVoxelItem))
		{
			return true;
		}
	)
	return false;
}

bool AVoxelChunk::CheckVoxelAdjacent(FIndex InIndex, EDirection InDirection)
{
	const FVoxelItem& voxelItem = GetVoxelItem(InIndex);
	const FIndex adjacentIndex = UMathBPLibrary::GetAdjacentIndex(InIndex, InDirection, voxelItem.Angle);
	
	if(!voxelItem.IsValid() || LocalIndexToWorld(adjacentIndex).Z <= 0) return true;
	
	const FVoxelItem& adjacentItem = GetVoxelItem(adjacentIndex);
	if(adjacentItem.IsValid())
	{
		const UVoxelData& voxelData = voxelItem.GetVoxelData();
		const UVoxelData& adjacentData = adjacentItem.GetVoxelData();
		switch(voxelData.Transparency)
		{
			case EVoxelTransparency::Solid:
			{
				switch(adjacentData.Transparency)
				{
					case EVoxelTransparency::Solid:
					{
						return true;
					}
					default: break;
				}
				break;
			}
			case EVoxelTransparency::SemiTransparent:
			{
				switch(adjacentData.Transparency)
				{
					case EVoxelTransparency::SemiTransparent:
					{
						if(voxelData.VoxelType == adjacentData.VoxelType)
						{
							switch(voxelData.VoxelType)
							{
								case EVoxelType::Oak_Leaves:
								case EVoxelType::Birch_Leaves:
								case EVoxelType::Ice:
								case EVoxelType::Glass:
								{
									return true;
								}
								default: break;
							}
						}
						break;
					}
					default: break;
				}
				break;
			}
			case EVoxelTransparency::Transparent:
			{
				switch(adjacentData.Transparency)
				{
					case EVoxelTransparency::Solid:
					case EVoxelTransparency::SemiTransparent:
					{
						return true;
					}
					case EVoxelTransparency::Transparent:
					{
						if(voxelData.VoxelType == adjacentData.VoxelType)
						{
							switch(voxelData.VoxelType)
							{
								case EVoxelType::Water:
								{
									return true;
								}
								default: break;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}
	else if(adjacentItem.IsUnknown())
	{
		return true;
	}
	return false;
}

bool AVoxelChunk::CheckVoxelNeighbors(FIndex InIndex, EVoxelType InVoxelType, FVector InRange, bool bFromCenter, bool bIgnoreBottom)
{
	INDEX_ITERATOR(Iter1, InRange, bFromCenter,
		DIRECTION_ITERATOR(Iter2, 
			if(!bIgnoreBottom || Iter2 != EDirection::Down)
			{
				FVoxelItem& NeighborItem = GetVoxelItem(InIndex + Iter1 + UMathBPLibrary::DirectionToIndex(Iter2));
				if(NeighborItem.IsValid() && NeighborItem.GetVoxelData().VoxelType == InVoxelType)
				{
					return true;
				}
			}
		)
	)
	return false;
}

bool AVoxelChunk::SetVoxelSample(FIndex InIndex, const FVoxelItem& InVoxelItem, bool bGenerate, IVoxelAgentInterface* InAgent)
{
	bool bSuccess = false;
	
	if(InVoxelItem.IsValid())
	{
		if(IsOnTheChunk(InIndex))
		{
			FVoxelItem VoxelItem = FVoxelItem(InVoxelItem);
			VoxelItem.Owner = this;
			VoxelItem.Index = InIndex;
			VoxelMap.Emplace(InIndex, VoxelItem);
			if(bGenerate) VoxelMap[InIndex].OnGenerate(InAgent);
			bSuccess = true;
		}
	}
	else if(HasVoxel(InIndex))
	{
		if(bGenerate) VoxelMap[InIndex].OnDestroy(InAgent);
		VoxelMap.Remove(InIndex);
		bSuccess = true;
	}

	if(bSuccess && bGenerate)
	{
		Generate(true);
		GenerateNeighbors(InIndex);
	}

	return bSuccess;
}

bool AVoxelChunk::SetVoxelSample(int32 InX, int32 InY, int32 InZ, const FVoxelItem& InVoxelItem, bool bGenerate, IVoxelAgentInterface* InAgent)
{
	return SetVoxelSample(FIndex(InX, InY, InZ), InVoxelItem, bGenerate, InAgent);
}

bool AVoxelChunk::SetVoxelComplex(FIndex InIndex, const FVoxelItem& InVoxelItem, bool bGenerate, IVoxelAgentInterface* InAgent)
{
	return SetVoxelComplex(InIndex.X, InIndex.Y, InIndex.Z, InVoxelItem, bGenerate, InAgent);
}

bool AVoxelChunk::SetVoxelComplex(int32 InX, int32 InY, int32 InZ, const FVoxelItem& InVoxelItem, bool bGenerate, IVoxelAgentInterface* InAgent)
{
	if(InX < 0)
	{
		if(GetNeighbor(EDirection::Backward))
		{
			return GetNeighbor(EDirection::Backward)->SetVoxelComplex(InX + AVoxelModule::Get()->GetWorldData().ChunkSize, InY, InZ, InVoxelItem, bGenerate, InAgent);
		}
	}
	else if(InX >= AVoxelModule::Get()->GetWorldData().ChunkSize)
	{
		if(GetNeighbor(EDirection::Forward))
		{
			return GetNeighbor(EDirection::Forward)->SetVoxelComplex(InX - AVoxelModule::Get()->GetWorldData().ChunkSize, InY, InZ, InVoxelItem, bGenerate, InAgent);
		}
	}
	else if(InY < 0)
	{
		if(GetNeighbor(EDirection::Left))
		{
			return GetNeighbor(EDirection::Left)->SetVoxelComplex(InX, InY + AVoxelModule::Get()->GetWorldData().ChunkSize, InZ, InVoxelItem, bGenerate, InAgent);
		}
	}
	else if(InY >= AVoxelModule::Get()->GetWorldData().ChunkSize)
	{
		if(GetNeighbor(EDirection::Right))
		{
			return GetNeighbor(EDirection::Right)->SetVoxelComplex(InX, InY - AVoxelModule::Get()->GetWorldData().ChunkSize, InZ, InVoxelItem, bGenerate, InAgent);
		}
	}
	else if(InZ < 0)
	{
		if(GetNeighbor(EDirection::Down))
		{
			return GetNeighbor(EDirection::Down)->SetVoxelComplex(InX, InY, InZ + AVoxelModule::Get()->GetWorldData().ChunkSize, InVoxelItem, bGenerate, InAgent);
		}
	}
	else if(InZ >= AVoxelModule::Get()->GetWorldData().ChunkSize)
	{
		if(!GetNeighbor(EDirection::Up))
		{
			Module->SpawnChunk(Index + UMathBPLibrary::DirectionToIndex(EDirection::Up), !bGenerate);
		}
		if(GetNeighbor(EDirection::Up))
		{
			return GetNeighbor(EDirection::Up)->SetVoxelComplex(InX, InY, InZ - AVoxelModule::Get()->GetWorldData().ChunkSize, InVoxelItem, bGenerate, InAgent);
		}
	}
	else
	{
		const FIndex index = FIndex(InX, InY, InZ);
		const bool bDestroying = !InVoxelItem.IsValid();
		FVoxelItem voxelItem = !bDestroying ? InVoxelItem : GetVoxelItem(index);
		UVoxelData& voxelData = voxelItem.GetVoxelData(false);
		if(voxelData.IsValid() && voxelData.PartType == EVoxelPartType::Main)
		{
			const FVector range = voxelData.GetRange(voxelItem.Angle);
			if(!CheckVoxel(index, InVoxelItem, range))
			{
				if(bDestroying)
				{
					// Replace with water
					if(CheckVoxelNeighbors(index, EVoxelType::Water, range, false, true))
					{
						voxelItem.ID = UVoxelModuleBPLibrary::VoxelTypeToAssetID(EVoxelType::Water);
					}
					else
					{
						voxelItem.ID = FPrimaryAssetId();
					}
				}
				bool bSuccess = true;
				INDEX_ITERATOR(partIndex, range, false,
					UVoxelData& partData = voxelData.GetPartData(partIndex);
					if(partData.IsValid())
					{
						if(!bDestroying) voxelItem.ID = partData.GetPrimaryAssetId();
						if(partData.PartType == EVoxelPartType::Main)
						{
							if(!SetVoxelSample(index, voxelItem, bGenerate, InAgent))
							{
								bSuccess = false;
							}
						}
						else
						{
							if(!SetVoxelComplex(index + partIndex, voxelItem, bGenerate, InAgent))
							{
								bSuccess = false;
							}
						}
					}
					else
					{
						bSuccess = false;
					}
				)
				return bSuccess;
			}
		}
		else
		{
			return SetVoxelSample(index, InVoxelItem, bGenerate, InAgent);
		}
	}
	return false;
}

void AVoxelChunk::AddSceneActor(AActor* InActor)
{
	if(!InActor || !InActor->Implements<USceneActorInterface>() || ISceneActorInterface::Execute_GetContainer(InActor) == this) return;

	if(ISceneActorInterface::Execute_GetContainer(InActor))
	{
		ISceneActorInterface::Execute_GetContainer(InActor)->RemoveSceneActor(InActor);
	}

	ISceneActorInterface::Execute_SetContainer(InActor, this);

	InActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	if(AAbilityPickUpBase* PickUp = Cast<AAbilityPickUpBase>(InActor))
	{
	 	PickUps.Add(PickUp);
	}
}

void AVoxelChunk::RemoveSceneActor(AActor* InActor)
{
	if(!InActor || !InActor->Implements<USceneActorInterface>() || ISceneActorInterface::Execute_GetContainer(InActor) != this) return;

	ISceneActorInterface::Execute_SetContainer(InActor, nullptr);

	InActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if(AAbilityPickUpBase* PickUp = Cast<AAbilityPickUpBase>(InActor))
	{
		PickUps.Remove(PickUp);
	}
}

AVoxelAuxiliary* AVoxelChunk::SpawnAuxiliary(FIndex InIndex)
{
	const auto& VoxelItem = GetVoxelItem(InIndex);
	if(VoxelItem.IsValid() && !VoxelItem.Auxiliary)
	{
		const auto& VoxelData = VoxelItem.GetVoxelData();
		if(VoxelData.AuxiliaryClass && VoxelData.PartType == EVoxelPartType::Main)
		{
			if(AVoxelAuxiliary* Auxiliary = UObjectPoolModuleBPLibrary::SpawnObject<AVoxelAuxiliary>(nullptr, VoxelData.AuxiliaryClass))
			{
				AddSceneActor(Auxiliary);
				Auxiliary->Initialize(VoxelItem);
				return Auxiliary;
			}
		}
	}
	return nullptr;
}

void AVoxelChunk::DestroyAuxiliary(FIndex InIndex)
{
	const auto& VoxelItem = GetVoxelItem(InIndex);
	if(VoxelItem.Auxiliary)
	{
		UObjectPoolModuleBPLibrary::DespawnObject(VoxelItem.Auxiliary);
	}
}

FVector AVoxelChunk::GetChunkLocation() const
{
	return UVoxelModuleBPLibrary::ChunkIndexToLocation(Index);
}
