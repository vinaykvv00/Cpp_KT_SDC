#include "ModbusClient.hpp"
#include <iostream>
#include <stdexcept>

ModbusClient::ModbusClient(const char* address, int port) : ctx_(nullptr) {

    ctx_ = modbus_new_tcp(address, port);

    if (!ctx_) {
        throw std::runtime_error("Failed to create Modbus client context");
    }

    if (modbus_connect(ctx_) == -1) {
        modbus_free(ctx_);
        throw std::runtime_error("Failed to connect to Modbus server");
    }
}

ModbusClient::~ModbusClient() {
    if (ctx_) {
        modbus_close(ctx_);
        modbus_free(ctx_);
    }
}

std::vector<uint16_t> ModbusClient::ReadHoldingRegisters(int start, int count) {
    std::vector<uint16_t> result(count);

    int rc = modbus_read_registers(ctx_, start, count, result.data());
    if (rc == -1) {
        throw std::runtime_error("Failed to read registers");
    }

    return result;
}
