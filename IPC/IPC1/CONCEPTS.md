# Shared Memory Concept Explained

## Visual Representation

```
┌──────────────────────────────────────────────────────────────┐
│                     PHYSICAL MEMORY                          │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  Process 1 (Writer)          SHARED MEMORY         Process 2 (Reader)
│  ┌──────────┐                ┌──────────┐          ┌──────────┐
│  │  Code    │                │ counter  │          │  Code    │
│  ├──────────┤                ├──────────┤          ├──────────┤
│  │  Data    │                │ message  │          │  Data    │
│  ├──────────┤   ┌────────►  ├──────────┤  ◄────┐  ├──────────┤
│  │  Stack   │   │            │  ready   │       │  │  Stack   │
│  ├──────────┤   │            └──────────┘       │  ├──────────┤
│  │  Heap    │   │                               │  │  Heap    │
│  ├──────────┤   │                               │  ├──────────┤
│  │ Attached │───┘      SAME MEMORY REGION       └──│ Attached │
│  │  SHM     │               (SHARED)               │  SHM     │
│  └──────────┘                                      └──────────┘
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

## How It Works Step-by-Step

### Phase 1: Setup

```
Writer (P1):
1. ftok("/tmp", 65) → Key: 1093672961
2. shmget(key, size, IPC_CREAT) → Creates segment, ID: 0
3. shmat(0, NULL, 0) → Attaches at 0x70f15b0f4000
4. Now can read/write at that address

Reader (P2):
1. ftok("/tmp", 65) → Key: 1093672961 (SAME!)
2. shmget(key, size, 0) → Finds existing segment, ID: 0
3. shmat(0, NULL, 0) → Attaches at 0x7f80f7c5c000 (different virtual address)
4. Now can read/write SAME physical memory
```

### Phase 2: Communication

```
Time    Writer (P1)                     SHARED MEMORY              Reader (P2)
----    -----------                     -------------              -----------
t0      data->counter = 1               counter: 1                 (waiting)
        data->message = "Message #1"    message: "Message #1"
        data->ready = true              ready: true

t1                                      counter: 1                 if (data->ready)
                                        message: "Message #1"        read data
                                        ready: true                  print message

t2      while(data->ready) wait...      counter: 1                 data->ready = false
        (blocked)                       message: "Message #1"
                                        ready: false ◄──────────────┘

t3      wake up, ready=false            counter: 2                 (waiting for next)
        data->counter = 2               message: "Message #2"
        data->message = "Message #2"    ready: true
        data->ready = true

t4                                      counter: 2                 if (data->ready)
                                        message: "Message #2"        read data
                                        ready: true                  print message
                                        
... repeat ...
```

## Memory Layout

### Shared Structure in Memory

```
Address Offset    Content              Size        Value (Example)
--------------    -------              ----        ---------------
+0x00             counter (int)        4 bytes     0x00000003 (3)
+0x04             message[100]         100 bytes   "Message #3 from Writer..."
+0x68             ready (bool)         1 byte      0x01 (true)
+0x69             padding              3 bytes     (alignment)
Total Size:       108 bytes
```

## Key Differences: Virtual vs Physical Memory

```
Virtual Memory (Process View):
┌─────────────┐                    ┌─────────────┐
│   Writer    │                    │   Reader    │
│  Address:   │                    │  Address:   │
│ 0x70f15...  │                    │ 0x7f80f7... │
└──────┬──────┘                    └──────┬──────┘
       │                                  │
       │    ┌──────────────────┐         │
       └───►│ Physical Memory  │◄────────┘
            │   (ONE COPY)     │
            │  Shared Region   │
            └──────────────────┘

Both processes see DIFFERENT virtual addresses
BUT they point to the SAME physical memory!
```

## System Calls Deep Dive

### 1. ftok() - Generate Key

```cpp
key_t key = ftok("/tmp", 65);
```

**What it does:**
- Combines: file's inode number + project ID (65 = 'A')
- Result: Unique integer key
- **IMPORTANT**: Same file + project ID = same key on all processes

**Why /tmp?**
- Guaranteed to exist on all Unix systems
- Could use any accessible file

### 2. shmget() - Create/Access Memory

```cpp
// Writer: CREATE
int shmid = shmget(key, sizeof(SharedData), IPC_CREAT | 0666);

