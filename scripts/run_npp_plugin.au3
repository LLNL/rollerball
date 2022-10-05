#include <GuiMenu.au3>

Run("C:\\Program Files\Notepad++\notepad++.exe")
WinWaitActive("[CLASS:Notepad++]")
$hWnd = WinGetHandle("[CLASS:Notepad++]")
While 1
   WinMenuSelectItem($hWnd, "", "&Plugins", "AutoSave", "Enable")
   WinWaitActive("[CLASS:#32770]")
   ControlClick("[CLASS:#32770]", "OK", 2, "left", 1)
   Sleep(10000)
WEnd
