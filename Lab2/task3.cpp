#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <numeric>
#include <cmath>

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

// Функция для вычисления среднего и стандартного отклонения времени
pair<double, double> compute_time_stats(const vector<double>& times) {
    double mean = compute_mean(times);
    double variance = compute_variance(times, mean);
    double std_dev = sqrt(variance);
    return {mean, std_dev};
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

// Функция для выполнения одного замера параллельных вычислений
double measure_parallel_time(int num_threads, size_t chunk_size, size_t total_size) {
    vector<double> data = generate_data(total_size);
    
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
    
    return duration.count();
}

int main() {
    constexpr size_t base_n = 10000000;  // 10^7 элементов базовая выборка
    constexpr int num_measurements = 13; // Количество замеров для статистической значимости
    
    // Параллельные вычисления с 2, 3 и 4 потоками
    for (int num_threads : {2, 3, 4}) {
        // Размер каждого подмассива в num_threads раз меньше
        size_t chunk_size = base_n / num_threads;
        size_t total_size = chunk_size * num_threads;
        
        vector<double> execution_times;
        
        // Выполняем multiple замеров
        for (int i = 0; i < num_measurements; ++i) {
            double time = measure_parallel_time(num_threads, chunk_size, total_size);
            execution_times.push_back(time);
        }
        
        // Вычисляем статистику времени
        auto [mean_time, std_dev] = compute_time_stats(execution_times);
        
        // Вывод результатов
        cout << "Количество потоков: " << num_threads << endl;
        cout << "Количество замеров: " << num_measurements << endl;
        cout << "Размер каждого подмассива: " << chunk_size << " элементов" << endl;
        cout << "Общий размер данных: " << total_size << " элементов" << endl;
        cout << "Время выполнения: " << mean_time << " ± " << std_dev << " секунд" << endl;
        
        // Дополнительная информация о замерах
        cout << "Отдельные замеры: ";
        for (double t : execution_times) {
            cout << t << " ";
        }
        cout << endl << endl;
    }

    return 0;
}