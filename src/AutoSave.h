#ifndef AUTOSAVE_H
#define AUTOSAVE_H

#include <Windows.h>

const int MAX_WORD_LEN = 256;

struct Config {
    TCHAR host[256];
    int port;
    TCHAR path[1024];
    int numKeywords;
    char** keywords;
};

struct Config getConfig(char *);

int matchWords(char *, struct Config);
int postContent(char *, struct Config);

#endif //AUTOSAVE_H