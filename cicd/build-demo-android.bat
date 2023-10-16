@echo OFF

set PROJECT_DIR=%1
set PROJECT_BRANCH=%2
set PLUGIN_BRANCH=%3
set CI_WORK_DIR=%4
set ENGINE_VERSION=%5
set PROJECT_NAME=StoreUeSdkTest
set MAJOR_VERSION=%ENGINE_VERSION:~0,1%

echo.
echo ===========================
echo ### PREPARE TEST PROJECT ##

call %CI_WORK_DIR%\cicd\prepare-test-project.bat %PROJECT_DIR% %PROJECT_BRANCH% %PLUGIN_BRANCH% %CI_WORK_DIR% %ENGINE_VERSION%
if not %errorlevel%==0 goto onFinish

echo.
echo =================================================
echo ### DEFINE VARIABLES FOR PACKAGE TEST PROJECT ###

set AT_PATH="C:\EpicGames\UE_%ENGINE_VERSION%\Engine\Binaries\DotNET\AutomationTool.exe"
if %MAJOR_VERSION%==5 (set AT_PATH="C:\EpicGames\UE_%ENGINE_VERSION%\Engine\Binaries\DotNET\AutomationTool\AutomationTool.exe")

set GLOBAL_ENGINE_INI_PATH="C:\Users\Runner\AppData\Local\Unreal Engine\Engine\Config\UserEngine.ini*"
if %MAJOR_VERSION%==4 (xcopy /Y /S /F "%CI_WORK_DIR%\cicd\UserEngine-4.ini" %GLOBAL_ENGINE_INI_PATH%)
if %MAJOR_VERSION%==5 (xcopy /Y /S /F "%CI_WORK_DIR%\cicd\UserEngine-5.ini" %GLOBAL_ENGINE_INI_PATH%)

set UPROJECT_PATH=%PROJECT_DIR%\%PROJECT_NAME%.uproject
set PACKAGE_ROOT_DIR=%CI_WORK_DIR%\Builds

echo ### AUTOMATION_TOOL_PATH: %AT_PATH%
echo ### UPROJECT_PATH: %UPROJECT_PATH%
echo ### PACKAGE_ROOT_DIR: %PACKAGE_ROOT_DIR%

echo.
echo ===================================
echo ### SWITCH NDK AND SDK API LEVEL ##

set USER_SETTINGS_INI_PATH="C:\Users\Runner\AppData\Local\Unreal Engine\Engine\Config\UserEngine.ini"
echo ### SETTINGS_INI_PATH: %SETTINGS_INI_PATH%

call %CI_WORK_DIR%\cicd\change-ini-settings.bat %USER_SETTINGS_INI_PATH% "/Script/AndroidPlatformEditor.AndroidSDKSettings" SDKAPILevel android-31
call %CI_WORK_DIR%\cicd\change-ini-settings.bat %USER_SETTINGS_INI_PATH% "/Script/AndroidPlatformEditor.AndroidSDKSettings" NDKAPILevel android-21
if not %errorlevel%==0 goto onFinish

echo.
echo =======================================
echo ### PACKAGE IN GAME BROWSER VARIANT ###

set SETTINGS_INI_PATH=%PROJECT_DIR%\Config\DefaultEngine.ini
echo ### SETTINGS_INI_PATH: %SETTINGS_INI_PATH%


call %CI_WORK_DIR%\cicd\change-ini-settings.bat %SETTINGS_INI_PATH% "/Script/XsollaSettings.XsollaProjectSettings" UsePlatformBrowser False
if not %errorlevel%==0 goto onFinish

call %AT_PATH% BuildCookRun -platform=Android -project=%UPROJECT_PATH% -archivedirectory=%PACKAGE_ROOT_DIR%\InGameBrowser -cookflavor=Multi -nop4 -cook -build -stage -prereqss -package -archive 
if not %errorlevel%==0 goto onFinish

echo.
echo ========================================
echo ### PACKAGE PLATFORM BROWSER VARIANT ###

call %CI_WORK_DIR%\cicd\change-ini-settings.bat %SETTINGS_INI_PATH% "/Script/XsollaSettings.XsollaProjectSettings" UsePlatformBrowser True
if not %errorlevel%==0 goto onFinish

call %AT_PATH% BuildCookRun -platform=Android -project=%UPROJECT_PATH% -archivedirectory=%PACKAGE_ROOT_DIR%\PlatformBrowser -cookflavor=Multi -nop4 -cook -build -stage -prereqss -package -archive 
if not %errorlevel%==0 goto onFinish

:onFinish
exit /b %errorlevel%