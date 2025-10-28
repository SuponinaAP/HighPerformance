#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

using namespace std;

int main() {
    const unsigned int num_threads = 8;
    mutex cout_mutex;
    
    // Создаем и запускаем 8 потоков
    vector<thread> threads;
    for (unsigned int i = 1; i <= num_threads; ++i) {
        threads.emplace_back([i, &cout_mutex]() {
            // Замер времени начала для этого потока
            auto start = chrono::steady_clock::now();
            
            this_thread::sleep_for(chrono::seconds(i));
            
            // Замер времени окончания для этого потока
            auto end = chrono::steady_clock::now();
            
            // Вычисляем время работы этого потока
            auto duration = chrono::duration_cast<chrono::seconds>(end - start);
            
            // Синхронизированный вывод
            lock_guard<mutex> lock(cout_mutex);
            cout << "Поток " << i << " работал: " << duration.count() << " секунд" << endl;
        });
    }
    
    // Ожидаем завершения всех потоков
    for (auto& th : threads) {
        th.join();
    }
    
    return 0;
}