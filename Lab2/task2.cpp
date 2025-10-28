#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <numeric>

using namespace std;

// Функция вычисления выборочного среднего
double compute_mean(const vector<double>& data) {
    double sum = accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

// Функция вычисления несмещенной выборочной дисперсии
double compute_variance(const vector<double>& data, double mean) {
    double sum_squares = 0.0;
    for (double x : data) {
        double diff = x - mean;
        sum_squares += diff * diff;
    }
    return sum_squares / (data.size() - 1);
}

// Функция для проверки правильности работы на малой выборке
void test_functions() {
    cout << "ПРОВЕРКА ФУНКЦИЙ НА МАЛОЙ ВЫБОРКЕ" << endl;
    
    // Тестовая выборка: 1, 2, 3, 4, 5
    vector<double> test_data = {1.0, 2.0, 3.0, 4.0, 5.0};
    
    // Ожидаемые результаты:
    // Среднее = (1+2+3+4+5)/5 = 15/5 = 3.0
    // Дисперсия = [(1-3)² + (2-3)² + (3-3)² + (4-3)² + (5-3)²] / (5-1) 
    //           = [4 + 1 + 0 + 1 + 4] / 4 = 10/4 = 2.5
    
    double test_mean = compute_mean(test_data);
    double test_variance = compute_variance(test_data, test_mean);
    
    cout << "Тестовая выборка: ";
    for (double x : test_data) {
        cout << x << " ";
    }
    cout << endl;
    
    cout << "Вычисленное среднее: " << test_mean << endl;
    cout << "Ожидаемое среднее: 3.0" << endl;
    
    cout << "Вычисленная дисперсия: " << test_variance << endl;
    cout << "Ожидаемая дисперсия: 2.5" << endl;
    cout << endl;
}

// Функция для работы в потоке
void compute_partial_stats(const vector<double>& data, double& mean, double& variance, int& count) {
    mean = compute_mean(data);
    variance = compute_variance(data, mean);
    count = data.size();
}

// Функция генерации массива заданного размера
vector<double> generate_data(size_t n) {
    vector<double> data(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 100.0);
    
    for (size_t i = 0; i < n; ++i) {
        data[i] = dis(gen);
    }
    return data;
}

int main() {

    test_functions();

    constexpr size_t base_n = 10000000;  // 10^7 элементов базовая выборка
    
    // Параллельные вычисления с 2, 3 и 4 потоками
    for (int num_threads : {2, 3, 4}) {
        // Размер каждого подмассива в num_threads раз меньше
        size_t chunk_size = base_n / num_threads;
        size_t total_size = chunk_size * num_threads;  // общий размер данных
        
        // Генерируем данные для этого количества потоков
        vector<double> data = generate_data(total_size);
        
        // Вычисляем общие статистики
        double overall_mean = compute_mean(data);
        double overall_variance = compute_variance(data, overall_mean);
        
        vector<thread> threads;
        vector<double> means(num_threads);
        vector<double> variances(num_threads);
        vector<int> counts(num_threads);
        vector<vector<double>> subData(num_threads);

        // Разделяем данные на подмассивы
        for (int i = 0; i < num_threads; ++i) {
            int start = i * chunk_size;
            int end = start + chunk_size;
            subData[i] = vector<double>(data.begin() + start, data.begin() + end);
        }

        // Замер времени начала
        auto start = chrono::steady_clock::now();

        // Создаем потоки
        for (int i = 0; i < num_threads; ++i) {
            threads.push_back(thread(compute_partial_stats, ref(subData[i]), 
                                   ref(means[i]), ref(variances[i]), ref(counts[i])));
        }

        // Ждем завершения потоков
        for (auto& th : threads) {
            th.join();
        }

        // Замер времени окончания
        auto end = chrono::steady_clock::now();
        chrono::duration<double> duration = end - start;

        // Вывод результатов
        cout << "Количество потоков: " << num_threads << endl;
        cout << "Размер каждого подмассива: " << chunk_size << " элементов" << endl;
        cout << "Общий размер данных: " << total_size << " элементов" << endl;
        cout << "Общее выборочное среднее: " << overall_mean << endl;
        cout << "Общая несмещенная выборочная дисперсия: " << overall_variance << endl;
        cout << "Время выполнения: " << duration.count() << " секунд." << endl << endl;
    }

    return 0;
}