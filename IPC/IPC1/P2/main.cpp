// P2: Shared Memory READER (Consumer)
// This process accesses existing shared memory and reads data

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>

// Same structure as writer (MUST match exactly!)
struct SharedData {
    int counter;
    char message[100];
    bool ready;
};

int main() {
    std::cout << "=== P2: READER (Consumer) ===" << std::endl;
    
    // 1. Generate the SAME key as writer
    // MUST use same path and project ID!
    key_t key = ftok("/tmp", 65);
    if (key == -1) {
        perror("ftok failed");
        return 1;
    }
    std::cout << "[P2] Generated key: " << key << std::endl;
    
    // 2. Access existing shared memory segment
    // Note: NO IPC_CREAT flag (we're accessing, not creating)
    std::cout << "[P2] Waiting for shared memory to be created..." << std::endl;
    
    int shmid = -1;
    int retry_count = 0;
    const int MAX_RETRIES = 10;
    
    // Retry loop: wait for writer to create shared memory
    while (retry_count < MAX_RETRIES) {
        shmid = shmget(key, sizeof(SharedData), 0666);
        if (shmid != -1) {
            break;  // Found it!
        }
        std::cout << "[P2] Shared memory not found, retrying... (" 
                  << (retry_count + 1) << "/" << MAX_RETRIES << ")" << std::endl;
        sleep(1);
        retry_count++;
    }
    
    if (shmid == -1) {
        std::cerr << "[P2] ERROR: Shared memory not found. Make sure P1 is running!" << std::endl;
        return 1;
    }
    
    std::cout << "[P2] Found shared memory. ID: " << shmid << std::endl;
    
    // 3. Attach to shared memory
    void* shm_ptr = shmat(shmid, nullptr, 0);
    if (shm_ptr == (void*)-1) {
        perror("shmat failed");
        return 1;
    }
    std::cout << "[P2] Attached to shared memory at address: " << shm_ptr << std::endl;
    
    // 4. Cast to structure
    SharedData* data = static_cast<SharedData*>(shm_ptr);
    
    // 5. Read data from shared memory
    std::cout << "\n[P2] Starting to read data..." << std::endl;
    
    int messages_read = 0;
    const int EXPECTED_MESSAGES = 5;
    
    while (messages_read < EXPECTED_MESSAGES) {
        // Wait for data to be ready
        if (data->ready) {
            // Read data
            std::cout << "[P2] Read: counter=" << data->counter 
                      << ", message=\"" << data->message << "\"" << std::endl;
            
            messages_read++;
            
            // Signal that we consumed the data
            data->ready = false;
            std::cout << "[P2] Consumed data, waiting for next..." << std::endl;
        } else {
            // Data not ready yet, wait
            usleep(100000);  // Sleep 100ms
        }
    }
    
    std::cout << "\n[P2] Finished reading all messages." << std::endl;
    std::cout << "[P2] Total messages read: " << messages_read << std::endl;
    
    // 6. Detach from shared memory
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt failed");
        return 1;
    }
    std::cout << "[P2] Detached from shared memory" << std::endl;
    
    // Note: We don't delete (IPC_RMID) here - that's writer's job
    
    std::cout << "[P2] Process completed successfully" << std::endl;
    
    return 0;
}

/*
WORKFLOW EXPLANATION:

1. Writer (P1) starts:
   - Creates shared memory with ftok + shmget
   - Attaches with shmat
   - Writes data
   - Sets ready=true flag

2. Reader (P2) starts:
   - Uses SAME key (ftok with same parameters)
   - Accesses existing memory with shmget (no IPC_CREAT)
   - Attaches with shmat
   - Waits for ready=true
   - Reads data
   - Sets ready=false (signals consumed)

3. Synchronization:
   - ready flag: simple boolean synchronization
   - Writer waits while ready=true (reader hasn't consumed)
   - Reader waits while ready=false (writer hasn't produced)
   - For production: use semaphores for robust synchronization

4. Cleanup:
   - Both processes detach (shmdt)
   - Writer deletes segment (shmctl IPC_RMID)
   - If writer exits without cleanup, memory persists!
     Use: ipcs -m (list), ipcrm -m <shmid> (remove)

KEY DIFFERENCES FROM WRITER:
- No IPC_CREAT flag in shmget (access existing, don't create)
- Retry loop to wait for memory creation
- Doesn't delete memory (writer's responsibility)
- Consumer role: waits for ready=true, then reads
*/
