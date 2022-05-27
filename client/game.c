
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#define NAUGHT "0"
#define CROSS "X"
#define EMPTY " "

static const char* TileTypeToString(TileType type) {
	switch (type) {
		case TILE_TYPE_NAUGHTS:
			return NAUGHT;
		case TILE_TYPE_CROSSES:
			return CROSS;
		case TILE_TYPE_NONE:
			return EMPTY;
	}
}

Game* InitGame() {
	Game* game = calloc(1, sizeof(Game));
	game->TopLeft = TILE_TYPE_NONE;
	game->TopMiddle = TILE_TYPE_NONE;
	game->TopRight = TILE_TYPE_NONE;
	game->MiddleLeft = TILE_TYPE_NONE;
	game->MiddleMiddle = TILE_TYPE_NONE;
	game->MiddleRight = TILE_TYPE_NONE;
	game->BottomLeft = TILE_TYPE_NONE;
	game->BottomMiddle = TILE_TYPE_NONE;
	game->BottomRight = TILE_TYPE_NONE;
	game->PlayerTile = TILE_TYPE_NONE;
	game->PlayerNumber = PLAYER_NUMBER_NONE;
	game->Result = GAME_RESULT_NONE;

	return game;
}

void Draw(Game* game) {
	const char* format = "\n\
\t      |      |\n\
\t  %s   |  %s   |   %s\n\
\t      |      |\n\
\t--------------------\n\
\t      |      |\n\
\t  %s   |  %s   |   %s\n\
\t      |      |\n\
\t--------------------\n\
\t      |      |\n\
\t  %s   |  %s   |  %s\n\
\t      |      |\n\n\n";

	char* topLeft = TileTypeToString(game->TopLeft);
	char* topMiddle = TileTypeToString(game->TopMiddle);
	char* topRight = TileTypeToString(game->TopRight);
	char* middleLeft = TileTypeToString(game->MiddleLeft);
	char* middleMiddle = TileTypeToString(game->MiddleMiddle);
	char* middleRight = TileTypeToString(game->MiddleRight);
	char* bottomLeft = TileTypeToString(game->BottomLeft);
	char* bottomMiddle = TileTypeToString(game->BottomMiddle);
	char* bottomRight = TileTypeToString(game->BottomRight);

	char output[1024];

	sprintf_s(output, 1024, format,
		topLeft, topMiddle, topRight,
		middleLeft, middleMiddle, middleRight,
		bottomLeft, bottomMiddle, bottomRight
	);

	system("cls");
	printf(output);

	if (game->DrawStr != NULL) {
		printf("%s\n", game->DrawStr);
		game->DrawStr = NULL;
	}
}


