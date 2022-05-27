
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "networking.h"
#include "debugging.h"
#include "error codes.h"
#include "string.h"
#include "game.h"
#include "json.h"
#define HOST_IP "127.0.0.1"
#define HOST_PORT 1024

char* GetUserInput() {
	printf(">>> ");
	char buffer[255];
	gets_s(buffer, 255);

	return buffer;
}

void SendUserInput(TcpClient* client) {
	char* input = GetUserInput();
	char data[255];
	xjson* parser = InitJsonWriter(data, 255);

	JsonWrite(parser, "header", "user-input", JSON_TYPE_STRING);
	JsonWrite(parser, "value", input, JSON_TYPE_STRING);

	DeallocJsonParser(parser);
	RemoveChars(data, "\n\t\0");
	SendString(client, data);
}

int main(void) {
	InitializeNetworking();
	TcpClient* client = InitTcpClient(HOST_IP, HOST_PORT);

	while (ConnectToServer(client) == CONNECTION_FAILURE) {
		PRINTERROR("Failed to connect to server, retrying...");
		Sleep(1000);
	}

	PRINTLINE("Established TCP connection to server");

	Game* game = InitGame();
	Draw(game);

	while (client->ConnectionState == CONNECTION_STATE_CONNECTED && game->Result == GAME_RESULT_NONE) {
		char* buffer = ReceiveData(client, 1024, 0);
		xjson* parser = InitJsonReader(buffer, strlen(buffer));
		char* header = JsonRead(parser, "header", JSON_TYPE_STRING);

		if (strcmp(header, "player-info") == 0) {
			game->PlayerTile = (int)JsonRead(parser, "tile", JSON_TYPE_UINT32);
			game->PlayerNumber = (int)JsonRead(parser, "number", JSON_TYPE_UINT32);
		}
		else if (strcmp(header, "table") == 0) {
			game->TopLeft = (int)JsonRead(parser, "top-left", JSON_TYPE_UINT32);
			game->TopMiddle = (int)JsonRead(parser, "top-middle", JSON_TYPE_UINT32);
			game->TopRight = (int)JsonRead(parser, "top-right", JSON_TYPE_UINT32);
			game->MiddleLeft = (int)JsonRead(parser, "middle-left", JSON_TYPE_UINT32);
			game->MiddleMiddle = (int)JsonRead(parser, "middle-middle", JSON_TYPE_UINT32);
			game->MiddleRight = (int)JsonRead(parser, "middle-right", JSON_TYPE_UINT32);
			game->BottomLeft = (int)JsonRead(parser, "bottom-left", JSON_TYPE_UINT32);
			game->BottomMiddle = (int)JsonRead(parser, "bottom-middle", JSON_TYPE_UINT32);
			game->BottomRight = (int)JsonRead(parser, "bottom-right", JSON_TYPE_UINT32);
		}
		else if (strcmp(header, "user-input-request") == 0) {
			SendUserInput(client);
			continue;
		}
		else if (strcmp(header, "error") == 0) {
			int errorcode = JsonRead(parser, "error-code", JSON_TYPE_UINT32);
			printf("[-] Error: %s\n", ErrorCodeToString(errorcode));
			continue;
		}
		else if (strcmp(header, "game-end") == 0) {
			game->Result = (int)JsonRead(parser, "result", JSON_TYPE_UINT32);
		}
		
		Draw(game);
		free(buffer);
		DeallocJsonParser(parser);
	}

	if (game->Result == PLAYER_ONE_WON) {
		if (game->PlayerNumber == PLAYER_NUMBER_ONE) {
			PRINTLINE("[+] You have won!");
		}
		else if (game->PlayerNumber == PLAYER_NUMBER_TWO) {
			PRINTLINE("[+] Other player has won!");
		}
	}
	else if (game->Result == PLAYER_TWO_WON) {
		if (game->PlayerNumber == PLAYER_NUMBER_ONE) {
			PRINTLINE("[+] Other player has won!");
		}
		else if (game->PlayerNumber == PLAYER_NUMBER_TWO) {
			PRINTLINE("[+] You have won!");
		}
	}
	else if (game->Result == GAME_RESULT_DRAW) {
		PRINTLINE("[+] Game ended in a draw!");
	}
	else if (game->Result == GAME_RESULT_NONE) {
		PRINTLINE("[-] Lost connection to the server");
	}

	UninitializeNetworking();
	free(client);
	free(game);
	getchar();

	PRINTLINE("Program Exit");
}





