
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <inttypes.h> // for PRIi64 macros

typedef enum xjson_state
{
    XJSON_STATE_UNITIALIZED = 0,
    XJSON_STATE_READ,
    XJSON_STATE_WRITE
} xjson_state;

typedef struct xjson
{
    // Will be passed to the string allocator function
    void* mem_ctx;
    // Current state of this xjson object. Is XJSON_STATE_UNITIALIZED by default
    xjson_state mode;
    // Will output json with newline/tab.
    bool pretty_print;
    int intendation;

    // These point to the beginning/end and current location in either the write or read buffer
    uint8_t* current;
    uint8_t* end;
    uint8_t* start;

    // The custom string allocator function
    char* (*string_allocator)(const char* str, size_t size, void* mem_ctx);

    // Error handling. Set to true on error + appropriate message in error_message.
    bool error;
    char error_message[256];
} xjson;
