 ## MemoryGameC (BattleShip):
- It's a game that make use of your memory.
- In this battle ship game there is 3 level of difficulty:
   * Normal:
      - You have a map wich indicate missed and hitted target
      - You have a message when you hitted or not
      - You have a message when you sinked a ship
   * Hard:
      - You don't have a map
      - You have a message when you hitted or not
      - You have a message when you sinked a ship
   * Hardcore:
      - You don't have a map
      - You have a message when you hitted or not
      - You have don't a message when you sinked a ship
- The AI difficulty grow stronger based on the level of difficulty

## Setup:
 * At the moment, it doesn't seems to be necesary to follow this setup. Normal environment should work.
 * I. [Install MSYS2](https://www.msys2.org/)
    * Launch 'MSYS2 MINGW64'
    * Run the following command:
        - `pacman -S mingw-w64-x86_64-gtk4` Enter 'Y' and wait
        - `pacman -S mingw-w64-x86_64-toolchain base-devel` Press 'ENTER' (default) and 'Y' and wait

    * Once that setup, Edit the systeme environment variable and click on 'Environment Variables'
    In the 'User variables for *' search 'path' and click 'edit', now click 'new' and copy the path where you installed MSYS
    By default: `C:\msys64\mingw64\bin`, press 'OK' and leave 
 
 * II. In 'Visual Studio 2022': if asked to configure git
    - Install [Git](https://git-scm.com/downloads)
    - Open 'Git Bash' and run `git config --global user.name "Username"` `git config --global user.email mail@example.com`



## Build:
 * I. In 'MSYS2 MINGW64' run the folowing command 
   -  (!!You must store the project file inside the dir!!) (Ex: C:\msys64\home\user)
   -  [Ex: C:\msys64\home\user]
        ```
        gcc main.c -o main `pkg-config --cflags --libs gtk4`
        ```

 * II. Using terminal:
   -  Run `pkg-config --libs -cflags gtk4` in 'MSYS2 MINGW64':
   - Create a `build.bat` with
      ```
      @echo off
      set GTK_I=[output from the previous command]

      gcc [name].c -o main %GTK_I%
      pause
      ```
   - Save the file. 
   - When compiling run `.\build.bat`

## Known Issue:
 * The colors in the map doesn't display on some terminal (display character that hide the content)
   - Windows fix: run  ```REG ADD HKCU\CONSOLE /f /v VirtualTerminalLevel /t REG_DWORD /d 1``` in CMD as Admin
