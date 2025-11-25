# Pipes - Simple IPC (Inter-Process Communication)

## What is a Pipe?

A **pipe** is like a **tube** that connects two processes for one-way communication.

```
Process A  ──► [PIPE] ──►  Process B
(Writer)       Kernel      (Reader)
               Buffer
```

---

## Key Concepts

### 1. **Creating a Pipe**
```cpp
int pipefd[2];
pipe(pipefd);  // Creates 2 file descriptors
```

- `pipefd[0]` = **Read end** (input)
- `pipefd[1]` = **Write end** (output)

### 2. **fork() - Creating Child Process**
```cpp
pid_t pid = fork();
if (pid == 0) {
    // Child process
} else {
    // Parent process
}
```

### 3. **Close Unused Ends**
**IMPORTANT:** Each process should close the end it doesn't use!

```cpp
// Parent (writer)
close(pipefd[0]);  // Close read end

// Child (reader)
close(pipefd[1]);  // Close write end
```

### 4. **Communication**
```cpp
// Parent writes
write(pipefd[1], message, length);

// Child reads
read(pipefd[0], buffer, size);
```

---

## Files in This Folder

| File | Description |
|------|-------------|
| `simple_pipe.cpp` | Basic one-way pipe (Parent → Child) |
| `bidirectional_pipe.cpp` | Two-way communication (2 pipes) |

---

## Compile and Run

```bash
# Simple pipe
g++ -o simple simple_pipe.cpp
./simple

# Bidirectional pipe
g++ -o bidirectional bidirectional_pipe.cpp
./bidirectional
```

---

## Visual Explanation

### Simple Pipe (One-Way)
```
┌─────────┐              ┌─────────┐
│ Parent  │              │  Child  │
│         │              │         │
│ fd[1]───┼──► [PIPE] ───┼──►fd[0] │
│ (write) │   (kernel)   │ (read)  │
└─────────┘              └─────────┘
```

### Bidirectional (Two Pipes)
```
┌─────────┐              ┌─────────┐
│         │──pipe1[1]──► │         │
│ PARENT  │              │  CHILD  │
│         │◄─pipe2[0]────│         │
└─────────┘              └─────────┘
```

---

## Key Functions

| Function | Purpose |
|----------|---------|
| `pipe(fd)` | Create a pipe (returns 2 file descriptors) |
| `fork()` | Create child process (returns PID) |
| `write(fd, data, size)` | Write data to pipe |
| `read(fd, buffer, size)` | Read data from pipe |
| `close(fd)` | Close file descriptor |
| `wait(NULL)` | Parent waits for child to finish |

---

## Advantages ✓

- **Simple**: Easy to use
- **Fast**: Direct kernel buffer
- **Safe**: OS-managed, isolated processes
- **Blocking**: Automatic synchronization

---

## Limitations ✗

- **One-way**: Need 2 pipes for bidirectional
- **Related processes only**: Parent-child, siblings
- **Temporary**: Lost when processes exit
- **Limited size**: Kernel buffer (~64KB)

---

## Comparison: Pipe vs Shared Memory

| Feature | Pipe | Shared Memory |
|---------|------|---------------|
| Direction | One-way (unidirectional) | Both read/write |
| Setup | Easy (`pipe()`) | Complex (ftok, shmget, shmat) |
| Speed | Fast | Faster (direct memory) |
| Size | Limited (~64KB) | Large (MB/GB) |
| Processes | Parent-child only | Any processes |
| Persistence | Lost on exit | Can persist |
| Sync | Automatic (blocking) | Manual (semaphores needed) |

---

## When to Use Pipes?

✓ Simple parent-child communication  
✓ One-way data flow  
✓ Shell-like command chaining  
✓ Small amounts of data  
✓ Need automatic synchronization  

**Example:** `ls | grep .txt` (shell uses pipes!)

---

## Real-World Examples

1. **Shell Commands**
   ```bash
   ps aux | grep firefox | wc -l
   ```
   Each `|` creates a pipe!

2. **Web Server**
   ```
   Master Process ──pipe──► Worker Process
   (receives requests)      (processes requests)
   ```

3. **Logging System**
   ```
   Application ──pipe──► Logger Process
   (generates logs)        (writes to file)
   ```

---

## Quick Reference

```cpp
// Create pipe
int fd[2];
pipe(fd);

// Fork
pid_t pid = fork();

if (pid == 0) {
    // Child: read from pipe
    close(fd[1]);        // Close write end
    read(fd[0], buf, 100);
    close(fd[0]);
} else {
    // Parent: write to pipe
    close(fd[0]);        // Close read end
    write(fd[1], "Hi", 2);
    close(fd[1]);
    wait(NULL);          // Wait for child
}
```

---

**Next:** Named Pipes (FIFOs) for unrelated processes!
