#include <iostream>
#include <bitset>

// Дано число в двоичном виде. Вывернуть число "наизнанку" (разряды стоящие в середине сделать крайними). Результат разделить на 16.
int main() {
	char c; c = ' ';
	_asm {
		mov al,01100010b
		mov bl,al
		mov cl,00000000b

		and bl,10001000b // сдвинуть 7 и 3 позиции на 0 и 4
		shr bl,3
		or cl,bl
		mov bl,al

		and bl,01000100b // сдвинуть 6 и 2 позиции на 1 и 5
		shr bl,1
		or cl, bl
		mov bl,al

		and bl, 00100010b // сдвинуть 5 и 1 позиции на 6 и 2
		shl bl, 1
		or cl, bl

		shr cl,4 // делить на 16

		mov c,cl
	}
	std::cout << std::bitset<8>(c) << std::endl;
}