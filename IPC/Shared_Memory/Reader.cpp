#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main() {
    std::cout << " READER " << std::endl;

    key_t key = ftok("/tmp", 65);
    std::cout << "Key created: " << key << std::endl;
    
    std::cout << "Looking for shared memory..." << std::endl;
    int shmid = shmget(key, 100, 0666);
    if (shmid == -1) {
        std::cout << "ERROR: Shared memory not found!" << std::endl;
        std::cout << "Make sure writer is running first!" << std::endl;
        return 1;
    }
    std::cout << "Found shared memory ID: " << shmid << std::endl;
    
    char* data = (char*)shmat(shmid, nullptr, 0);
    std::cout << "Attached to memory" << std::endl;
    
    std::cout << "Read: " << data << std::endl;
    
    shmdt(data);
    std::cout << "Detached from memory" << std::endl;
    
    return 0;
}
