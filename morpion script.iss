; Inno Setup Script for Morpion
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Morpion"
#define MyAppVersion "1.5"
#define MyAppPublisher "FromTheRags"
#define MyAppURL "https://github.com/FromTheRags/morpion"
#define MyAppExeName "morpion.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
AppId={{1BED98C0-355F-470E-BE61-D18DE3169719}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=dist
OutputBaseFilename=morpion-setup-v{#MyAppVersion}
SetupIconFile=morpion.ico
Compression=lzma2/max
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "PatrickHand-Regular.ttf"; DestDir: "{app}"; Flags: ignoreversion
Source: "Graduate-Regular.ttf"; DestDir: "{app}"; Flags: ignoreversion
Source: "croix.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "grille.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "nice.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "rond.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "sdl_icone.bmp"; DestDir: "{app}"; Flags: ignoreversion
Source: "morpion.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "morpion.jpg"; DestDir: "{app}"; Flags: ignoreversion
Source: "SDL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "SDL_ttf.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libfreetype-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "LICENSE"; DestDir: "{app}"; Flags: ignoreversion
Source: "OFL.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "README.md"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#MyAppName}}"; Flags: nowait postinstall skipifsilent
