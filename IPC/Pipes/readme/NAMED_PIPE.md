# Named Pipe (FIFO) - Simple Guide

## What is a Named Pipe?

A **Named Pipe** (also called **FIFO**) is like a **mailbox**:
- Has a **name** (file path like `/tmp/my_pipe`)
- **Any process** can send/receive if they know the name
- **No need** for parent-child relationship

---

## Normal Pipe vs Named Pipe

| Feature | Normal Pipe | Named Pipe |
|---------|-------------|------------|
| Who can use? | Parent-child only | Any two processes |
| Has a name? | No | Yes (file path) |
| Persistent? | No | Yes (until deleted) |
| Visibility | Hidden | You can see it with `ls` |

---

## Key Functions

### 1. `mkfifo()` - Create Named Pipe
```cpp
mkfifo("/tmp/my_pipe", 0666);
```
- Creates a **special file** in filesystem
- `0666` = permissions (read+write for all)
- Like creating a mailbox

### 2. `open()` - Open the Pipe
```cpp
int fd = open("/tmp/my_pipe", O_WRONLY);  // Writer
int fd = open("/tmp/my_pipe", O_RDONLY);  // Reader
```
- `O_WRONLY` = Write Only (for sender)
- `O_RDONLY` = Read Only (for receiver)
- **BLOCKS** until other side opens!

### 3. `write()` - Send Data
```cpp
write(fd, message, length);
```
- Writer sends data through pipe

### 4. `read()` - Receive Data
```cpp
int bytes = read(fd, buffer, size);
```
- Reader receives data
- Returns 0 when writer closes
- **BLOCKS** if no data available

### 5. `close()` - Close Pipe
```cpp
close(fd);
```
- Closes your end of pipe

---

## Visual Flow

```
STEP 1: Writer creates named pipe
┌─────────┐
│ Writer  │  mkfifo("/tmp/my_pipe")
└─────────┘  
              Creates:  /tmp/my_pipe (special file)

STEP 2: Writer opens, Reader opens
┌─────────┐              ┌─────────┐
│ Writer  │  open(W)     │ Reader  │  open(R)
└─────────┘  ──────────► └─────────┘
              Connected!

STEP 3: Communication
┌─────────┐              ┌─────────┐
│ Writer  │              │ Reader  │
│ write() ├──► [FIFO] ───┤ read()  │
└─────────┘   /tmp/      └─────────┘
              my_pipe

STEP 4: Close
┌─────────┐              ┌─────────┐
│ Writer  │              │ Reader  │
│ close() │              │ close() │
└─────────┘              └─────────┘
```

---

## How to Run

### Terminal 1 - Start Reader First
```bash
cd /mnt/c/Users/z00542kh/Desktop/C++_KT/IPC/Pipes

# Compile reader
g++ -o reader_fifo reader_fifo.cpp

# Run reader (will wait for writer)
./reader_fifo
```

### Terminal 2 - Start Writer
```bash
cd /mnt/c/Users/z00542kh/Desktop/C++_KT/IPC/Pipes

# Compile writer
g++ -o writer_fifo writer_fifo.cpp

# Run writer
./writer_fifo
```

**OR Start Writer First:**
- Writer will wait for reader to connect
- Then both will communicate

---

## Complete Example Output

**Reader Output:**
```
=== NAMED PIPE READER ===
Looking for named pipe: /tmp/my_pipe
Opening pipe for reading...
✓ Pipe opened successfully!

⏳ Waiting for messages...

[READER] Message 1: "Hello from Writer!"
[READER] Message 2: "This is message 2"
[READER] Message 3: "Final message - Goodbye!"

[READER] Writer closed the pipe (no more data)
[READER] Closing pipe
✓ Done! Received 3 messages total.
```

**Writer Output:**
```
=== NAMED PIPE WRITER ===
Creating named pipe: /tmp/my_pipe
✓ Named pipe created successfully

Opening pipe for writing...
⏳ Waiting for reader to connect...
✓ Reader connected! Pipe is open.

[WRITER] Sending: "Hello from Writer!"

[WRITER] Sending: "This is message 2"

[WRITER] Sending: "Final message - Goodbye!"

[WRITER] Closing pipe
✓ Done!
```

---

## Key Concepts

### 1. **BLOCKING Behavior**
- `open()` waits until other side opens
- `read()` waits if no data available
- `write()` waits if buffer full

### 2. **The Named Pipe File**
You can see it in filesystem:
```bash
ls -l /tmp/my_pipe
prw-rw-rw- 1 user user 0 Nov 12 10:30 /tmp/my_pipe
```
Note the `p` at the start = **pipe file**

### 3. **Cleanup**
To remove the named pipe:
```bash
rm /tmp/my_pipe
```

---

## When to Use Named Pipes?

✅ **USE when:**
- Two unrelated processes need to communicate
- Simple one-way data flow
- Don't want to use sockets (named pipes are simpler)
- Small to medium data amounts

❌ **DON'T USE when:**
- Need network communication (use sockets)
- Very large data (use shared memory)
- Need bidirectional easily (use 2 named pipes or sockets)
- Need to communicate across machines (named pipes are local only)

---

## Simple Summary

1. **Writer** creates named pipe: `mkfifo("/tmp/my_pipe")`
2. **Writer** opens for writing: `open(..., O_WRONLY)`
3. **Reader** opens for reading: `open(..., O_RDONLY)`
4. They are now **connected**!
5. **Writer** sends: `write(fd, data, size)`
6. **Reader** receives: `read(fd, buffer, size)`
7. Both **close**: `close(fd)`

**That's it!** 🎉

---

## Comparison to Other IPC

| IPC Method | Complexity | Speed | Use Case |
|------------|-----------|-------|----------|
| Normal Pipe | Easy | Fast | Parent-child only |
| **Named Pipe** | **Easy** | **Fast** | **Any 2 processes** |
| Shared Memory | Hard | Fastest | Large data |
| Sockets | Medium | Medium | Network or local |
| Message Queue | Medium | Fast | Multiple processes |

---

## Common Issues

**Problem:** "Failed to open pipe"
- **Solution:** Make sure writer runs first to create pipe

**Problem:** "Permission denied"
- **Solution:** Check permissions with `ls -l /tmp/my_pipe`

**Problem:** Program hangs at `open()`
- **Solution:** This is normal! Waiting for other side to open

**Problem:** Named pipe file remains after program exits
- **Solution:** This is normal! Delete with `rm /tmp/my_pipe`

---

**Next:** Message Queues for more advanced IPC!
