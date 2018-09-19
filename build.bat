@echo off

if not exist bin mkdir bin
pushd bin

set cmp=/FC /Zi /MDd /EHsc /Ob0 /Oy- /Fe%project%
set inc=-I..\inc
set lnk=/SUBSYSTEM:WINDOWS /DEBUG ..\lib\*.lib opengl32.lib /NODEFAULTLIB:MSVCRT
set clang_wno=-Wno-unknown-pragmas -Wno-deprecated-declarations -Wno-macro-redefined

if "%1" == "clang" (
  echo clang-cl main.cpp -^> %project%.exe
  echo.
  clang-cl %cmp% -Wall %clang_wno% ../main.cpp %inc% /link %lnk%
) else (
  cl %cmp% ../main.cpp %inc% /link %lnk%
)

popd
