: # Bilingual script that works on Windows and Unix
:; make $@; exit $?
@if exist "%ANDROID_NDK%\prebuilt\windows-x86_64\bin\make.exe" ("%ANDROID_NDK%\prebuilt\windows-x86_64\bin\make.exe" %*) else (make.exe %*)
@exit /b %ERRORLEVEL%
