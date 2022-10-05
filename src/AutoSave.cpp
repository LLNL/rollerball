#include "AutoSave.h"
#include <Windows.h>
#include <WinHttp.h>
#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "winhttp.lib")

struct Config getConfig(char * filename)
{
    struct Config settings;

    wchar_t wpath[1024];
    mbstowcs(wpath, filename, strlen(filename) + 1);
    LPWSTR wpath_ptr = wpath;

    GetPrivateProfileString(TEXT("server"), TEXT("host"), TEXT("127.0.0.1"), settings.host, sizeof(settings.host) / sizeof(settings.host[0]), wpath_ptr);
    GetPrivateProfileString(TEXT("server"), TEXT("path"), TEXT("/"), settings.path, sizeof(settings.path) / sizeof(settings.path[0]), wpath_ptr);
    settings.port = GetPrivateProfileInt(TEXT("server"), TEXT("port"), 8000, wpath_ptr);

    char keywordList[4096];
    GetPrivateProfileStringA("filter", "keywords", NULL, keywordList, sizeof(keywordList) / sizeof(keywordList[0]), filename);

    if (keywordList)
    {
        int i, count;
        for (i = 0, count = 0; keywordList[i]; i++)
            count += (keywordList[i] == ';');

        settings.numKeywords = count + 1;

        settings.keywords = new char * [settings.numKeywords];
        char* token = strtok(keywordList, ";");
        for (i = 0; i < settings.numKeywords; i++)
        {
            settings.keywords[i] = new char[MAX_WORD_LEN];
            strncpy(settings.keywords[i], token, MAX_WORD_LEN);
            token = strtok(NULL, ";");
        }
    }
    else
    {
        settings.numKeywords = 0;
        settings.keywords = NULL;
    }
    return settings;
}

int matchWords(char* content, struct Config settings)
{
    for (int i = 0; i < settings.numKeywords; i++)
    {
        if (strstr(content, settings.keywords[i]) != NULL)
            return 1;
    }
    return 0;
}

int postContent(char *content, struct Config settings)
{
    BOOL  bResults = FALSE;
    HINTERNET hSession = NULL,
    hConnect = NULL,
    hRequest = NULL;
    
    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(L"Notepad++ AutoSave Plugin/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);

    // Specify an HTTP server.
    if (hSession)
        hConnect = WinHttpConnect(hSession, settings.host,
            (INTERNET_PORT)settings.port, 0);

    // Create an HTTP Request handle.
    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, L"POST",
            settings.path,
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            0);

    // Send a Request.
    if (hRequest)
    {
        LPCWSTR headers = L"content-type:application/x-www-form-urlencoded";
        bResults = WinHttpSendRequest(hRequest,
            headers,
            (DWORD)wcslen(headers),
            (LPVOID)content,
            (DWORD)strlen(content),
            (DWORD)strlen(content),
            NULL);
    }

    // Report any errors.
    if (!bResults)
    {
        printf("Error %d has occurred.\n", GetLastError());
        return 0;
    }

    // Close any open handles.
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    return 1;
}