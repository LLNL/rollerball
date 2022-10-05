# Rollerball Notepad++ Plugin

This repo contains code for a benignware (pseudo malware) plugin for the Notepad++ editor based on this [plugin template](https://github.com/npp-plugins/plugintemplate)

The plugin purports to be an AutoSave feature, but in fact it exfiltrates file content based on keywords to a webserver that you specify.  This code was designed as a way to test detection capabilities.

## Getting Started

### Prerequisites

Ensure that Notepad++ is installed in your environment.  This plugin was tested with the amd64 version of [Notepad++ v8.2.1](https://github.com/notepad-plus-plus/notepad-plus-plus/releases/download/v8.2.1/npp.8.2.1.Installer.x64.exe).

### Installation

1. Locate the plugins directory.  For me this was `C:\Program Files\Notepad++\plugins`
2. Create a new directory in the plugin directory called AutoSave
3. Copy the [AutoSave.dll](bin/AutoSave.dll) and [AutoSave.ini](bin/AutoSave.ini) file into this new directory.

### Configuration

Edit the host, port, path, and keywords values in the AutoSave.ini file as needed.


## Release

Rollerball is released under a GPL-2.0 license. See [LICENSE](LICENSE) for details.

LLNL-CODE-840298
