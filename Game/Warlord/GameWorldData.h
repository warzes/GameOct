#pragma once

class Camera;
struct GameModel;

struct GameWorldData final
{
	void Init()
	{
		gameModels.reserve(10000);
	}

	void ResetFrame()
	{
		camera = nullptr;
		countGameModels = 0;
	}

	void Bind(GameModel* go)
	{
		if (countGameModels >= gameModels.size())
			gameModels.push_back(go);
		else
			gameModels[countGameModels] = go;

		countGameModels++;
	}

	Camera* camera{ nullptr };

	std::vector<GameModel*> gameModels;
	size_t                  countGameModels{ 0 };
};