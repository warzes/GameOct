#pragma once

enum class NoiseType : uint8_t
{
	OpenSimplex2,
	OpenSimplex2S,
	Cellular,
	Perlin,
	ValueCubic,
	Value
};

enum class FractalType : uint8_t
{
	None,
	FBM,
	Ridged,
	PingPong
};

struct HeightmapDesc final
{
	uint32_t width;
	uint32_t depth;
	uint32_t max_height;
	FractalType fractal_type = FractalType::None;
	NoiseType noise_type = NoiseType::Perlin;
	int seed = 1337;
	float persistence = 0.5f;
	float lacunarity = 2.0f;
	int octaves = 3;
	float noise_scale = 10;
};

class Heightmap final
{
public:
	explicit Heightmap(const HeightmapDesc& desc);

	float HeightAt(uint64_t x, uint64_t z);
	uint64_t Width() const;
	uint64_t Depth() const;

private:
	std::vector<std::vector<float>> heightmap;
};