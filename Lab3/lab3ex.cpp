#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

// ============================================================================
// ЗАДАНИЕ №1.1: Реализация ГПСЧ с использованием побитовых операций
// ============================================================================

uint64_t LCG(uint64_t& seed) {
    constexpr uint64_t m = 18446744073709551615llu;
    constexpr uint64_t a = 6364136223846793005llu;
    constexpr uint64_t c = 1442695040888963407llu;
    seed = (a * seed + c) % m;
    return seed;
}

// ============================================================================
// ЗАДАНИЕ №1.2: Заполнение массива 64-битными числами
// ============================================================================

vector<uint64_t> generate_numbers(uint64_t seed, unsigned long N) {
    vector<uint64_t> numbers(N);
    uint64_t current_seed = seed;
    
    for (unsigned long i = 0; i < N; ++i) {
        numbers[i] = LCG(current_seed);
    }
    
    return numbers;
}

// ============================================================================
// ЗАДАНИЕ №1.3: Инициализация из /dev/random или random_device
// ============================================================================

uint64_t initialize_seed() {
    ifstream devrand("/dev/urandom", ios::in | ios::binary);
    if (devrand.is_open()) {
        uint64_t seed;
        devrand.read(reinterpret_cast<char*>(&seed), sizeof(seed));
        devrand.close();
        cout << "Seed инициализирован из /dev/urandom" << endl;
        return seed;
    }
    
    random_device rd;
    uint64_t seed = rd();
    cout << "Seed инициализирован из random_device" << endl;
    return seed;
}

// ============================================================================
// Вспомогательные функции
// ============================================================================

vector<double> normalize_data(const vector<uint64_t>& data) {
    vector<double> normalized(data.size());
    uint64_t max_val = *max_element(data.begin(), data.end());
    
    for (size_t i = 0; i < data.size(); ++i) {
        normalized[i] = static_cast<double>(data[i]) / static_cast<double>(max_val);
    }
    
    return normalized;
}

double compute_mean(const vector<double>& data) {
    return accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double compute_variance(const vector<double>& data) {
    double mean = compute_mean(data);
    double sum_sq = 0.0;
    for (double x : data) {
        sum_sq += (x - mean) * (x - mean);
    }
    return sum_sq / (data.size() - 1);
}

// ============================================================================
// ЗАДАНИЕ №4: Онлайн алгоритм Уэлфорда
// ============================================================================

struct OnlineStatistics {
    double mean = 0.0;
    double M2 = 0.0;
    int count = 0;
    
    void update(double x) {
        count++;
        double delta = x - mean;
        mean += delta / count;
        double delta2 = x - mean;
        M2 += delta * delta2;
    }
    
    double get_variance() const {
        return (count > 1) ? M2 / (count - 1) : 0.0;
    }
};

// ============================================================================
// Основная программа для заданий
// ============================================================================

int main(int argc, char *argv[]) {
    unsigned long N = 100000;
    if (argc > 1) {
        N = stoul(argv[1]);
    }

    cout << "ЛАБОРАТОРНАЯ РАБОТА №3 - ГЕНЕРАТОРЫ СЛУЧАЙНЫХ ЧИСЕЛ" << endl;
    cout << "Генерация " << N << " псевдослучайных чисел" << endl;
    
    // ============================================================================
    // ЗАДАНИЕ №1
    // ============================================================================
    
    cout << "\n=== ВЫПОЛНЕНИЕ ЗАДАНИЯ №1 ===" << endl;
    
    uint64_t seed = initialize_seed();
    cout << "Используется seed: " << seed << endl;
    
    vector<uint64_t> numbers = generate_numbers(seed, N);
    cout << "Сгенерировано " << numbers.size() << " чисел" << endl;
    
    // ============================================================================
    // ЗАДАНИЕ №2: Расчет использования памяти
    // ============================================================================
    
    cout << "\n=== ВЫПОЛНЕНИЕ ЗАДАНИЯ №2 ===" << endl;
    
    vector<int> test_sizes = {100, 1000, 10000, 100000, 1000000, 10000000};
    
    cout << "Расчет использования памяти:" << endl;
    for (int size : test_sizes) {
        size_t memory_bytes = size * sizeof(uint64_t);
        size_t memory_mb = memory_bytes / (1024 * 1024);
        cout << "  " << size << " чисел: " << memory_bytes << " байт (" 
             << memory_mb << " МБ)" << endl;
    }
    
    // ============================================================================
    // ЗАДАНИЕ №3: Статистический анализ
    // ============================================================================
    
    cout << "\n=== ВЫПОЛНЕНИЕ ЗАДАНИЯ №3 ===" << endl;
    
    for (int n : test_sizes) {
        if (n > N) continue;
        
        cout << "\n--- Тест для " << n << " чисел ---" << endl;
        
        vector<uint64_t> test_data = generate_numbers(seed, n);
        vector<double> normalized_data = normalize_data(test_data);
        
        double mean = compute_mean(normalized_data);
        double variance = compute_variance(normalized_data);
        double theoretical_mean = 0.5;
        double theoretical_variance = 1.0 / 12.0;
        
        cout << "Выборочное среднее: " << mean << endl;
        cout << "Теоретическое среднее: " << theoretical_mean << endl;
        cout << "Отклонение среднего: " << abs(mean - theoretical_mean) << endl;
        
        cout << "Выборочная дисперсия: " << variance << endl;
        cout << "Теоретическая дисперсия: " << theoretical_variance << endl;
        cout << "Отклонение дисперсии: " << abs(variance - theoretical_variance) << endl;
    }
    
        
        // ============================================================================
    // ЗАДАНИЕ №4: Онлайн алгоритм для больших N
    // ============================================================================
    
    cout << "\n=== ВЫПОЛНЕНИЕ ЗАДАНИЯ №4 ===" << endl;
    
    vector<unsigned long> large_sizes = {100000000, 1000000000};
    
    for (unsigned long large_N : large_sizes) {
        cout << "\n--- Онлайн обработка " << large_N << " чисел ---" << endl;
        
        OnlineStatistics stats;
        uint64_t current_seed = seed;
        
        for (unsigned long i = 0; i < large_N; ++i) {
            uint64_t rnd = LCG(current_seed);
            double x = static_cast<double>(rnd) / static_cast<double>(UINT64_MAX);
            stats.update(x);
        }
        
        cout << "Онлайн среднее: " << stats.mean << endl;
        cout << "Онлайн дисперсия: " << stats.get_variance() << endl;
        cout << "Теоретическое среднее: 0.5" << endl;
        cout << "Теоретическая дисперсия: " << (1.0/12.0) << endl;
        cout << "Отклонение среднего: " << abs(stats.mean - 0.5) << endl;
        cout << "Отклонение дисперсии: " << abs(stats.get_variance() - 1.0/12.0) << endl;
        cout << "Обработано чисел: " << stats.count << endl;
    }
    
    cout << "\nЛабораторная работа завершена!" << endl;
    return 0;
}