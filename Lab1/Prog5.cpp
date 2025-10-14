#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Определение структуры для хранения точки на плоскости
struct Point {
    double x, y;
};

// Функция для проверки принадлежности точки области Ω
bool area(const Point& p, double a) {
    return (p.x + p.y < 5 * a / 2) && (p.x * p.y > a * a);
}

int main() {
    // Пример чисел
    vector<int> arr = {0, -2, 3, 0, 5, -4, 1, 0, -3, 2};

    // 1. Удаляем все нули
    arr.erase(remove(arr.begin(), arr.end(), 0), arr.end());
    cout << "После удаления нулей: ";
    for (const auto& num : arr) {
        cout << num << " ";
    }
    cout << endl;

    // 2. Удаляем отрицательные четные числа
    arr.erase(remove_if(arr.begin(), arr.end(), [](int num) { return num < 0 && num % 2 == 0; }), arr.end());
    cout << "После удаления отрицательных четных чисел: ";
    for (const auto& num : arr) {
        cout << num << " ";
    }
    cout << endl;

    // 3. Удаляем положительные нечетные числа
    arr.erase(remove_if(arr.begin(), arr.end(), [](int num) { return num > 0 && num % 2 != 0; }), arr.end());
    cout << "После удаления положительных нечетных чисел: ";
    for (const auto& num : arr) {
        cout << num << " ";
    }
    cout << endl;

    // Работа с точками
    vector<Point> points = {{2.5, 1.7}, {5.0, 7.0}, {1.8, 2.3}, {3.0, 1.0}, {15.0, 8.0}};
    double a = 2.0; // Значение параметра a

    // Удаляем точки, не попадающие в область Ω
    points.erase(remove_if(points.begin(), points.end(), [&a](const Point& p) { return !area(p, a); }), points.end());
    cout << "После удаления точек, не попадающих в область Ω: ";
    
    // Выводим точки, которые остались
    for (const auto& p : points) {
        cout << "(" << p.x << ", " << p.y << ") ";
    }
    cout << endl;

    return 0;
}
