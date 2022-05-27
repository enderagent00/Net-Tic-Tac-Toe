
#include <stdlib.h>
#include <string.h>

void RemoveChar(char* string, char chr) {
    int j, n = strlen(string);

    for (int i = j = 0; i < n; i++) {
        if (string[i] != chr) {
            string[j++] = string[i];
        }
    }

    string[j] = '\0';
}

void RemoveChars(char* string, char* chrs) {
    size_t chrlen = strlen(chrs);

    for (int i = 0; i < chrlen; i++) {
        RemoveChar(string, chrs[i]);
    }
}


