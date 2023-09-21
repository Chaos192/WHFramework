#include "Voxel/Datas/VoxelWaterData.h"

#include "Voxel/Voxels/VoxelWater.h"

UVoxelWaterData::UVoxelWaterData()
{
	VoxelType = EVoxelType::Water;
	VoxelClass = UVoxelWater::StaticClass();
	Transparency = EVoxelTransparency::Transparent;
}
