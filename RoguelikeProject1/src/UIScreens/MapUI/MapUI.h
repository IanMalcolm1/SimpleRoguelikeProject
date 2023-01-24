#pragma once

#include <memory>
#include <SDL.h>
#include "../../Topography/LocalMap/LocalMap.h"

class MapUI {
private:
	std::unique_ptr<LocalMap> map;

	SDL_Texture* mapTexture;

public:
	void render();
};