#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>

std::counting_semaphore<3> parking_slots(3);  

void car(int id) {
    std::cout << "Car " << id << " waiting for parking...\n";
    parking_slots.acquire(); 
    std::cout << "Car " << id << " parked!\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); 
    std::cout << "Car " << id << " leaving.\n";
    parking_slots.release(); 
}

int main() {
    std::thread cars[6];
    for (int i = 0; i < 6; ++i)
        cars[i] = std::thread(car, i+1);

    for (auto& c : cars)
        c.join();
}
