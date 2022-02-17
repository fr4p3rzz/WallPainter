clang.exe -o WallPainter.exe -I .\SDL2-2.0.20\include -L .\SDL2-2.0.20\lib\x64 .\main.c .\src\*.c -Xlinker /subsystem:console -lSDL2main -lSDL2 -lshell32 -lws2_32 && WallPainter.exe
