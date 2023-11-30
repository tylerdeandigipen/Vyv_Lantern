;; TODO:
;; - Settle on a name for the game that we can redistribute with.

[Setup]
AppName=Refraction                                                                                   
AppVersion=1.0
WizardStyle=modern
DefaultDirName={autopf}\DigiPen\Refraction
DefaultGroupName=Refraction
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
Source: "../GameFiles/GameProject/x64/Release/GameProject.exe"; DestDir: "{app}"; DestName: "Refraction.exe"
Source: "../GameFiles/GameProject/Fmod/fmodL.dll"; DestDir: "{app}"; DestName: "fmodL.dll"
Source: "../GameFiles/GameProject/SDL2.dll"; DestDir: "{app}"; DestName: "SDL2.dll"
Source: "../GameFiles/GameProject/SDL2_ttf/SDL2_ttf.dll"; DestDir: "{app}"; DestName: "SDL2_ttf.dll"
Source: "../GameFiles/GameProject/Assets/*"; DestDir: "{app}\Assets"; Flags: recursesubdirs
Source: "../GameFiles/GameProject/Data/*"; DestDir: "{app}\Data"; Flags: recursesubdirs

[Icons]
Name: "{group}\GameProject"; Filename: "../GameProject/GameProject/x64/Release/GameProject.ico"
