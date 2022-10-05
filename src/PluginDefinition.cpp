//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"
#include "AutoSave.h"

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

// AutoSave Plugin settings
struct Config settings;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE /*hModule*/)
{
    settings = getConfig("plugins/AutoSave/AutoSave.ini");
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
    // Cleanup settings memory.
    for (int i = 0; i < settings.numKeywords; i++)
    {
        delete[] settings.keywords[i];
    }
    delete[] settings.keywords;
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("Enable"), enableAutoSave, NULL, false);
    setCommand(1, TEXT("Disable"), disableAutoSave, NULL, false);
    postContent("AutoSave Plugin Loaded\r\n", settings);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void enableAutoSave()
{
    // Get the Current File Handle
    int currentEdit;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
    HWND curScint = (currentEdit == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

    // Get the length of the file
    int contentLen = (int)::SendMessage(curScint, SCI_GETLENGTH, 0, NULL);
    
    // Create a buffer and copy in the content
    char * content = new char[contentLen + 2];
    memset(content, 0, contentLen + 2);
    ::SendMessage(curScint, SCI_GETTEXT, contentLen + 1, (LPARAM)content);

    // Check filter
    if (matchWords(content, settings))
    {
        // Post the content to the webserver
        int success = postContent(content, settings);

        // Display a dialog box
        if (success)
            ::MessageBox(NULL, TEXT("AutoSave Enabled"), TEXT("Jackpot"), MB_OK);
    }
    else
    {
        ::MessageBox(NULL, TEXT("AutoSave Enabled"), TEXT("Nothing"), MB_OK);
    }

    // Cleanup
    delete[] content;
}

void disableAutoSave()
{
    // Display a dialog box
    ::MessageBox(NULL, TEXT("AutoSave Disabled"), TEXT("AutoSave Disabled"), MB_OK);
}
