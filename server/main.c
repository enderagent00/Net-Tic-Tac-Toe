
#include <stdio.h>
#include <Windows.h>
#include "networking.h"
#include "debugging.h"
#include "error codes.h"
#include "json.h"
#include "game.h"
#define HOST_IP "127.0.0.1"
#define HOST_PORT 1024
#define MAX_CLIENTS 2

char* ReceiveUserInput(Game* game) {
	char* buffer = ReceiveData(game->PlayerTurn->Client, 1024, 0);
	xjson* parser = InitJsonReader(buffer, strlen(buffer));
	char* header = JsonRead(parser, "header", JSON_TYPE_STRING);
	char* input = JsonRead(parser, "value", JSON_TYPE_STRING);

	DeallocJsonParser(parser);
	free(buffer);
	return input;
}

void ConnectionListener(TcpServer* server, Game* game, PlayerNumber playerno) {
	TcpClient* client = AcceptConnection(server);
	Player* player = InitPlayer(client, playerno);
	
	if (playerno == PLAYER_NUMBER_ONE) {
		game->PlayerOne = player;
	}
	else if (playerno == PLAYER_NUMBER_TWO) {
		game->PlayerTwo = player;
	}

	SendPlayerInfo(player);
}

int main(void) {
	InitializeNetworking();

	TcpServer* server = InitTcpServer(HOST_IP, HOST_PORT, MAX_CLIENTS);
	Game* game = InitGame();

	PRINTLINE("Waiting for player one to connect");
	ConnectionListener(server, game, PLAYER_NUMBER_ONE);
	PRINTLINE("Player one has connected");

	PRINTLINE("Waiting for player two to connect");
	ConnectionListener(server, game, PLAYER_NUMBER_TWO);
	PRINTLINE("Player two has connected");
	
	BroadcastTable(game);
	game->PlayerTurn = game->PlayerOne;

	while (game->State == GAME_STATE_RUNNING) {
		SendUserInputRequest(game->PlayerTurn);
		char* input = ReceiveUserInput(game);
		ValidateInputResult result = ProcessUserInput(game, input);

		if (result == USER_INPUT_VALID) {
			/*
				The input is valid and is being updated and broadcasted to the clients
			*/

			Update(game);
			BroadcastTable(game);

			if (game->PlayerTurn == game->PlayerOne) {
				game->PlayerTurn = game->PlayerTwo;
			}
			else if (game->PlayerTurn == game->PlayerTwo) {
				game->PlayerTurn = game->PlayerOne;
			}

			if (game->Result != GAME_RESULT_NONE) {
				BroadcastResult(game);
			}
		}
		else if (result == USER_INPUT_TILE_TAKEN) {
			/*
				The tile has already been taken in this game
			*/

			SendInvalidInput(game->PlayerTurn, ERROR_CODE_TILE_TAKEN);
		}
		else if (result == USER_INPUT_INVALID) {
			/*
				The input is completely invalid
			*/

			SendInvalidInput(game->PlayerTurn, ERROR_CODE_INVALID_INPUT);
		}
	}

	Sleep(2000);
	UninitializeNetworking();
	DeallocGame(game);
	free(server);

	PRINTLINE("Program Exit");
}





