To create a shared library (dll) on Windows and use it in a C program do the following:

crystal build logger.cr --single-module --emit obj --verbose

mv logger.obj liblogger.obj

Copy command from the verbose output and remove /ENTRY:wmainCRTStartup ; add /LD and /DEF:liblogger.def ; remove the default output name also.

"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.36.32532\bin\Hostx64\x64\cl.exe" /nologo /LD liblogger.obj /link "/LIBPATH:C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.36.32532\atlmfc\lib\x64" "/LIBPATH:C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.36.32532\lib\x64" "/LIBPATH:C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\ucrt\x64" "/LIBPATH:C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64" /DEBUG:FULL /PDBALTPATH:%_PDB% /INCREMENTAL:NO /STACK:0x800000 /LIBPATH:C:\Users\jools\scoop\apps\crystal\current\lib pcre2-8.lib gc.lib  libcmt.lib iconv.lib advapi32.lib Kernel32.lib shell32.lib ole32.lib WS2_32.lib kernel32.lib legacy_stdio_definitions.lib DbgHelp.lib libucrt.lib /DEF:liblogger.def

cl main.c /link liblogger.lib

Files needed:

C:\Users\jools\tmp\crystal>more logger.cr
fun init = crystal_init : Void

GC.init

LibCrystalMain.__crystal_main(0, Pointer(Pointer(UInt8)).null)
end

fun log = crystal_log(text: UInt8*): Void
puts String.new(text)
end

C:\Users\jools\tmp\crystal>more logger.h
#ifndef _CRYSTAL_LOGGER_H
#define _CRYSTAL_LOGGER_H

void crystal_init(void);
void crystal_log(char* text);
#endif

C:\Users\jools\tmp\crystal>more main.c
#include “logger.h”

int main(void) {
crystal_init();
crystal_log(“Hello world!”);
}

To create the def file, can use nm liblogger.obj | grep crystal_ in WSL

The file should be like this:

LIBRARY liblogger
EXPORTS
  __crystal_main
  __crystal_malloc
  __crystal_malloc64
  __crystal_malloc_atomic
  __crystal_malloc_atomic64
  __crystal_once
  __crystal_once_init
  __crystal_raise
  __crystal_raise_overflow
  __crystal_raise_string
  __crystal_realloc
  __crystal_realloc64
  crystal_init
  crystal_log

Can use dumpbin /EXPORTS *.dll to see the dll exports are there too.
