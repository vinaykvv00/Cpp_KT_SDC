#include "Topic.h"
#include "Producer.h"
#include "Consumer.h"
#include "Message.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "  Pub-Sub System " << std::endl;
    
    Topic newsTopic("news");
    
    Consumer consumer1(1, "C1");
    Consumer consumer2(2, "C2");
    Consumer consumer3(3, "C3");
    
    Producer producer1(1, "P1");
    Producer producer2(2, "P2");
    
    std::cout << "\n--- Starting Consumers ---" << std::endl;
    consumer1.start(newsTopic, 10);
    consumer2.start(newsTopic, 10);
    consumer3.start(newsTopic, 10);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    std::cout << "\n--- Starting Producers ---" << std::endl;
    producer1.start(newsTopic, 5);
    producer2.start(newsTopic, 5);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    
    producer1.stop();
    producer2.stop();
    
    std::cout << "\n--- Producers Finished ---" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    newsTopic.shutdown();
    consumer1.stop();
    consumer2.stop();
    consumer3.stop();
    
    std::cout << "\n Results---" << std::endl;
    std::cout << "Consumer 1 received: " << consumer1.getMessagesReceived() << " messages" << std::endl;
    std::cout << "Consumer 2 received: " << consumer2.getMessagesReceived() << " messages" << std::endl;
    std::cout << "Consumer 3 received: " << consumer3.getMessagesReceived() << " messages" << std::endl;
    std::cout << "Total messages sent: 10" << std::endl;
    std::cout << "\n Pub-Sub Pattern: Each consumer gets ALL messages" << std::endl;
    
    std::cout << "\n Test Complete-------------" << std::endl;
    return 0;
}
