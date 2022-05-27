
#define XJSON_H_IMPLEMENTATION
#include <stdlib.h>
#include <xjson/json.h>
#include "json.h"

static char* StringAllocator(const char* str, size_t strlen, void* memctx) {
	char* output = calloc(strlen + 1, sizeof(char));
	memcpy(output, str, strlen);
	output[strlen] = '\0';

	return output;
}

static xjson* InitJsonParser() {
	xjson* parser = calloc(1, sizeof(xjson));
	memset(parser, 0, sizeof(xjson));
	xjson_set_string_allocator(parser, StringAllocator);

	return parser;
}

xjson* InitJsonWriter(char* buffer, size_t bufferlen) {
	xjson* parser = InitJsonParser();
	xjson_setup_write(parser, true, buffer, bufferlen);
	xjson_object_begin(parser, NULL);

	return parser;
}

xjson* InitJsonReader(char* serial, size_t seriallen) {
	xjson* parser = InitJsonParser();
	xjson_setup_read(parser, serial, seriallen);
	xjson_object_begin(parser, NULL);

	return parser;
}

void JsonWrite(xjson* parser, char* key, void* value, JsonDatatype type) {
	switch (type) {
		case JSON_TYPE_STRING:
			xjson_string(parser, key, &(const char*)value);
			break;
		case JSON_TYPE_UINT32:
			xjson_u32(parser, key, &(uint32_t*)value);
			break;
	}
}

void* JsonRead(xjson* parser, char* key, JsonDatatype type) {
	char* buffer = "";
	xjson_key(parser, &key);

	switch (type) {
		case JSON_TYPE_STRING:
			xjson_string(parser, NULL, &buffer);
			break;
		case JSON_TYPE_UINT32:
			xjson_u32(parser, NULL, &buffer);
			break;
	}

	return buffer;
}

void DeallocJsonParser(xjson* parser) {
	xjson_object_end(parser);
	free(parser);
}

