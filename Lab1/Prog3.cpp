#include <iostream>
#include <random>
#include <vector>
#include <iomanip>

using namespace std;

// Функция для генерации биномиального распределения и вывода нормированной гистограммы
void generateBinomialDistribution(int n, double p, int num_samples) {
    // Генератор случайных чисел
    random_device rd;                 // Использует аппаратный источник случайных чисел
    mt19937 gen(rd());                // Инициализация генератора Mersenne Twister
    binomial_distribution<int> distribution(n, p); // Биномиальное распределение

    vector<int> results(n + 1, 0);    // Вектор для хранения количества результатов (от 0 до n)

    // Генерация выборок
    for (int i = 0; i < num_samples; ++i) {
        int sample = distribution(gen);
        results[sample]++; // Увеличиваем счетчик для выпавшего значения
    }

    // Вывод нормированной гистограммы
    cout << "Нормированная гистограмма биномиального распределения (n = "
         << n << ", p = " << p << "):" << endl;

    for (int i = 0; i < static_cast<int>(results.size()); ++i) {
        double normalized_frequency = static_cast<double>(results[i]) / num_samples; // Нормировка
        cout << setw(2) << i << ": ";
        int num_hashes = static_cast<int>(normalized_frequency * 100); // Умножение для визуализации
        for (int j = 0; j < num_hashes; ++j) {
            cout << "#"; // Отображение количества с помощью символа #
        }
        cout << " (" << fixed << setprecision(4) << normalized_frequency << ")" << endl; // Вывод нормированной частоты
    }
}

int main() {
    int n = 10;              // Количество испытаний
    double p = 0.5;          // Вероятность успеха
    int num_samples = 10000; // Количество выборок

    generateBinomialDistribution(n, p, num_samples); // Генерация биномиального распределения

     // Проверка ЦПТ (увеличение n)
    cout << "\nПроверка центральной предельной теоремы (увеличение n):\n";
    generateBinomialDistribution(50, p, num_samples);

    return 0;
}
