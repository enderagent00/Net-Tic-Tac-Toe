
#define XJSON_H_IMPLEMENTATION
#include <stdlib.h>
#include <xjson/json.h>
#include "json.h"

/*
	String Allocator required by the json library
*/
static char* StringAllocator(const char* str, size_t strlen, void* memctx) {
	char* output = calloc(strlen + 1, sizeof(char));
	memcpy(output, str, strlen);
	output[strlen] = '\0';

	return output;
}

/*
	Initialize a json parser on the heap
*/
static xjson* InitJsonParser() {
	xjson* parser = calloc(1, sizeof(xjson));
	memset(parser, 0, sizeof(xjson));
	xjson_set_string_allocator(parser, StringAllocator);

	return parser;
}

/*
	Initialize a json parser in write mode
*/
xjson* InitJsonWriter(char* buffer, size_t bufferlen) {
	xjson* parser = InitJsonParser();
	xjson_setup_write(parser, true, buffer, bufferlen);
	xjson_object_begin(parser, NULL);

	return parser;
}

/*
	Initialize a json parser in read mode
*/
xjson* InitJsonReader(char* serial, size_t seriallen) {
	xjson* parser = InitJsonParser();
	xjson_setup_read(parser, serial, seriallen);
	xjson_object_begin(parser, NULL);

	return parser;
}

/*
	Writes to a json parser
*/
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

/*
	Reads from a json parser
*/
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

/*
	Fully deallocates the dynamic memory used by a json parser
*/
void DeallocJsonParser(xjson* parser) {
	xjson_object_end(parser);
	free(parser);
}

