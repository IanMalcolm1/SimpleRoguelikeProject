#pragma once

#include <vector>
#include "../../Topography/LocalMap/LocalMap.h"


//Basically a fraction
class Slope {
private:
	int _num, _denom;

public:
	Slope(int tileDepth = 0, int tileColumn = 0);
	float multiply(int multiplier);
	void manualSlope(int numerator, int denominator);
};

//A row of tiles as encapsulated by start and end slopes (which represent rays)
class Row {
private:
	//offset from origin tile
	int _depth;

	Slope _startSlope, _endSlope;

public:
	Row(int depth, Slope startSlope = Slope(), Slope endSlope = Slope());

	void setStartSlope(int tileColumn);
	void setEndSlope(int tileColumn);

	inline int getDepth();

	void getTilesRange(int& start, int& end);
	Row getNext();

	//Checks to see whether a tile is fully contained between a row's start and end slopes
	//Given that we consider a tile to be a diamond
	bool isSymmetric(int tileColumn);
};


//Used to transform between relative quadrant coords and absolute x,y coords
class Quadrant {
private:
	TileCoordinates origin;
	LocalMap* localMap;
	Actor* actor; //unused when calculating player fov

	//Cardinal represents direction. 0 is north (v), 1 is south (^), 2 is east (<), 3 west (>)
	TileCoordinates quadrantToAbsolute(int cardinal, int tileDepth, int tileColumn);

public:
	Quadrant(TileCoordinates origin, LocalMap* localMap, Actor* actor = nullptr) :
		origin(origin), localMap(localMap), actor(actor) {};

	
	bool isOpaque(int cardinal, int tileDepth, int tileColumn);

	void makeVisibleToPlayer(int cardinal, int tileDepth, int tileColumn);

	void makeVisibleToActor(int cardinal, int tileDepth, int tileColumn);
};

class FoV {
private:
	static void calcFoV(LocalMap* localMap, TileCoordinates origin, Actor* actor, void (Quadrant::*makeVisible)(int, int, int));

public:
	static void calcPlayerFoV(LocalMap* localMap, TileCoordinates playerLocation);
	
	static void calcActorFoV(LocalMap* localMap, Actor* actor);
};