// P1: Shared Memory WRITER (Producer)
// This process creates shared memory and writes data to it

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>

// Shared memory structure
struct SharedData {
    int counter;
    char message[100];
    bool ready;  // Flag to indicate data is ready
};

int main() {
    std::cout << "=== P1: WRITER (Producer) ===" << std::endl;
    
    // 1. Generate a unique key for shared memory
    // ftok() creates a key from a file path and project ID
    key_t key = ftok("/tmp", 65);  // 'A' = 65
    if (key == -1) {
        perror("ftok failed");
        return 1;
    }
    std::cout << "[P1] Generated key: " << key << std::endl;
    
    // 2. Create shared memory segment
    // shmget(key, size, flags)
    //   - IPC_CREAT: Create if doesn't exist
    //   - 0666: Read/write permissions for all users
    int shmid = shmget(key, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        return 1;
    }
    std::cout << "[P1] Shared memory created. ID: " << shmid << std::endl;
    
    // 3. Attach shared memory to process address space
    // shmat() returns pointer to shared memory
    void* shm_ptr = shmat(shmid, nullptr, 0);
    if (shm_ptr == (void*)-1) {
        perror("shmat failed");
        return 1;
    }
    std::cout << "[P1] Attached to shared memory at address: " << shm_ptr << std::endl;
    
    // 4. Cast void* to our structure type
    SharedData* data = static_cast<SharedData*>(shm_ptr);
    
    // 5. Initialize shared memory
    data->counter = 0;
    data->ready = false;
    std::cout << "[P1] Initialized shared memory" << std::endl;
    
    // 6. Write data to shared memory in a loop
    std::cout << "\n[P1] Starting to write data..." << std::endl;
    for (int i = 1; i <= 5; ++i) {
        // Write counter
        data->counter = i;
        
        // Write message
        snprintf(data->message, sizeof(data->message), 
                 "Message #%d from Writer Process (PID: %d)", i, getpid());
        
        // Set ready flag
        data->ready = true;
        
        std::cout << "[P1] Wrote: counter=" << data->counter 
                  << ", message=\"" << data->message << "\"" << std::endl;
        
        // Wait for reader to consume (simple synchronization)
        while (data->ready) {
            usleep(100000);  // Sleep 100ms
        }
        
        std::cout << "[P1] Data consumed by reader, writing next..." << std::endl;
        sleep(1);  // Wait 1 second before next write
    }
    
    std::cout << "\n[P1] Finished writing. Press Enter to cleanup..." << std::endl;
    std::cin.get();
    
    // 7. Detach from shared memory
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt failed");
        return 1;
    }
    std::cout << "[P1] Detached from shared memory" << std::endl;
    
    // 8. Delete shared memory segment
    // shmctl(shmid, IPC_RMID, nullptr) removes the segment
    if (shmctl(shmid, IPC_RMID, nullptr) == -1) {
        perror("shmctl IPC_RMID failed");
        return 1;
    }
    std::cout << "[P1] Shared memory deleted" << std::endl;
    
    return 0;
}

/*
EXPLANATION OF KEY FUNCTIONS:

1. ftok(pathname, proj_id)
   - Creates a unique key from file path and project ID
   - Same path + ID = same key (allows processes to find same memory)
   - Returns: key_t (integer key) or -1 on error

2. shmget(key, size, shmflg)
   - Creates or accesses shared memory segment
   - key: Unique identifier (from ftok)
   - size: Size in bytes
   - shmflg: IPC_CREAT (create), permissions (0666)
   - Returns: shared memory ID (shmid) or -1 on error

3. shmat(shmid, shmaddr, shmflg)
   - Attaches shared memory to process address space
   - shmid: Shared memory ID from shmget
   - shmaddr: nullptr = let OS choose address
   - shmflg: 0 = read/write, SHM_RDONLY = read-only
   - Returns: pointer to memory or (void*)-1 on error

4. shmdt(shmaddr)
   - Detaches shared memory from process
   - shmaddr: Pointer from shmat
   - Returns: 0 on success, -1 on error

5. shmctl(shmid, cmd, buf)
   - Controls shared memory
   - cmd: IPC_RMID (remove), IPC_STAT (get info)
   - Returns: 0 on success, -1 on error
*/
