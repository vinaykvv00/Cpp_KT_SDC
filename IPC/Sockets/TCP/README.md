# Socket Programming - Server & Client Explanation

## What Was Wrong?

### Original Issues:
1. **Client.cpp had syntax error** - Random `\` character on line 16
2. **No error checking** - Hard to debug failures
3. **Wrong IP address** - Used `INADDR_ANY` in client (should use specific IP)
4. **Missing socket close** - Server didn't close client socket
5. **No output messages** - Hard to see what's happening

### Fixed:
✅ Removed syntax error  
✅ Added error checking for all socket operations  
✅ Changed client to use `127.0.0.1` (localhost)  
✅ Added proper socket cleanup  
✅ Added status messages at each step  

---

## How to Run (CORRECT ORDER)

### Terminal 1 - Start Server First:
```bash
cd /mnt/c/Users/z00542kh/Desktop/C++_KT/IPC/Sockets
g++ -o server Server.cpp
./server
```

**Expected Output:**
```
Server socket created successfully
Socket bound to port 8080
Server listening on port 8080...
(waiting for client...)
```

### Terminal 2 - Then Start Client:
```bash
cd /mnt/c/Users/z00542kh/Desktop/C++_KT/IPC/Sockets
g++ -o client Client.cpp
./client
```

**Expected Output:**
```
Client socket created successfully
Connected to server successfully!
Message sent: Hello, server!
Connection closed
```

### Terminal 1 - Server Shows:
```
Client connected!
Message from client: Hello, server!
Server closed
```

---

## Socket Functions Explained

### 1. `socket()` - Create Socket

**Syntax:**
```cpp
int socket(int domain, int type, int protocol);
```

**Parameters:**
- `AF_INET` - IPv4 Internet protocols
- `SOCK_STREAM` - TCP (reliable, connection-based)
- `0` - Automatically choose protocol (TCP for SOCK_STREAM)

**Returns:**
- Socket file descriptor (positive number)
- `-1` on error

**Example:**
```cpp
int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
```

**What it does:** Creates an endpoint for communication

---

### 2. `bind()` - Bind Socket to Address (SERVER ONLY)

**Syntax:**
```cpp
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

**Parameters:**
- `sockfd` - Socket file descriptor from socket()
- `addr` - Pointer to sockaddr_in structure (address info)
- `addrlen` - Size of address structure

**Returns:**
- `0` on success
- `-1` on error

**Example:**
```cpp
bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
```

**What it does:** Assigns IP address and port to the socket

---

### 3. `listen()` - Listen for Connections (SERVER ONLY)

**Syntax:**
```cpp
int listen(int sockfd, int backlog);
```

**Parameters:**
- `sockfd` - Socket file descriptor
- `backlog` - Maximum number of pending connections (e.g., 5)

**Returns:**
- `0` on success
- `-1` on error

**Example:**
```cpp
listen(serverSocket, 5);
```

**What it does:** Marks socket as passive (ready to accept connections)

---

### 4. `accept()` - Accept Connection (SERVER ONLY)

**Syntax:**
```cpp
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

**Parameters:**
- `sockfd` - Listening socket
- `addr` - Client address info (can be nullptr)
- `addrlen` - Size of address (can be nullptr)

**Returns:**
- New socket descriptor for accepted connection
- `-1` on error

**Example:**
```cpp
int clientSocket = accept(serverSocket, nullptr, nullptr);
```

**What it does:** **BLOCKS** until client connects, then returns new socket

---

### 5. `connect()` - Connect to Server (CLIENT ONLY)

**Syntax:**
```cpp
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

**Parameters:**
- `sockfd` - Client socket
- `addr` - Server address to connect to
- `addrlen` - Size of address structure

**Returns:**
- `0` on success
- `-1` on error

**Example:**
```cpp
connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
```

**What it does:** Establishes connection to server

---

### 6. `send()` - Send Data (CLIENT)

**Syntax:**
```cpp
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

**Parameters:**
- `sockfd` - Socket descriptor
- `buf` - Pointer to data to send
- `len` - Length of data
- `flags` - Options (0 = none)

**Returns:**
- Number of bytes sent
- `-1` on error

**Example:**
```cpp
send(clientSocket, message, strlen(message), 0);
```

**What it does:** Sends data through socket

---

### 7. `recv()` - Receive Data (SERVER)

**Syntax:**
```cpp
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

**Parameters:**
- `sockfd` - Socket descriptor
- `buf` - Buffer to store received data
- `len` - Maximum bytes to receive
- `flags` - Options (0 = none)

**Returns:**
- Number of bytes received
- `0` if connection closed
- `-1` on error

**Example:**
```cpp
recv(clientSocket, buffer, sizeof(buffer), 0);
```

**What it does:** Receives data from socket

---

### 8. `close()` - Close Socket

**Syntax:**
```cpp
int close(int fd);
```

**Parameters:**
- `fd` - File descriptor to close

**Returns:**
- `0` on success
- `-1` on error

**Example:**
```cpp
close(clientSocket);
close(serverSocket);
```

**What it does:** Closes socket and frees resources

---

## Structure: `sockaddr_in`

```cpp
struct sockaddr_in {
    short sin_family;        // Address family (AF_INET)
    unsigned short sin_port; // Port number
    struct in_addr sin_addr; // IP address
    char sin_zero[8];        // Padding
};
```

**Example Configuration:**
```cpp
sockaddr_in serverAddress;
serverAddress.sin_family = AF_INET;           // IPv4
serverAddress.sin_port = htons(8080);         // Port 8080 (network byte order)
serverAddress.sin_addr.s_addr = INADDR_ANY;   // Any available interface (server)
// OR
serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Specific IP (client)
```

