#include <stdio.h>
#include <string.h>

int main() {
    char source[64] = "mmmmmm nnnnnn oooo pppp qqq rr";
    char output[256] = "";
    static char freq[256];

    _asm {
        lea esi, source; ESI points to the start of the source string
        lea edi, output; EDI points to the start of the output string

        Start :
        ; Skip leading spaces using SCASB
            cld; Clear direction flag(increments EDI)
            mov al, ' '; Character to compare(space)
            push edi; Save EDI(output pointer)
            mov ebx, esi; Use EBX as temporary pointer for scanning
            mov ecx, 0xFFFFFFFF; Set ECX to maximum value

            SkipSpaces :
        ; Set EDI to EBX for scanning
            mov edi, ebx
            repne scasb; Scan for non - space character
            ; After SCASB, EDI points to the first non - space character
            mov esi, edi; ESI now points to the start of the word
            pop edi; Restore EDI(output pointer)

            ; Check for end of string after skipping spaces
            cmp byte ptr[esi], 0
            je End

            ; Find the end of the word using SCASB
            push edi; Save EDI again
            mov ebx, esi; EBX points to the start of the word
            mov al, ' '; Character to compare(space)
            mov ecx, 0xFFFFFFFF; Set ECX to maximum value

            FindEndOfWord :
        ; Set EDI to EBX for scanning
            mov edi, ebx
            repne scasb; Scan for space or null terminator
            ; After SCASB, EDI points to the space or null terminator
            mov edx, edi; EDX points to the end of the word
            dec edx; Adjust EDX to point to the last character of the word

            ; Check if we reached the end of the string
            cmp byte ptr[edi - 1], 0
            je ProcessWord; If null terminator, process the word

            ProcessWord :
        ; Zero out the freq array
            push edi; Save EDI
            lea edi, freq; EDI points to freq array
            mov ecx, 256; Length of freq array
            mov eax, 0; Zero value
            rep stosb; Zero out freq array
            pop edi; Restore EDI

            ; Process the word from ESI to EDX
            mov ecx, edx; ECX = end pointer
            sub ecx, esi; ECX = length of the word

            ProcessWordLoop :
        cmp ecx, 0
            jl CheckFreq
            mov al, [esi]; Load character from source
            movzx ebx, al; Zero - extend AL into EBX
            inc byte ptr[freq + ebx]; Increment frequency count
            inc esi; Move to next character
            dec ecx
            jmp ProcessWordLoop

            CheckFreq :
        ; Check frequencies in freq array
            lea esi, freq; ESI points to freq array
            mov ecx, 256; Length of freq array

            CheckFreqLoop:
        lodsb; Load byte from freq into AL and increment ESI
            cmp al, 3; Compare frequency with 3
            jg HasRepeats; If frequency > 3, proceed to copy word
            loop CheckFreqLoop; Continue checking frequencies

            ; If no character repeats more than 3 times, skip the word
            pop edi; Restore EDI(output pointer)
            mov esi, edx; ESI points to the position after the word
            inc esi; Move past the space or null terminator
            jmp Start

            HasRepeats :
        ; Copy the word to the output
            ; ESI currently points to freq array; reset it to start of the word
            mov esi, edx; EDX points to end of the word + 1
            sub esi, ecx; ESI = EDX - word length(start of the word)
            mov ecx, edx; ECX = end pointer
            sub ecx, esi; ECX = length of the word

            CopyWordLoop :
        cmp ecx, 0
            jle EndCopy
            mov al, [esi]; Load character from source
            mov[edi], al; Store character to output
            inc esi; Move to next character in source
            inc edi; Move to next position in output
            dec ecx
            jmp CopyWordLoop

            EndCopy :
        ; Add a space after the word in the output
            mov byte ptr[edi], ' '
            inc edi

            pop edi; Restore EDI(output pointer)
            mov esi, edx; ESI points to the position after the word
            inc esi; Move past the space or null terminator
            jmp Start

            End :
        mov byte ptr[edi], 0; Null - terminate the output string
    }

    printf("%s\n", output);
    return 0;
}
