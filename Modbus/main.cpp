#include "ModbusServer.hpp"
#include "ModbusClient.hpp"
#include <thread>
#include <chrono>
#include <iostream>

int main() {

    std::thread serverThread([]() {
        ModbusServer server("0.0.0.0", 5020);
        server.Run();
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));
    try {
        ModbusClient client("127.0.0.1", 5020);

        auto values = client.ReadHoldingRegisters(0, 10);

        std::cout << "Client received registers:\n";
        for (int v : values) {
            std::cout << v << " ";
        }
        std::cout << std::endl;

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    serverThread.join();
    return 0;
}
