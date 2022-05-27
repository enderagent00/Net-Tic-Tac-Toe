
#include <stdlib.h>
#include "game.h"
#include "string.h"
#include "json.h"
#define TOP_LEFT "TL"
#define TOP_MIDDLE "TM"
#define TOP_RIGHT "TR"
#define MIDDLE_LEFT "ML"
#define MIDDLE_MIDDLE "MM"
#define MIDDLE_RIGHT "MR"
#define BOTTOM_LEFT "BL"
#define BOTTOM_MIDDLE "BM"
#define BOTTOM_RIGHT "BR"
#define TRUE 1
#define FALSE 0

static int AllTilesClaimed(Game* game) {
	return (game->TopLeft != TILE_TYPE_NONE && game->TopMiddle != TILE_TYPE_NONE && game->TopRight != TILE_TYPE_NONE &&
		game->MiddleLeft != TILE_TYPE_NONE && game->MiddleMiddle != TILE_TYPE_NONE && game->MiddleRight != TILE_TYPE_NONE &&
		game->BottomLeft != TILE_TYPE_NONE && game->BottomMiddle != TILE_TYPE_NONE && game->BottomRight != TILE_TYPE_NONE);
}

static int DetectWinner(Game* game) {
	TileType tile = game->PlayerTurn->PlayerTile;

	// Detect Horizontal Win

	if (game->TopLeft == tile && game->TopMiddle == tile && game->TopRight == tile) {
		return TRUE;
	}

	if (game->MiddleLeft == tile && game->MiddleMiddle == tile && game->MiddleRight == tile) {
		return TRUE;
	}

	if (game->BottomLeft == tile && game->BottomMiddle == tile && game->BottomRight == tile) {
		return TRUE;
	}

	// Detect Vertical Win

	if (game->TopLeft == tile && game->MiddleLeft == tile && game->BottomLeft == tile) {
		return TRUE;
	}
	if (game->TopMiddle == tile && game->MiddleMiddle == tile && game->BottomMiddle == tile) {
		return TRUE;
	}
	if (game->TopRight == tile && game->MiddleRight == tile && game->BottomRight == tile) {
		return TRUE;
	}

	// Detect Diagonal Win

	if (game->TopLeft == tile && game->MiddleMiddle == tile && game->BottomRight == tile) {
		return TRUE;
	}
	if (game->BottomLeft == tile && game->MiddleMiddle == tile && game->TopRight == tile) {
		return TRUE;
	}

	return FALSE;
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
	game->PlayerTurn = PLAYER_NUMNER_NONE;
	game->State = GAME_STATE_RUNNING;
	game->Result = GAME_RESULT_NONE;

	return game;
}

Player* InitPlayer(TcpClient* client, PlayerNumber number) {
	Player* player = calloc(1, sizeof(Player));
	player->Client = client;
	player->Number = number;

	if (number == PLAYER_NUMBER_ONE) {
		player->PlayerTile = TILE_TYPE_NAUGHTS;
	}
	else if (number == PLAYER_NUMBER_TWO) {
		player->PlayerTile = TILE_TYPE_CROSSES;
	}

	return player;
}

void DeallocGame(Game* game) {
	free(game->PlayerOne);
	free(game->PlayerTwo);
	free(game);
}

void Update(Game* game) {
	if (game->State == GAME_STATE_ENDED) {
		return;
	}

	if (AllTilesClaimed(game)) {
		game->State = GAME_STATE_ENDED;
		game->Result = GAME_RESULT_DRAW;
	}
	else if (DetectWinner(game)) {
		game->State = GAME_STATE_ENDED;
		
		if (game->PlayerTurn->PlayerTile == PLAYER_NUMBER_ONE) {
			game->Result = PLAYER_ONE_WON;
		}
		else if (game->PlayerTurn->PlayerTile == PLAYER_NUMBER_TWO) {
			game->Result = PLAYER_TWO_WON;
		}
	}
}

int SendPlayerInfo(Player* player) {
	char buffer[255];
	xjson* parser = InitJsonWriter(buffer, 255);

	JsonWrite(parser, "header", "player-info", JSON_TYPE_STRING);
	JsonWrite(parser, "tile", (int*)player->PlayerTile, JSON_TYPE_UINT32);
	JsonWrite(parser, "numnber", (int*)player->Number, JSON_TYPE_UINT32);

	DeallocJsonParser(parser);
	RemoveChars(buffer, "\n\t\0");
	return SendString(player->Client, buffer);
}

int SendUserInputRequest(Player* player) {
	char buffer[255];
	xjson* parser = InitJsonWriter(buffer, 255);

	JsonWrite(parser, "header", "user-input-request", JSON_TYPE_STRING);

	DeallocJsonParser(parser);
	RemoveChars(buffer, "\n\t\0");
	return SendString(player->Client, buffer);
}

void SendInvalidInput(Player* player, int errorcode) {
	char buffer[255];
	xjson* parser = InitJsonWriter(buffer, 255);

	JsonWrite(parser, "header", "error", JSON_TYPE_STRING);
	JsonWrite(parser, "error-code", errorcode, JSON_TYPE_UINT32);

	DeallocJsonParser(parser);
	RemoveChars(buffer, "\n\t\0");
	return SendString(player->Client, buffer);
}

