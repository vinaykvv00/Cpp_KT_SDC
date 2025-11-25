#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>

int main() {
    std::cout << " WRITER " << std::endl;
 
    key_t key = ftok("/tmp", 65);
    std::cout << "Key created: " << key << std::endl;
    
    int shmid = shmget(key, 100, IPC_CREAT | 0666);
    std::cout << "Shared memory ID: " << shmid << std::endl;
    
    char* data = (char*)shmat(shmid, nullptr, 0);
    std::cout << "Attached to memory" << std::endl;
    
    strcpy(data, "Hello writing the message from writer process!");
    std::cout << "Wrote: " << data << std::endl;
    
    std::cout << "\nWaiting 10 seconds for reader to read..." << std::endl;
    sleep(10);
    
    shmdt(data);  
    shmctl(shmid, IPC_RMID, nullptr); 
    std::cout << "Cleaned up!" << std::endl;
    
    return 0;
}
