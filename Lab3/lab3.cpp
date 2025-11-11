#include <iostream>
#include <fstream>
#include <random>
#include <vector>

using namespace std;

// LCG генератор
uint64_t LCG(uint64_t& seed) {
    constexpr uint64_t m = 18446744073709551615llu;
    constexpr uint64_t a = 6364136223846793005llu;
    constexpr uint64_t c = 1442695040888963407llu;
    seed = (a * seed + c) % m;
    return seed;
}

// Инициализация seed
uint64_t initialize_seed() {
    random_device rd;
    uint64_t seed = rd();
    return seed;
}

int main(int argc, char *argv[]) {
    unsigned long N = 10000000;
    if (argc > 1) {
        N = stoul(argv[1]);
    }

    // Инициализация генератора
    uint64_t seed = initialize_seed();
    
    // Вывод ТОЛЬКО чисел для графиков
    uint64_t current_seed = seed;
    for (unsigned long i = 0; i < N; ++i) {
        cout << LCG(current_seed);
        if (i < N - 1) {
            cout << endl;
        }
    }

    return 0;
}