// Reader: ACCESS (no IPC_CREAT)
int shmid = shmget(key, sizeof(SharedData), 0666);
```

**Flags:**
- `IPC_CREAT`: Create if doesn't exist
- `0666`: Permissions (rw-rw-rw-)
- Returns: Shared memory ID (integer handle)

### 3. shmat() - Attach to Process

```cpp
void* ptr = shmat(shmid, nullptr, 0);
```

**What it does:**
- Maps shared memory into process virtual address space
- `nullptr`: Let OS choose address
- `0`: Read-write access
- Returns: Pointer to access the memory

### 4. shmdt() - Detach

```cpp
shmdt(ptr);
```

**What it does:**
- Unmaps shared memory from process address space
- Process can no longer access that memory
- **Does NOT delete** the segment (still exists for other processes)

### 5. shmctl() - Control/Delete

```cpp
shmctl(shmid, IPC_RMID, nullptr);
```

**Commands:**
- `IPC_RMID`: Mark for deletion (deleted when all processes detach)
- `IPC_STAT`: Get segment info
- `IPC_SET`: Set segment properties

## Synchronization Mechanism

### Simple Flag-Based (Current Implementation)

```cpp
// Writer side:
data->counter = 1;
data->ready = true;        // Signal: data is ready
while (data->ready) {      // Wait for reader to consume
    usleep(100000);
}

// Reader side:
while (!data->ready) {     // Wait for data
    usleep(100000);
}
int val = data->counter;   // Read data
data->ready = false;       // Signal: consumed
```

**Problems with this approach:**
- ❌ Busy-waiting (wastes CPU)
- ❌ Race conditions possible
- ❌ No atomicity guarantee

### Better: Using Semaphores (Production)

```cpp
// Shared memory:
struct SharedData {
    sem_t writer_sem;      // Semaphore for writer
    sem_t reader_sem;      // Semaphore for reader
    int data;
};

// Writer:
sem_post(&data->reader_sem);  // Signal reader
sem_wait(&data->writer_sem);  // Wait for reader to consume

// Reader:
sem_wait(&data->reader_sem);  // Wait for data
sem_post(&data->writer_sem);  // Signal consumed
```

## Performance Comparison

### Shared Memory vs Other IPC

```
Data Transfer Time for 1MB:

Shared Memory:    ~0.01 ms  (just pointer copy!)
Pipes:            ~2-5 ms   (kernel buffer copy)
Message Queue:    ~3-6 ms   (kernel copy + queue overhead)
Sockets (local):  ~5-10 ms  (protocol stack overhead)
Sockets (TCP):    ~50+ ms   (network stack)

Shared memory is 100-1000x faster!
```

### Why So Fast?

```
Shared Memory:
  Writer ──► Physical Memory ◄── Reader
             (NO COPY!)

Other IPC:
  Writer ──► Kernel Buffer ──► Reader
        (COPY 1)      (COPY 2)
```

## Real-World Use Cases

### 1. Video Processing
```
Camera Process → Shared Memory ← Display Process
                    ↑
                Encoder Process
(All read same frame, no copying!)
```

### 2. Database Systems
```
Multiple Query Processes → Shared Buffer Pool ← Disk Manager
(All share cache, avoid redundant disk reads)
```

### 3. Game Engines
```
Rendering Thread → Shared Scene Data ← Physics Thread
                       ↑
                   Audio Thread
(All access game state efficiently)
```

## Common Pitfalls

### 1. Forgetting to Clean Up
```cpp
// BAD: Process crashes before shmctl()
shmat();
// ... work ...
// CRASH! Memory persists forever!

// Check with: ipcs -m
// Clean up: ipcrm -m <shmid>
```

### 2. Structure Mismatch
```cpp
// Writer:
struct Data { int x; double y; };

// Reader (WRONG!):
struct Data { double y; int x; };  // Different order!
// Result: Garbage data
```

### 3. Size Mismatch
```cpp
// Writer: creates 100 bytes
shmget(key, 100, IPC_CREAT);

// Reader: tries to access 200 bytes
shmget(key, 200, 0);  // Works, but writes beyond 100 = crash!
```

### 4. No Synchronization
```cpp
// Writer & Reader access simultaneously without sync
// Result: Race condition, corrupted data
```

## Best Practices

✅ **Always use same structure** in all processes  
✅ **Add version field** to detect structure changes  
✅ **Use proper synchronization** (semaphores)  
✅ **Handle cleanup in signal handlers** (SIGINT, SIGTERM)  
✅ **Check return values** of all system calls  
✅ **Use fixed-size types** (`int32_t` not `int`)  
✅ **Document the protocol** between processes  

## Summary

**Shared Memory = Direct Memory Access Between Processes**

- ✅ Fastest IPC (no copying)
- ✅ Simple concept (just pointers)
- ⚠️ Requires synchronization
- ⚠️ Manual cleanup needed
- 🎯 Best for: Large data, high-frequency communication