**Fields Explained:**
- `sin_family` - Protocol family (always `AF_INET` for IPv4)
- `sin_port` - Port number (use `htons()` to convert to network byte order)
- `sin_addr.s_addr` - IP address:
  - `INADDR_ANY` (0.0.0.0) - Server: accept on any network interface
  - `inet_addr("127.0.0.1")` - Client: connect to specific IP
- `sin_zero` - Padding to match size of `sockaddr`

---

## Helper Functions

### `htons()` - Host to Network Short

**Purpose:** Converts port number from host byte order to network byte order

**Example:**
```cpp
serverAddress.sin_port = htons(8080);
```

**Why needed:** Different computers store numbers differently (big-endian vs little-endian)

---

### `inet_addr()` - Convert IP String to Binary

**Purpose:** Converts IP address string to binary format

**Example:**
```cpp
serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
```

---

## Server.cpp - Line by Line

```cpp
#include <sys/socket.h>   // socket(), bind(), listen(), accept(), send(), recv()
#include <netinet/in.h>   // sockaddr_in, INADDR_ANY
#include <unistd.h>       // close()

int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
// Creates TCP socket, returns file descriptor (e.g., 3)

sockaddr_in serverAddress;
serverAddress.sin_family = AF_INET;
// Set protocol to IPv4

serverAddress.sin_port = htons(8080);
// Set port to 8080, convert to network byte order

serverAddress.sin_addr.s_addr = INADDR_ANY;
// Accept connections on ANY network interface (0.0.0.0)

bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
// Bind socket to IP:PORT (0.0.0.0:8080)

listen(serverSocket, 5);
// Mark socket as passive, allow up to 5 pending connections

int clientSocket = accept(serverSocket, nullptr, nullptr);
// BLOCKS here until client connects
// Returns NEW socket for this specific client

char buffer[1024] = { 0 };
recv(clientSocket, buffer, sizeof(buffer), 0);
// Receives data from client into buffer
// BLOCKS until data arrives

cout << "Message from client: " << buffer << endl;
// Prints received message

close(clientSocket);  // Close client connection
close(serverSocket);  // Close server socket
```

---

## Client.cpp - Line by Line

```cpp
#include <sys/socket.h>   // socket(), connect(), send()
#include <netinet/in.h>   // sockaddr_in
#include <arpa/inet.h>    // inet_addr()
#include <unistd.h>       // close()

int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
// Creates TCP socket for client

sockaddr_in serverAddress;
serverAddress.sin_family = AF_INET;
// Set protocol to IPv4

serverAddress.sin_port = htons(8080);
// Server port 8080

serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
// Connect to localhost (127.0.0.1)
// NOTE: Use specific IP, NOT INADDR_ANY!

connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
// Establishes connection to server
// BLOCKS until connection succeeds or fails

const char* message = "Hello, server!";
send(clientSocket, message, strlen(message), 0);
// Sends message to server

close(clientSocket);
// Close connection
```

---

## Execution Flow Visualization

```
SERVER                          CLIENT
======                          ======

socket()                        
  ↓                             
bind() to port 8080             
  ↓                             
listen()                        
  ↓                             
accept() ⏳ WAITING...          socket()
  ↓                               ↓
  ↓                             connect() → Connects to server
  ↓ ← Connection established ← 
Client connected!                 ↓
  ↓                             send("Hello, server!")
recv() ⏳ WAITING...              ↓
  ↓ ← Data arrives ←            close()
"Hello, server!" received       
  ↓
close(clientSocket)
close(serverSocket)

DONE                            DONE
```

---

## Common Issues & Solutions

### Issue: "Address already in use"
**Cause:** Port 8080 still occupied from previous run  
**Solution:**
```bash
# Find process using port 8080
sudo lsof -i :8080
# Kill the process
kill -9 <PID>
```

### Issue: "Connection refused"
**Cause:** Server not running  
**Solution:** Start server BEFORE client

### Issue: Client connects but browser shows HTTP request
**Cause:** Opening `localhost:8080` in browser  
**Solution:** Use the ./client program, not web browser

### Issue: Server exits immediately
**Cause:** Server runs once then quits (current design)  
**Solution:** Add while loop to handle multiple clients (see below)

---

## Enhanced Server (Multiple Clients)

```cpp
while (true) {
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    cout << "Client connected!" << endl;
    
    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Message: " << buffer << endl;
    
    close(clientSocket);
}
```

---

## Summary Table

| Function | Used By | Purpose | Blocks? |
|----------|---------|---------|---------|
| `socket()` | Both | Create socket | No |
| `bind()` | Server | Assign address | No |
| `listen()` | Server | Mark as passive | No |
| `accept()` | Server | Accept connection | **Yes** |
| `connect()` | Client | Connect to server | **Yes** |
| `send()` | Client | Send data | Usually no |
| `recv()` | Server | Receive data | **Yes** |
| `close()` | Both | Close socket | No |

---

## Key Points

✅ Always start **server first**, then client  
✅ Server uses `INADDR_ANY`, client uses specific IP  
✅ `accept()` and `recv()` **BLOCK** (wait)  
✅ Always check return values for errors  
✅ Close both client and server sockets  
✅ Port must be free (not used by another program)  

---

## Quick Test

**Terminal 1:**
```bash
./server
```

**Terminal 2:**
```bash
./client
```

**Expected Result:**
Server prints: "Message from client: Hello, server!"
