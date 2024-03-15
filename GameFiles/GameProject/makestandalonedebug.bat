set OutDir=standaloneDEBUG
set BinDir=Debug
if exist %OutDir% rmdir /s /q %OutDir%
mkdir %OutDir%
xcopy .\x64\%BinDir%\*.exe %OutDir% /s /r /y /q /i
xcopy ".\x64\%BinDir%\*.dll" %OutDir% /s /r /y /q /i
xcopy .\Data\ %OutDir%\Data /s /r /y /q /i
xcopy .\Assets\ %OutDir%\Assets /s /r /y /q /i
xcopy .\Font\ %OutDir%\Font /s /r /y /q /i

pause