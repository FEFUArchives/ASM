#include <stdio.h>
#include <string.h>
#include <iostream>

int main() {
    setlocale(LC_ALL, "Russian");

    char source[100] = "programming";
    char dest[100];
    char vowel_present[5] = { 0 }; // Индикаторы наличия гласных 'a', 'e', 'i', 'o', 'u'
    char vowels[6] = "aeiou";

    _asm {
        lea esi, source; ESI указывает на исходную строку
        lea edi, dest; EDI указывает на результирующую строку

        ; Сохраняем EDI и инициализируем vowel_present нулями
        push edi
        lea edi, vowel_present
        mov ecx, 5
        xor eax, eax
        rep stosb; Заполняем vowel_present нулями
        pop edi; Восстанавливаем EDI

        ProcessLoop :
        mov al, [esi]
            cmp al, 0
            je EndProcess

            cmp al, 'a'
            je IsVowelA
            cmp al, 'e'
            je IsVowelE
            cmp al, 'i'
            je IsVowelI
            cmp al, 'o'
            je IsVowelO
            cmp al, 'u'
            je IsVowelU

            ; Не гласная, копируем символ в dest
            mov[edi], al
            inc edi
            inc esi
            jmp ProcessLoop

            IsVowelA :
        mov byte ptr[vowel_present], 1
            inc esi
            jmp ProcessLoop

            IsVowelE :
        mov byte ptr[vowel_present + 1], 1
            inc esi
            jmp ProcessLoop

            IsVowelI :
        mov byte ptr[vowel_present + 2], 1
            inc esi
            jmp ProcessLoop

            IsVowelO :
        mov byte ptr[vowel_present + 3], 1
            inc esi
            jmp ProcessLoop

            IsVowelU :
        mov byte ptr[vowel_present + 4], 1
            inc esi
            jmp ProcessLoop

            EndProcess :
        mov byte ptr[edi], 0; Завершаем строку нулевым символом
    }

    printf("Исходная строка: %s\n", source);
    printf("Результат без гласных: %s\n", dest);

    printf("Гласные, которых не было в строке: ");
    for (int i = 0; i < 5; i++) {
        if (vowel_present[i] == 0) {
            printf("%c ", vowels[i]);
        }
    }
    printf("\n");

    return 0;
}
