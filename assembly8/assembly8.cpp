#include <stdio.h>
#include <string.h>

int main() {
    char source[] = "madam level noon racecar hello world";
    int palindrome_count = 0;
    int length = strlen(source);

    __asm {
        lea esi, source; ESI указывает на начало строки
        mov edi, esi; EDI будет использоваться для SCASB
        mov ecx, length; ECX содержит оставшуюся длину строки
        xor edx, edx; EDX будет использоваться для хранения начала слова
        xor ebx, ebx; EBX будет использоваться как временный регистр
        xor eax, eax; Обнуляем EAX
        cld; Устанавливаем направление вперед для SCASB

        Start :
        cmp ecx, 0
            je End; Если достигли конца строки, выходим

            ; Пропускаем пробелы с помощью REPE SCASB
            mov al, ' '
            SkipSpaces:
        repe scasb; Повторяем, пока AL == [EDI]
            ; После REPE SCASB EDI указывает на первый символ, отличный от пробела
            ; Корректируем ECX
            mov esi, edi; ESI указывает на начало слова
            inc ecx; Возвращаем ECX, так как SCASB уменьшил его лишний раз
            dec edi; Корректируем EDI, так как SCASB увеличил его лишний раз
            cmp ecx, 0
            je End; Если достигли конца строки, выходим

            ; Проверяем, достигли ли конца строки
            mov al, [esi]
            cmp al, 0
            je End

            ; Сохраняем начало слова
            mov edx, esi; EDX = начало слова

            ; Находим конец слова с помощью REPNE SCASB
            mov al, ' '
            mov ecx, length
            sub ecx, esi
            add ecx, source
            sub ecx, edi; ECX = оставшаяся длина строки
            FindWordEnd :
        repne scasb; Повторяем, пока AL != [EDI]
            ; После REPNE SCASB EDI указывает на пробел или конец строки
            ; Корректируем ECX
            inc ecx; Возвращаем ECX, так как SCASB уменьшил его лишний раз
            dec edi; Корректируем EDI, чтобы указать на последний символ слова

            ; Проверяем, достигли ли конца строки
            cmp byte ptr[edi], 0
            je PreparePalindromeCheck

            ; Проверяем длину слова
            mov eax, edi
            sub eax, edx; EAX = конец слова - начало слова
            cmp eax, 0
            jl ContinueProcessing; Если длина <= 0, переходим к следующему слову

            ; Подготовка к проверке на палиндром
            mov esi, edx; ESI = начало слова
            mov edi, edi; EDI = конец слова

            CheckPalindrome :
        mov al, [esi]
            mov bl, [edi]
            cmp al, bl
            jne NotPalindrome; Если символы не равны, слово не палиндром
            inc esi
            dec edi
            cmp esi, edi
            jae IsPalindrome; Если указатели встретились или пересеклись
            jmp CheckPalindrome

            IsPalindrome :
        inc palindrome_count; Увеличиваем счетчик палиндромов
            jmp ContinueProcessing

            NotPalindrome :
        ; Не палиндром, продолжаем

            ContinueProcessing :
        ; Обновляем EDI и ECX для следующей итерации
            mov esi, edi
            inc esi; Переходим к следующему символу после слова
            mov edi, esi
            mov ecx, length
            sub ecx, esi
            add ecx, source
            sub ecx, edi; Обновляем ECX
            jmp Start

            PreparePalindromeCheck :
        ; Специальная обработка для конца строки
            cmp ecx, 0
            je End
            dec edi; Корректируем EDI
            jmp ContinueProcessing

            End :
        ; Результат в palindrome_count
    }

    printf("Number of palindromes: %d\n", palindrome_count);
    return 0;
}
