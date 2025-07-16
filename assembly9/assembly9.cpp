#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <fstream>
#include <algorithm>

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

int main() {
    double x;
    int N;
    const int MEASUREMENTS = 10000;

    unsigned long long cpp_times[MEASUREMENTS];
    unsigned long long asm_times[MEASUREMENTS];

    std::ofstream outFile("results.txt");
    if (!outFile) {
        std::cout << "Error opening file!" << std::endl;
        return 1;
    }

    std::cout << "Input X (in radians): ";
    std::cin >> x;
    outFile << "X = " << x << std::endl;

    std::cout << "Input N: ";
    std::cin >> N;
    outFile << "N = " << N << std::endl << std::endl;

    x = fmod(x + M_PI / 2, M_PI) - M_PI / 2;

    double resultC = 0.0;
    double resultA = 0.0;
    double term, x_power;

    double coefficients[] = {
        1.0,                    // x^1
        1.0 / 3.0,             // x^3
        2.0 / 15.0,            // x^5
        17.0 / 315.0,          // x^7
        62.0 / 2835.0,         // x^9
        1382.0 / 155925.0,     // x^11
        21844.0 / 6081075.0,   // x^13
        929569.0 / 638512875.0,// x^15
        6404582.0 / 10854718875.0,    // x^17
        443861162.0 / 1856156927625.0,    // x^19
        18888466084.0 / 194896477400625.0,    // x^21
        113927491862.0 / 2900518163668125.0,  // x^23
        58870668456604.0 / 3698160658676859375.0,  // x^25
        8374643517010684.0 / 1298068116137687890625.0,  // x^27
        689005380505609448.0 / 263711482548015766796875.0  // x^29
    };

    outFile << "C++ Implementation measurements:" << std::endl;
    for (int measurement = 0; measurement < MEASUREMENTS; measurement++) {
        unsigned long long start_rdtsc = __rdtsc();

        resultC = 0.0;
        for (int i = 0; i < N && i < 15; i++) {
            // Вычисляем x^(2i+1) прямым умножением
            x_power = x;  // Первая степень
            for (int j = 1; j <= 2 * i; j++) {  // Умножаем ещё 2*i раз
                x_power *= x;
            }
            term = coefficients[i] * x_power;
            resultC += term;
        }

        unsigned long long end_rdtsc = __rdtsc();
        cpp_times[measurement] = end_rdtsc - start_rdtsc;
        outFile << "Measurement " << measurement + 1 << ": " << cpp_times[measurement] << " ticks" << std::endl;
    }

    outFile << "\nAssembly Implementation measurements:" << std::endl;
    for (int measurement = 0; measurement < MEASUREMENTS; measurement++) {
        unsigned long long start_rdtsc = __rdtsc();

        __asm {
            finit
            fldz; загружаем 0 для суммы

            xor ecx, ecx; i = 0
            mov edx, N; загружаем N
            cmp edx, 15
            jle short_n
            mov edx, 15
            short_n:

loop_start:
            cmp ecx, edx
            jge loop_end

            ; Загружаем x для начала возведения в степень
            fld qword ptr[x]

            ; Вычисляем количество дополнительных умножений(2 * i)
            mov eax, ecx; eax = i
            shl eax, 1; eax = 2 * i

            ; Цикл умножения на x
            xor ebx, ebx; счётчик j = 0
power_loop:
            cmp ebx, eax; сравниваем j с 2 * i
            jge power_done; если j >= 2 * i, степень готова
            fld qword ptr[x]; загружаем x ещё раз
            fmulp st(1), st(0); умножаем
            inc ebx
            jmp power_loop

power_done :
            ; Умножаем на коэффициент
            fld qword ptr coefficients[ecx * 8]
            fmulp st(1), st(0)

            ; Добавляем к сумме
            faddp st(1), st(0)

            inc ecx
            jmp loop_start

loop_end :
            fstp qword ptr[resultA]
        }

        unsigned long long end_rdtsc = __rdtsc();
        asm_times[measurement] = end_rdtsc - start_rdtsc;
        outFile << "Measurement " << measurement + 1 << ": " << asm_times[measurement] << " ticks" << std::endl;
    }

    unsigned long long min_cpp_time = *std::min_element(cpp_times, cpp_times + MEASUREMENTS);
    unsigned long long min_asm_time = *std::min_element(asm_times, asm_times + MEASUREMENTS);

    outFile << "\nResults:" << std::endl;
    outFile << "Minimum C++ time: " << min_cpp_time << " ticks" << std::endl;
    outFile << "Minimum ASM time: " << min_asm_time << " ticks" << std::endl;
    outFile << "C++ result: " << resultC << std::endl;
    outFile << "ASM result: " << resultA << std::endl;
    outFile << "Standard tan(x): " << tan(x) << std::endl;

    outFile.close();
    std::cout << "\nResults have been written to results.txt" << std::endl;

    return 0;
}