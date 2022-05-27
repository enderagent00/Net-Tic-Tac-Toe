
#pragma once

typedef enum {
	TILE_TYPE_NAUGHTS,
	TILE_TYPE_CROSSES,
	TILE_TYPE_NONE
} TileType;

typedef struct {
	TileType TopLeft;
	TileType TopMiddle;
	TileType TopRight;
	TileType MiddleLeft;
	TileType MiddleMiddle;
	TileType MiddleRight;
	TileType BottomLeft;
	TileType BottomMiddle;
	TileType BottomRight;
	TileType PlayerTile;
	char* DrawStr;

	enum {
		PLAYER_NUMBER_ONE,
		PLAYER_NUMBER_TWO,
		PLAYER_NUMBER_NONE
	} PlayerNumber;

	enum {
		PLAYER_ONE_WON,
		PLAYER_TWO_WON,
		GAME_RESULT_DRAW,
		GAME_RESULT_NONE
	} Result;
} Game;

Game* InitGame();
void Draw(Game* game);
