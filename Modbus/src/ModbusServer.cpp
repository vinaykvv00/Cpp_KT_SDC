#include "ModbusServer.hpp"
#include <iostream>

ModbusServer::ModbusServer(const char* address, int port) : ctx_(nullptr), mapping_(nullptr), serverSocket_(-1) {

    ctx_ = modbus_new_tcp(address, port);
    if (!ctx_) {
        throw std::runtime_error("Failed to create Modbus TCP context");
    }

    mapping_ = modbus_mapping_new(0, 0, 10, 0);
    if (!mapping_) {
        modbus_free(ctx_);
        throw std::runtime_error("Failed to allocate Modbus mapping");
    }

    // Initialize holding registers
    for (int i = 0; i < 10; ++i) {
        mapping_->tab_registers[i] = (i + 1) * 10;   // 10,20,30,...
    }

    serverSocket_ = modbus_tcp_listen(ctx_, 1);
    if (serverSocket_ == -1) {
        throw std::runtime_error("Failed to listen on socket");
    }
}

ModbusServer::~ModbusServer() {
    if (mapping_) modbus_mapping_free(mapping_);
    if (ctx_) modbus_free(ctx_);
}

void ModbusServer::Run() {
    std::cout << "Modbus TCP Server running on port 5020..." << std::endl;

    for (;;) {
        int clientSocket = modbus_tcp_accept(ctx_, &serverSocket_);
        if (clientSocket == -1) continue;

        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
        int rc = modbus_receive(ctx_, query);

        if (rc > 0) {
            modbus_reply(ctx_, query, rc, mapping_);
        }
    }
}
