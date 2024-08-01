#include <barrier>
#include <thread>
#include <iostream>

void phase_work(std::barrier<>& barrier, int phase) {
    // Do some work for the current phase...
    std::cout << "Thread in phase " << phase << std::endl;
    barrier.arrive_and_wait(); // Wait for other threads to reach this point
}

int main() {
    std::barrier barrier(3); // Create a barrier for 3 threads

    std::jthread t1([&barrier]() {
        for (int i = 0; i < 3; ++i) {
            phase_work(barrier, i);
        }
    });

    std::jthread t2([&barrier]() {
        for (int i = 0; i < 3; ++i) {
            phase_work(barrier, i);
        }
    });

    std::jthread t3([&barrier]() {
        for (int i = 0; i < 3; ++i) {
            phase_work(barrier, i);
        }
    });

    t1.join();
    t2.join();
    t3.join();

    std::cout << "All phases completed\n";
}
