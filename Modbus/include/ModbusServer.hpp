#ifndef MODBUS_SERVER_HPP
#define MODBUS_SERVER_HPP

#include <modbus/modbus.h>
#include <memory>
#include <vector>

class ModbusServer {
public:
    ModbusServer(const char* address, int port);
    ~ModbusServer();

    void Run();

private:
    modbus_t* ctx_;
    modbus_mapping_t* mapping_;
    int serverSocket_;
};

#endif
