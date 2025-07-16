@echo off
.\masm32\bin\ml.exe /c /coff /Fl /Sn ./app.asm
.\masm32\bin\link.exe /SUBSYSTEM:CONSOLE /LIBPATH:./masm32/lib/ app.obj
cls
app.exe