# Message Queue - Function & Structure Explanation

## Structure Definition

### `struct msg_buffer`
```cpp
struct msg_buffer {
    long msg_type;      // Message type (must be > 0)
    char msg_text[MAX]; // Message content
};
```

**Fields:**
- `msg_type` - Message type identifier (must be positive number)
- `msg_text` - Array to store message data (size = MAX)

---

## Functions Explained

### 1. `ftok()` - Generate Key

**Syntax:**
```cpp
key_t ftok(const char *pathname, int proj_id);
```

**Parameters:**
- `pathname` - File path (must exist, e.g., "progfile")
- `proj_id` - Project identifier (any number, e.g., 65)

**Returns:**
- `key_t` - Unique key for message queue
- `-1` on error

**Example:**
```cpp
key = ftok("progfile", 65);
```

**Purpose:** Creates unique key from file path and ID

---

### 2. `msgget()` - Create/Access Message Queue

**Syntax:**
```cpp
int msgget(key_t key, int msgflg);
```

**Parameters:**
- `key` - Key from ftok()
- `msgflg` - Flags:
  - `IPC_CREAT` - Create if doesn't exist
  - `0666` - Permissions (read+write for all)

**Returns:**
- Message queue ID (positive number)
- `-1` on error

**Example:**
```cpp
msgid = msgget(key, 0666 | IPC_CREAT);
```

**Purpose:** Creates or accesses existing message queue

---

### 3. `msgsnd()` - Send Message

**Syntax:**
```cpp
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
```

**Parameters:**
- `msqid` - Message queue ID from msgget()
- `msgp` - Pointer to message structure
- `msgsz` - Size of message (use sizeof(message))
- `msgflg` - Flags (0 = wait if queue full)

**Returns:**
- `0` on success
- `-1` on error

**Example:**
```cpp
msgsnd(msgid, &message, sizeof(message), 0);
```

**Purpose:** Sends message to queue

---

### 4. `msgrcv()` - Receive Message

**Syntax:**
```cpp
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
```

**Parameters:**
- `msqid` - Message queue ID
- `msgp` - Pointer to receive message
- `msgsz` - Maximum message size
- `msgtyp` - Message type to receive:
  - `0` - First message in queue
  - `> 0` - First message of that type
  - `< 0` - First message with lowest type
- `msgflg` - Flags (0 = wait if queue empty)

**Returns:**
- Number of bytes received
- `-1` on error

**Example:**
```cpp
msgrcv(msgid, &message, sizeof(message), 1, 0);
```

**Purpose:** Receives message from queue

---

### 5. `msgctl()` - Control Message Queue

**Syntax:**
```cpp
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```

**Parameters:**
- `msqid` - Message queue ID
- `cmd` - Command:
  - `IPC_RMID` - Delete message queue
  - `IPC_STAT` - Get queue info
  - `IPC_SET` - Set queue attributes
- `buf` - Buffer for data (NULL if not needed)

**Returns:**
- `0` on success
- `-1` on error

**Example:**
```cpp
msgctl(msgid, IPC_RMID, NULL);
```

**Purpose:** Controls/deletes message queue

---

## Writer.cpp Syntax Breakdown

```cpp
#include <iostream>     // For std::cout, std::cin
#include <sys/ipc.h>    // For ftok(), key_t
#include <sys/msg.h>    // For msgget(), msgsnd()
#include <cstring>      // For string operations
#define MAX 10          // Maximum message size

struct msg_buffer {
    long msg_type;      // Message type (1 in our case)
    char msg_text[MAX]; // Message content
} message;

int main()
{
    key_t key;          // Key for message queue
    int msgid;          // Message queue ID

    // STEP 1: Generate unique key
    key = ftok("progfile", 65);
    
    // STEP 2: Create message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    // STEP 3: Set message type
    message.msg_type = 1;

    // STEP 4: Get input from user
    std::cout << "Write Data : ";
    std::cin.getline(message.msg_text, MAX);

    // STEP 5: Send message to queue
    msgsnd(msgid, &message, sizeof(message), 0);

    // STEP 6: Confirm
    std::cout << "Data send is : " << message.msg_text << std::endl;

    return 0;
}
```

---

## Reader.cpp Syntax Breakdown

```cpp
#include <iostream>     // For std::cout
#include <sys/ipc.h>    // For ftok(), key_t
#include <sys/msg.h>    // For msgget(), msgrcv(), msgctl()
#define MAX 10          // Maximum message size

struct msg_buffer {
    long msg_type;      // Message type
    char msg_text[MAX]; // Message content
} message;

int main()
{
    key_t key;          // Key for message queue
    int msgid;          // Message queue ID

    // STEP 1: Generate same key as writer
    key = ftok("progfile", 65);
    
    // STEP 2: Access existing message queue
    msgid = msgget(key, 0666 | IPC_CREAT);

    // STEP 3: Receive message (type 1)
    msgrcv(msgid, &message, sizeof(message), 1, 0);

    // STEP 4: Display received message
    std::cout << "Data Received is : " << message.msg_text << std::endl;

    // STEP 5: Delete message queue
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
```

---

## Quick Reference Table

| Function | Purpose | Writer Uses? | Reader Uses? |
|----------|---------|--------------|--------------|
| `ftok()` | Generate key | ✓ | ✓ |
| `msgget()` | Create/access queue | ✓ | ✓ |
| `msgsnd()` | Send message | ✓ | ✗ |
| `msgrcv()` | Receive message | ✗ | ✓ |
| `msgctl()` | Delete queue | ✗ | ✓ |

---

## Compile and Run

```bash
# Compile
g++ -o writer Writer.cpp
g++ -o reader Reader.cpp

# Run writer first
./writer

# Run reader in another terminal
./reader
```

---

## Important Notes

1. **Same key required:** Both programs must use same file and ID in ftok()
2. **Message type:** Must be > 0, we use 1
3. **File must exist:** "progfile" must exist for ftok() to work
4. **Cleanup:** Reader deletes queue with msgctl()
