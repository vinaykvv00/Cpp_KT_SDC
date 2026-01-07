#ifndef MODBUS_CLIENT_HPP
#define MODBUS_CLIENT_HPP

#include <modbus/modbus.h>
#include <memory>
#include <vector>

class ModbusClient {
public:
    ModbusClient(const char* address, int port);
    ~ModbusClient();

    std::vector<uint16_t> ReadHoldingRegisters(int start, int count);

private:
    modbus_t* ctx_;
};

#endif
