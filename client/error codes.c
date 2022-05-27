
#include "error codes.h"

const char* ErrorCodeToString(int errorcode) {
	switch (errorcode) {
		case ERROR_CODE_INVALID_INPUT:
			return "Invalid Input";
		case ERROR_CODE_TILE_TAKEN:
			return "Tile Has Already Been Taken";
	}
}
