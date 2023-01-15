#include "Scene.h"
#include "../Algorithms/Terrain Generation/TerrainGenerators.h"


Scene::Scene() {
	map = std::make_shared<LocalMap>(96, 96);
	actorManager = std::make_shared<ActorManager>(map);

	TerrainGenerators::rectangleRooms(map, actorManager, 40, 20);
}

std::shared_ptr<MapDisplay> Scene::getMapDisplay() { return map->getMapDisplay(); }
void Scene::updateMapDisplay() { map->updateMapDisplay(); }


std::shared_ptr<LocalMap> Scene::getMap() {	return map; }


void Scene::processCommand(PlayerCommand command, Uint16 modification) {
	actorManager->processCommand(command, modification);
}


void Scene::setMouseTile(TileCoordinates coordinates) {
	map->setMouseTile(coordinates);
}