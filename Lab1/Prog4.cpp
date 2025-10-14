#include <iostream>
#include <random>
#include <map>
#include <iomanip>

using namespace std;

// Функция для генерации распределения Пуассона и построения гистограммы
void generatePoissonHistogram(double lambda, int num_samples) {
    // Генератор случайных чисел
    random_device rd;
    mt19937 gen(rd());
    poisson_distribution<int> distribution(lambda);

    map<int, int> frequency; // Контейнер для хранения частоты

    // Генерация выборок
    for (int i = 0; i < num_samples; ++i) {
        frequency[distribution(gen)]++; // Сгенерировали значение и сразу увеличили счетчик
    }

    // Вывод гистограммы
    cout << "Гистограмма распределения Пуассона (lambda = " << lambda << "):" << endl;

    //Находим максимальную частоту для нормализации высоты гистограммы
    int max_frequency = 0;
    for (const auto& [value, count] : frequency) {
      if (count > max_frequency) {
        max_frequency = count;
      }
    }

    for (const auto& [value, count] : frequency) {
        cout << setw(2) << value << ": ";
        // Нормируем высоту столбца гистограммы, чтобы она помещалась в консоль
        int num_hashes = static_cast<int>(static_cast<double>(count) / max_frequency * 50); // Умножаем для визуализации, регулируем высоту

        for (int j = 0; j < num_hashes; ++j) {
            cout << "*"; // Отображение количества с помощью символа *
        }
        cout << " (" << count << ")" << endl; // Вывод абсолютной частоты
    }
}

int main() {
    double lambda = 4.0;
    int num_samples = 1000;

    generatePoissonHistogram(lambda, num_samples);

    return 0;
}