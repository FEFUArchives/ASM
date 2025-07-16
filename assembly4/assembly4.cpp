#include <stdio.h>
#include <string.h>

int main() {
    char source[64] = "mmmgmm nnnnnn oooo pppp qqq rr";
    char output[256] = "";
    static char freq[256];

    __asm {
        ; Инициализация указателей
        lea esi, source; ESI указывает на начало source
        lea edi, output; EDI указывает на начало output

Start :
SkipSpaces :
        lodsb
        cmp al, 0
        je End
        cmp al, ' '
        je SkipSpaces
        ; Начало слова найдено
        ; Сохранение адреса начала слова
        mov edx, esi
        sub edx, 1

        ; Инициализация массива freq
        push edi
        lea edi, freq
        mov ecx, 256
        xor al, al; AL = 0
        rep stosb; Обнуляем массив freq
        pop edi; Восстанавливаем EDI(указатель на output)

ProcessWord:
        lodsb
        cmp al, 0
        je CheckFreq; Если конец, проверяем частоты
        cmp al, ' '
        je CheckFreq; Если пробел, проверяем частоты

        movzx ebx, al
        inc byte ptr[freq + ebx]; Увеличиваем freq[al]

        jmp ProcessWord

CheckFreq :
        ; Проверка массива freq на наличие символов с частотой > 3
        mov ecx, 256
        lea esi, freq
        xor eax, eax; EAX = флаг(0 - не найдено, 1 - найдено)

CheckFreqLoop:
        lodsb
        cmp al, 3; Проверяем, больше ли частота 3
        jae SetFlag; Если да, устанавливаем флаг
        loop CheckFreqLoop; Уменьшаем ECX и повторяем, если ECX != 0
        jmp AfterFreqCheck; Переходим после цикла

SetFlag :
        mov eax, 1; Устанавливаем флаг(EAX = 1)
        jmp AfterFreqCheck; Переходим после установки флага

AfterFreqCheck :
        cmp eax, 1
        jne SkipWord

        ; Копирование слова в output
        mov esi, edx; Восстанавливаем ESI на начало слова

CopyWord :
        lodsb; Загружаем символ из[ESI] в AL, увеличиваем ESI
        cmp al, 0
        je EndCopy
        cmp al, ' '
        je EndCopy
        stosb
        jmp CopyWord

EndCopy :
        mov al, ' '; Добавляем пробел после слова
        stosb; Записываем пробел в[EDI], увеличиваем EDI
        jmp Start; Переходим к обработке следующего слова

SkipWord :
        jmp Start; Переходим к обработке следующего слова

End :
        mov byte ptr[edi], 0
    }

    printf("%s\n", output);
    return 0;
}
