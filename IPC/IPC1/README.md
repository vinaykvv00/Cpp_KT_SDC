# Shared Memory IPC Demo

This project demonstrates **Inter-Process Communication (IPC)** using **POSIX Shared Memory** in Linux/Unix.

## What is Shared Memory?

**Shared Memory** is a method of IPC where multiple processes can access the same region of memory. It's the **fastest** IPC method because:
- No kernel involvement after setup (direct memory access)
- No data copying (all processes see same memory)
- Direct read/write operations

## Project Structure

```
IPC1/
├── P1/                  # Writer Process (Producer)
│   ├── main.cpp        # Creates and writes to shared memory
│   └── CMakeLists.txt
├── P2/                  # Reader Process (Consumer)
│   ├── main.cpp        # Reads from shared memory
│   └── CMakeLists.txt
├── build_and_run.sh    # Build script
├── run_demo.sh         # Automated demo runner
└── README.md           # This file
```

## How It Works

### Writer (P1):
1. Creates shared memory segment using `shmget()`
2. Attaches to memory using `shmat()`
3. Writes data to shared structure
4. Sets `ready` flag to signal data available
5. Waits for reader to consume
6. Repeats 5 times
7. Cleans up and deletes shared memory

### Reader (P2):
1. Finds existing shared memory using same key
2. Attaches to memory using `shmat()`
3. Waits for `ready` flag
4. Reads data when available
5. Clears `ready` flag to signal consumed
6. Repeats until all messages read
7. Detaches from memory

### Synchronization:
- Simple boolean `ready` flag
- Writer waits while `ready=true` (reader hasn't consumed)
- Reader waits while `ready=false` (no new data)
- **Note**: For production, use semaphores for robust synchronization

## Building

```bash
chmod +x build_and_run.sh
./build_and_run.sh
```

This will:
- Build P1 (writer) → `P1/build/writer`
- Build P2 (reader) → `P2/build/reader`

## Running

### Option 1: Automated Demo
```bash
chmod +x run_demo.sh
./run_demo.sh
```

### Option 2: Manual (Two Terminals)

**Terminal 1 (Writer):**
```bash
./P1/build/writer
```

**Terminal 2 (Reader):**
```bash
./P2/build/reader
```

### Option 3: Background & Foreground
```bash
./P1/build/writer &    # Start writer in background
./P2/build/reader      # Start reader in foreground
```

## Expected Output

### Writer (P1):
```
=== P1: WRITER (Producer) ===
[P1] Generated key: 1090650465
[P1] Shared memory created. ID: 32769
[P1] Attached to shared memory at address: 0x7f8b4c000000
[P1] Initialized shared memory

[P1] Starting to write data...
[P1] Wrote: counter=1, message="Message #1 from Writer Process (PID: 12345)"
[P1] Data consumed by reader, writing next...
[P1] Wrote: counter=2, message="Message #2 from Writer Process (PID: 12345)"
...
```

### Reader (P2):
```
=== P2: READER (Consumer) ===
[P2] Generated key: 1090650465
[P2] Found shared memory. ID: 32769
[P2] Attached to shared memory at address: 0x7f8b4c000000

[P2] Starting to read data...
[P2] Read: counter=1, message="Message #1 from Writer Process (PID: 12345)"
[P2] Consumed data, waiting for next...
[P2] Read: counter=2, message="Message #2 from Writer Process (PID: 12345)"
...
[P2] Total messages read: 5
```

## Key System Calls

| Function | Purpose |
|----------|---------|
| `ftok()` | Generate unique key from file path |
| `shmget()` | Create or access shared memory segment |
| `shmat()` | Attach shared memory to process address space |
| `shmdt()` | Detach shared memory from process |
| `shmctl()` | Control shared memory (delete, get info) |

## Shared Memory Lifecycle

```
1. Create:   shmget(key, size, IPC_CREAT | 0666)
2. Attach:   shmat(shmid, nullptr, 0)
3. Use:      Read/write to memory directly
4. Detach:   shmdt(shmaddr)
5. Delete:   shmctl(shmid, IPC_RMID, nullptr)
```

## Useful Commands

### List shared memory segments:
```bash
ipcs -m
```

### Remove specific segment:
```bash
ipcrm -m <shmid>
```

### Remove all your segments:
```bash
ipcs -m | grep $(whoami) | awk '{print $2}' | xargs -I {} ipcrm -m {}
```

### Monitor in real-time:
```bash
watch -n 1 'ipcs -m'
```

## Troubleshooting

### "Shared memory not found"
- Make sure writer (P1) is running first
- Check: `ipcs -m` to see if segment exists

### "Permission denied"
- Check permissions in `shmget()` (should be 0666)
- Make sure both processes use same user

### Segment persists after crash
- Shared memory survives process termination!
- Clean up manually: `ipcrm -m <shmid>`

### Key already exists error
- Another segment using same key
- Change project ID in `ftok()` call
- Or remove old segment: `ipcrm -m <shmid>`

## Advantages of Shared Memory

✅ **Fastest IPC method** (no copying, direct access)  
✅ **Efficient** for large data transfers  
✅ **Persistent** (survives process crashes)  
✅ **Multiple processes** can access simultaneously  

## Disadvantages

❌ **Requires synchronization** (race conditions possible)  
❌ **No built-in signaling** (need semaphores/flags)  
❌ **Manual cleanup** (persists if process crashes)  
❌ **Fixed size** (can't resize after creation)  
❌ **Platform-specific** (POSIX on Linux/Unix)  

## Comparison with Other IPC Methods

| Method | Speed | Data Size | Complexity | Sync Built-in |
|--------|-------|-----------|------------|---------------|
| **Shared Memory** | Fastest | Large | Medium | No (manual) |
| Pipes | Fast | Small-Medium | Easy | Yes |
| Message Queues | Medium | Medium | Easy | Yes |
| Sockets | Slow | Any | Complex | Yes |

## When to Use Shared Memory

✅ **Large data transfers** (images, videos, datasets)  
✅ **High-frequency communication** (real-time systems)  
✅ **Multiple processes** reading same data  
✅ **Performance critical** applications  

❌ **Simple messaging** (use pipes or message queues)  
❌ **Network communication** (use sockets)  
❌ **Small data** (overhead not worth it)  

## Improvements for Production

1. **Add Semaphores**: Replace boolean flag with proper semaphores
2. **Error Handling**: Add robust error checking
3. **Signal Handlers**: Clean up on SIGINT/SIGTERM
4. **Multiple Readers**: Use read/write locks
5. **Circular Buffer**: For continuous streaming
6. **Version Control**: Add structure version field

## Learning Resources

- Man pages: `man shmget`, `man shmat`, `man shmctl`
- POSIX IPC: [The Linux Programming Interface](https://man7.org/tlpi/)
- Advanced: [Boost.Interprocess](https://www.boost.org/doc/libs/release/doc/html/interprocess.html)

## License

Public domain - use freely for learning.
