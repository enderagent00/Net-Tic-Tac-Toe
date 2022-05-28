
#pragma once
#include "networking.h"

typedef enum {
	USER_INPUT_VALID,
	USER_INPUT_INVALID,
	USER_INPUT_TILE_TAKEN
} ValidateInputResult;

typedef enum {
	TILE_TYPE_NAUGHTS,
	TILE_TYPE_CROSSES,
	TILE_TYPE_NONE
} TileType;

typedef enum {
	PLAYER_NUMBER_ONE,
	PLAYER_NUMBER_TWO,
	PLAYER_NUMNER_NONE
} PlayerNumber;

typedef struct {
	TcpClient* Client;
	TileType PlayerTile;
	PlayerNumber Number;
} Player;

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

	Player* PlayerOne;
	Player* PlayerTwo;
	Player* PlayerTurn;

	enum {
		GAME_STATE_RUNNING,
		GAME_STATE_ENDED
	} State;

	enum {
		PLAYER_ONE_WON,
		PLAYER_TWO_WON,
		GAME_RESULT_DRAW,
		GAME_RESULT_NONE
	} Result;
} Game;

Game* InitGame();
Player* InitPlayer(TcpClient* client, PlayerNumber number);
void DeallocGame(Game* game);
void Update(Game* game);
int SendPlayerInfo(Player* player);
int SendUserInputRequest(Player* player);
void SendInvalidInput(Player* player, int errorcode);
void BroadcastTable(Game* game);
void BroadcastResult(Game* game);
ValidateInputResult ProcessUserInput(Game* game, char* input);
