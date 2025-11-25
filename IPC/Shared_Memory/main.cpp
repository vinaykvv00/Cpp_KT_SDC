#include <iostream>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main() {

    int shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    int* counter = (int*) shmat(shm_id, nullptr, 0);

    *counter = 0; 

    pid_t pid = fork();

    if (pid == 0) {
        for (int i = 0; i < 100000; i++) {
            (*counter)++;   
        }
        shmdt(counter);
        return 0;
    }

    for (int i = 0; i < 100000; i++) {
        (*counter)++;  
    }

    wait(nullptr);

    std::cout << "Final counter value: " << *counter << "\n";

    shmdt(counter);
    shmctl(shm_id, IPC_RMID, nullptr);
    return 0;
}
