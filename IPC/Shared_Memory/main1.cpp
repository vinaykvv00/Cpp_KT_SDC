#include <iostream>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>

struct SharedData {
    int counter;
    sem_t sem;
};

int main() {
    int shm_id = shmget(IPC_PRIVATE, sizeof(SharedData), IPC_CREAT | 0666);
    SharedData* data = (SharedData*) shmat(shm_id, nullptr, 0);

    data->counter = 0;

    // Initialize semaphore (pshared = 1 → shared between processes)
    sem_init(&data->sem, 1, 1);

    pid_t pid = fork();

    if (pid == 0) {
        for (int i = 0; i < 100000; i++) {
            sem_wait(&data->sem);    
            data->counter++;
            sem_post(&data->sem);      
        }
        shmdt(data);
        return 0;
    }

    for (int i = 0; i < 100000; i++) {
        sem_wait(&data->sem);
        data->counter++;
        sem_post(&data->sem);
    }

    wait(nullptr);

    std::cout << "Final counter value (correct): " << data->counter << "\n";

    shmdt(data);
    shmctl(shm_id, IPC_RMID, nullptr);
    return 0;
}
