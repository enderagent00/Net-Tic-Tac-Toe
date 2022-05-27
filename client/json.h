
#pragma once
#include "json objects.h"

typedef enum {
	JSON_TYPE_STRING,
	JSON_TYPE_UINT32
} JsonDatatype;

xjson* InitJsonWriter(char* buffer, size_t bufferlen);
xjson* InitJsonReader(char* serial, size_t seriallen);
void JsonWrite(xjson* parser, char* key, void* value, JsonDatatype type);
void* JsonRead(xjson* parser, char* key, JsonDatatype type);
void DeallocJsonParser(xjson* parser);

