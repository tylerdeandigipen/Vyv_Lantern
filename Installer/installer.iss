;; TODO:
;; - Settle on a name for the game that we can redistribute with.

[Setup]
AppName=Name TBD                                                                                   
AppVersion=1.0
WizardStyle=modern
DefaultDirName={autopf}\Name TBD
DefaultGroupName=My Program
UninstallDisplayIcon={app}\GameProject.ico
Compression=lzma2
SolidCompression=yes
OutputDir=.
; "ArchitecturesAllowed=x64" specifies that Setup cannot run on
; anything but x64.
ArchitecturesAllowed=x64
; "ArchitecturesInstallIn64BitMode=x64" requests that the install be
; done in "64-bit mode" on x64, meaning it should use the native
; 64-bit Program Files directory and the 64-bit view of the registry.
ArchitecturesInstallIn64BitMode=x64

[Files]
Source: "../GameFiles/GameProject/x64/Release/GameProject.exe"; DestDir: "{app}"; DestName: "GameProject.exe"
Source: "../GameFiles/GameProject/Fmod/fmod.dll"; DestDir: "{app}"; DestName: "fmod.dll"
Source: "../GameFiles/GameProject/SDL2.dll"; DestDir: "{app}"; DestName: "SDL2.dll"
Source: "../GameFiles/GameProject/Assets/*"; DestDir: "{app}\Assets"; Flags: recursesubdirs
Source: "../GameFiles/GameProject/Data/*"; DestDir: "{app}\Data"; Flags: recursesubdirs

// NOTE(thomas): There are currently stray JSON files lying around in the game's source code directory
// that were likely meant to be placed into the "Data" directory.
Source: "../GameFiles/GameProject/*.json"; DestDir: "{app}";

[Icons]
Name: "{group}\GameProject"; Filename: "../GameProject/GameProject/x64/Release/GameProject.ico"