void BroadcastTable(Game* game) {
	static char buffer[1024];
	xjson* parser = InitJsonWriter(buffer, 1024);

	JsonWrite(parser, "header", "table", JSON_TYPE_STRING);
	JsonWrite(parser, "top-left", (int*)game->TopLeft, JSON_TYPE_UINT32);
	JsonWrite(parser, "top-middle", (int*)game->TopMiddle, JSON_TYPE_UINT32);
	JsonWrite(parser, "top-right", (int*)game->TopRight, JSON_TYPE_UINT32);
	JsonWrite(parser, "middle-left", (int*)game->MiddleLeft, JSON_TYPE_UINT32);
	JsonWrite(parser, "middle-middle", (int*)game->MiddleMiddle, JSON_TYPE_UINT32);
	JsonWrite(parser, "middle-right", (int*)game->MiddleRight, JSON_TYPE_UINT32);
	JsonWrite(parser, "bottom-left", (int*)game->BottomLeft, JSON_TYPE_UINT32);
	JsonWrite(parser, "bottom-middle", (int*)game->BottomMiddle, JSON_TYPE_UINT32);
	JsonWrite(parser, "bottom-right", (int*)game->BottomRight, JSON_TYPE_UINT32);

	DeallocJsonParser(parser);
	RemoveChars(buffer, "\n\t\0");
	SendString(game->PlayerOne->Client, buffer);
	SendString(game->PlayerTwo->Client, buffer);
}

void BroadcastResult(Game* game) {
	static char buffer[1024];
	xjson* parser = InitJsonWriter(buffer, 1024);

	JsonWrite(parser, "header", "game-end", JSON_TYPE_STRING);
	JsonWrite(parser, "result", (int*)game->Result, JSON_TYPE_UINT32);

	DeallocJsonParser(parser);
	RemoveChars(buffer, "\n\t\0");
	SendString(game->PlayerOne->Client, buffer);
	SendString(game->PlayerTwo->Client, buffer);
}

ValidateInputResult ProcessUserInput(Game* game, char* input) {
	if (strcmp(input, TOP_LEFT) == 0) {
		if (game->TopLeft == TILE_TYPE_NONE) {
			game->TopLeft = game->PlayerTurn->PlayerTile;
			return USER_INPUT_VALID;
		}
		else {
			return USER_INPUT_TILE_TAKEN;
		}
	}
	else if (strcmp(input, TOP_MIDDLE) == 0) {
		if (game->TopMiddle == TILE_TYPE_NONE) {
			game->TopMiddle = game->PlayerTurn->PlayerTile;
			return USER_INPUT_VALID;
		}
		else {
			return USER_INPUT_TILE_TAKEN;
		}
	}
	else if (strcmp(input, TOP_RIGHT) == 0) {
		if (game->TopRight == TILE_TYPE_NONE) {
			game->TopRight = game->PlayerTurn->PlayerTile;
			return USER_INPUT_VALID;
		}
		else {
			return USER_INPUT_TILE_TAKEN;
		}
	}
	else if (strcmp(input, MIDDLE_LEFT) == 0) {
		if (game->MiddleLeft == TILE_TYPE_NONE) {
			game->MiddleLeft = game->PlayerTurn->PlayerTile;
			return USER_INPUT_VALID;
		}
		else {
			return USER_INPUT_TILE_TAKEN;
		}
	}
	else if (strcmp(input, MIDDLE_MIDDLE) == 0) {
		if (game->MiddleMiddle == TILE_TYPE_NONE) {
			game->MiddleMiddle = game->PlayerTurn->PlayerTile;
			return USER_INPUT_VALID;
		}
		else {
			return USER_INPUT_TILE_TAKEN;
		}
	}
	else if (strcmp(input, MIDDLE_RIGHT) == 0) {
		if (game->MiddleRight == TILE_TYPE_NONE) {
			game->MiddleRight = game->PlayerTurn->PlayerTile;
			return USER_INPUT_VALID;
		}
		else {
			return USER_INPUT_TILE_TAKEN;
		}
	}
	else if (strcmp(input, BOTTOM_LEFT) == 0) {
		if (game->BottomLeft == TILE_TYPE_NONE) {
			game->BottomLeft = game->PlayerTurn->PlayerTile;
			return USER_INPUT_VALID;
		}
		else {
			return USER_INPUT_TILE_TAKEN;
		}
	}
	else if (strcmp(input, BOTTOM_MIDDLE) == 0) {
		if (game->BottomMiddle == TILE_TYPE_NONE) {
			game->BottomMiddle = game->PlayerTurn->PlayerTile;
			return USER_INPUT_VALID;
		}
		else {
			return USER_INPUT_TILE_TAKEN;
		}
	}
	else if (strcmp(input, BOTTOM_RIGHT) == 0) {
		if (game->BottomRight == TILE_TYPE_NONE) {
			game->BottomRight = game->PlayerTurn->PlayerTile;
			return USER_INPUT_VALID;
		}
		else {
			return USER_INPUT_TILE_TAKEN;
		}
	}
	else {
		return USER_INPUT_INVALID;
	}
}
