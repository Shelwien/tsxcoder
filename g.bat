@echo off

del *.exe

set incs=-DNDEBUG -DSTRICT -DWIN32 -D_WIN32 -D_MSC_VER -D_CRT_DISABLE_PERFCRIT_LOCKS

set opts=-fwhole-program -fstrict-aliasing -fomit-frame-pointer -ffast-math -fpermissive ^
-fno-rtti -fno-stack-protector -fno-stack-check -fno-check-new -fno-exceptions 

rem -flto -ffat-lto-objects -Wl,-flto -fuse-linker-plugin -Wl,-O -Wl,--sort-common -Wl,--as-needed -ffunction-sections

rem -fprofile-use -fprofile-correction 

set gcc=C:\MinGWD21x\bin\g++.exe -march=k8 -mrtm
set path=%gcc%\..\

del *.exe *.o

%gcc% -s -O9 -Ofast %incs% %opts% -static fpaq0mw.cpp -o fpaq0mw.exe

