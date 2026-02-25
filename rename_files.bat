@echo off
setlocal enabledelayedexpansion

rem Replace with the path to your folder containing files to rename
set "folder_path=C:/Users/hp/Desktop/esp/junk/death/"

rem Initialize a counter for the new numerical names
set "counter=1"

rem Loop through the files in the folder and rename them
for %%F in ("%folder_path%\*.*") do (
    rem Get the file extension
    set "extension=%%~xF"

    rem Construct the new numerical name with a specified format (e.g., 'file_001.txt')
    set "new_name=file_!counter:~0,3!!extension!"

    rem Rename the file
    ren "%%F" "!new_name!"

    rem Increment the counter
    set /a "counter+=1"
)

echo Files have been renamed in numerical order.
pause
