#include <iostream>
#include <limits>
#include <cstdint>

using namespace std;

int main(int argc, char* argv[]) {
    // Преобразуем аргумент командной строки в беззнаковое целое 64-битное число
    unsigned long long number = static_cast<unsigned long long>(atoi(argv[1]));

    // Выводим информацию о числе
    cout << "Положительное целое число: " << number << endl;

    // Преобразуем число в другие целые типы
    int32_t  int32_signed   = static_cast<int32_t>(number);
    uint32_t uint32_unsigned = static_cast<uint32_t>(number);
    int64_t  int64_signed   = static_cast<int64_t>(number);

    // Выводим преобразованные числа
    cout << "32-битное целое со знаком: " << int32_signed << endl;
    cout << "32-битное целое без знака: " << uint32_unsigned << endl;
    cout << "64-битное целое со знаком: " << int64_signed << endl;

    // Максимальные и минимальные значения для типов
    cout << "Максимальное значение для 32-битного целого со знаком: " << numeric_limits<int32_t>::max() << endl;
    cout << "Минимальное значение для 32-битного целого со знаком: " << numeric_limits<int32_t>::min() << endl;

    cout << "Максимальное значение для 32-битного целого без знака: " << numeric_limits<uint32_t>::max() << endl;

    cout << "Максимальное значение для 64-битного целого со знаком: " << numeric_limits<int64_t>::max() << endl;
    cout << "Минимальное значение для 64-битного целого со знаком: " << numeric_limits<int64_t>::min() << endl;

    return 0;
}

