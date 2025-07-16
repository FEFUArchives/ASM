.386
.model flat,stdcall
option casemap:none

; libraries
includelib .\masm32\lib\kernel32.lib
include .\masm32\include\kernel32.inc
include .\masm32\include\windows.inc

; structure
COORDS struc
    X word  ?
    Y word  ?
COORDS ends

.data
saveFlags   dword ?
InHandle    dword ?
buffer1    dword ?
bytesRead    dword ?

OutHandle dword ?
cellsWritten dword ?
pos COORDS <0,2> ; cords for uncolorized text
posWC COORDS <0,3> ; cords for colorized text
buffer byte 20 dup (0) ; buf for input
BufSize =($-buffer)

; color codes
attributes word  4, 12, 10, 14, 9, 13, 11, 15, 6, 5, 3, 7, 8, 2, 1, 0Eh, 0Fh, 0Ah, 0Bh, 0101101010101010b
.code 
main  PROC
push -10
    call GetStdHandle
    mov InHandle, EAX

push -11
    call GetStdHandle
    mov OutHandle, EAX

; read from keyboard
invoke ReadConsole,
    InHandle,
    ADDR buffer,
    BufSize,
    ADDR bytesRead,
    0

; colorize text
mov eax, offset attributes
push eax
push pos
push bytesRead
lea eax, attributes
push eax
push OutHandle
call WriteConsoleOutputAttribute

; print colored text
lea eax, cellsWritten
push eax
push pos
push bytesRead
lea eax, buffer
push eax
push OutHandle
call WriteConsoleOutputCharacter

; print uncolored text
lea eax, cellsWritten
push eax
push posWC
push bytesRead
lea eax, buffer
push eax
push OutHandle
call WriteConsoleOutputCharacter


push 6000
call Sleep

invoke CloseHandle , OutHandle
invoke ExitProcess,0
main ENDP
END main

