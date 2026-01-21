#include "stdafx.h"
#include "Heightmap.h"
#include "FastNoiseLite.h"
//=============================================================================
constexpr FastNoiseLite::NoiseType GetNoiseType(NoiseType type)
{
	switch (type)
	{
	case NoiseType::OpenSimplex2:
		return FastNoiseLite::NoiseType_OpenSimplex2;
	case NoiseType::OpenSimplex2S:
		return FastNoiseLite::NoiseType_OpenSimplex2S;
	case NoiseType::Cellular:
		return FastNoiseLite::NoiseType_OpenSimplex2;
	case NoiseType::ValueCubic:
		return FastNoiseLite::NoiseType_ValueCubic;
	case NoiseType::Value:
		return FastNoiseLite::NoiseType_Value;
	case NoiseType::Perlin:
	default:
		return FastNoiseLite::NoiseType_Perlin;
	}

	return FastNoiseLite::NoiseType_Perlin;
}
//=============================================================================
constexpr FastNoiseLite::FractalType GetFractalType(FractalType type)
{
	switch (type)
	{
	case FractalType::FBM:
		return FastNoiseLite::FractalType_FBm;
	case FractalType::Ridged:
		return FastNoiseLite::FractalType_Ridged;
	case FractalType::PingPong:
		return FastNoiseLite::FractalType_PingPong;
	case FractalType::None:
	default:
		return FastNoiseLite::FractalType_None;
	}

	return FastNoiseLite::FractalType_None;
}
//=============================================================================
Heightmap::Heightmap(const HeightmapDesc& desc)
{
	FastNoiseLite noise{};
	noise.SetFractalType(GetFractalType(desc.fractal_type));
	noise.SetSeed(desc.seed);
	noise.SetNoiseType(GetNoiseType(desc.noise_type));
	noise.SetFractalOctaves(desc.octaves);
	noise.SetFractalLacunarity(desc.lacunarity);
	noise.SetFractalGain(desc.persistence);
	noise.SetFrequency(0.1f);
	heightmap.resize(desc.depth);

	for (uint32_t z = 0; z < desc.depth; z++)
	{
		heightmap[z].resize(desc.width);
		for (uint32_t x = 0; x < desc.width; x++)
		{

			float xf = x * desc.noise_scale / desc.width;
			float zf = z * desc.noise_scale / desc.depth;

			float total = noise.GetNoise(xf, zf);

			heightmap[z][x] = total * desc.max_height;
		}
	}
}
//=============================================================================
float Heightmap::HeightAt(uint64_t x, uint64_t z)
{
	return heightmap[z][x];
}
//=============================================================================
uint64_t Heightmap::Width() const
{
	return heightmap[0].size();
}
//=============================================================================
uint64_t Heightmap::Depth() const
{
	return heightmap.size();
}
//=============================================================================