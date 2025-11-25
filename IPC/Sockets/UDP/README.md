# UDP Socket Programming - Complete Guide

## TCP vs UDP - Key Differences

### Visual Comparison:

```
TCP (Connection-Oriented)          UDP (Connectionless)
========================          ======================

Server                            Server
  ↓                                 ↓
socket(SOCK_STREAM)               socket(SOCK_DGRAM)
  ↓                                 ↓
bind()                            bind()
  ↓                                 ↓
listen() ← Connection setup       (NO listen!)
  ↓                                 ↓
accept() ⏳ Wait for client       recvfrom() ⏳ Wait for datagram
  ↓                                 ↓
Connection established            Direct receive!
  ↓                                 ↓
recv()                            sendto() (response)
  ↓
close()                           close()


Client                            Client
  ↓                                 ↓
socket(SOCK_STREAM)               socket(SOCK_DGRAM)
  ↓                                 ↓
connect() ← Establish connection  (NO connect!)
  ↓                                 ↓
Connected!                        sendto() (direct!)
  ↓                                 ↓
send()                            recvfrom() (response)
  ↓
close()                           close()
```

---

## Main Differences Table

| Feature | TCP | UDP |
|---------|-----|-----|
| **Socket Type** | `SOCK_STREAM` | `SOCK_DGRAM` |
| **Connection** | Yes (must establish) | No (connectionless) |
| **Server Setup** | `socket()` → `bind()` → `listen()` → `accept()` | `socket()` → `bind()` → `recvfrom()` |
| **Client Setup** | `socket()` → `connect()` → `send()` | `socket()` → `sendto()` |
| **Send Function** | `send(socket, data, len, flags)` | `sendto(socket, data, len, flags, addr, addrlen)` |
| **Receive Function** | `recv(socket, buffer, len, flags)` | `recvfrom(socket, buffer, len, flags, addr, addrlen)` |
| **Reliability** | Guaranteed delivery | No guarantee (may lose packets) |
| **Order** | Ordered (data arrives in sequence) | Unordered (may arrive out of order) |
| **Speed** | Slower (overhead for reliability) | Faster (no overhead) |
| **Data Units** | Stream (continuous) | Datagrams (individual packets) |
| **Use Cases** | Web, Email, File transfer | Video streaming, DNS, Gaming |

---

## Function Differences

### 1. Creating Socket

**TCP:**
```cpp
int socket = socket(AF_INET, SOCK_STREAM, 0);  // Connection-oriented
```

**UDP:**
```cpp
int socket = socket(AF_INET, SOCK_DGRAM, 0);   // Connectionless
```

---

### 2. Server Functions

**TCP Server:**
```cpp
socket();    // Create socket
bind();      // Bind to port
listen();    // Mark as passive (wait for connections)
accept();    // Accept client connection (BLOCKS)
recv();      // Receive data from connected client
send();      // Send data to connected client
close();     // Close sockets
```

**UDP Server:**
```cpp
socket();    // Create socket
bind();      // Bind to port
// NO listen()! NO accept()!
recvfrom();  // Receive datagram from ANY client (BLOCKS)
sendto();    // Send datagram to specific client
close();     // Close socket
```

---

### 3. Client Functions

**TCP Client:**
```cpp
socket();    // Create socket
connect();   // Establish connection to server (BLOCKS)
send();      // Send data through established connection
recv();      // Receive response
close();     // Close socket
```

**UDP Client:**
```cpp
socket();    // Create socket
// NO connect()!
sendto();    // Send datagram directly to server
recvfrom();  // Receive response
close();     // Close socket
```

---

## New Functions Explained

### `sendto()` - Send UDP Datagram

**Syntax:**
```cpp
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);
```

**Parameters:**
- `sockfd` - Socket descriptor
- `buf` - Data to send
- `len` - Length of data
- `flags` - Options (0 = none)
- `dest_addr` - **Destination address** (who to send to)
- `addrlen` - Size of address structure

**Returns:**
- Number of bytes sent
- `-1` on error

**Example:**
```cpp
sendto(socket, message, strlen(message), 0,
       (struct sockaddr*)&serverAddress, sizeof(serverAddress));
```

**Key Difference from `send()`:**
- `send()` - Sends to connected peer (TCP)
- `sendto()` - Specifies destination for each datagram (UDP)

---

### `recvfrom()` - Receive UDP Datagram

**Syntax:**
```cpp
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen);
```

**Parameters:**
- `sockfd` - Socket descriptor
- `buf` - Buffer to store received data
- `len` - Maximum bytes to receive
- `flags` - Options (0 = none)
- `src_addr` - **Source address** (who sent it) - OUTPUT parameter
- `addrlen` - Size of address structure - INPUT/OUTPUT parameter

**Returns:**
- Number of bytes received
- `-1` on error

**Example:**
```cpp
sockaddr_in clientAddress;
socklen_t clientLen = sizeof(clientAddress);

recvfrom(socket, buffer, sizeof(buffer), 0,
         (struct sockaddr*)&clientAddress, &clientLen);
```

**Key Difference from `recv()`:**
- `recv()` - Receives from connected peer (TCP)
- `recvfrom()` - Tells you who sent the datagram (UDP)

---

## UDP Server.cpp - Line by Line

```cpp
int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
//                                 ↑
//                                 SOCK_DGRAM = UDP

sockaddr_in serverAddress;
serverAddress.sin_family = AF_INET;
serverAddress.sin_port = htons(9090);      // Port 9090 for UDP
serverAddress.sin_addr.s_addr = INADDR_ANY;

bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
// Bind UDP socket to port 9090

// NO listen()! NO accept()!
// UDP doesn't establish connections

sockaddr_in clientAddress;
socklen_t clientLen = sizeof(clientAddress);

recvfrom(serverSocket, buffer, sizeof(buffer), 0,
         (struct sockaddr*)&clientAddress, &clientLen);
// Receives datagram from ANY client
// clientAddress will contain sender's IP and port

sendto(serverSocket, response, strlen(response), 0,
       (struct sockaddr*)&clientAddress, clientLen);
// Send response back to the specific client
```

---

## UDP Client.cpp - Line by Line

```cpp
int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
//                                 ↑
//                                 SOCK_DGRAM = UDP

sockaddr_in serverAddress;
serverAddress.sin_family = AF_INET;
serverAddress.sin_port = htons(9090);
serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

// NO connect()!
// UDP doesn't establish connections

sendto(clientSocket, message, strlen(message), 0,
       (struct sockaddr*)&serverAddress, sizeof(serverAddress));
// Send datagram directly to server
// Specify destination address in sendto()

sockaddr_in fromAddress;
socklen_t fromLen = sizeof(fromAddress);

recvfrom(clientSocket, buffer, sizeof(buffer), 0,
         (struct sockaddr*)&fromAddress, &fromLen);
// Receive response
// fromAddress will contain server's IP and port
```

---

## How to Run

### Terminal 1 - Start UDP Server:
```bash
cd /mnt/c/Users/z00542kh/Desktop/C++_KT/IPC/Sockets/UDP
g++ -o server Server.cpp
./server
```

**Expected Output:**
```
=== UDP SERVER ===
✓ UDP socket created (SOCK_DGRAM - connectionless)
✓ Socket bound to port 9090
✓ Server ready (NO connection needed - UDP is connectionless)
⏳ Waiting for datagrams...

📦 Datagram received!
   From: 127.0.0.1:54321
   Message: Hello from UDP Client!
   Bytes: 23

📤 Response sent back to client

✓ UDP server closed
```

### Terminal 2 - Start UDP Client:
```bash
cd /mnt/c/Users/z00542kh/Desktop/C++_KT/IPC/Sockets/UDP
g++ -o client Client.cpp
./client
```

**Expected Output:**
```
=== UDP CLIENT ===
✓ UDP socket created (SOCK_DGRAM - connectionless)
✓ Ready to send (NO connection needed - UDP is connectionless)

📤 Datagram sent!
   To: 127.0.0.1:9090
   Message: Hello from UDP Client!
   Bytes: 23

⏳ Waiting for response...

📦 Response received!
   From: 127.0.0.1:9090
   Message: Hello from UDP Server!
   Bytes: 22

✓ UDP client closed
```

---

## Side-by-Side Code Comparison

### Server Creation:

**TCP:**
```cpp
socket(AF_INET, SOCK_STREAM, 0);
bind();
listen(socket, 5);  ← Connection setup
accept();           ← Wait for client
recv();             ← Receive from connected client
```

**UDP:**
```cpp
socket(AF_INET, SOCK_DGRAM, 0);
bind();
// NO listen()!
// NO accept()!
recvfrom();         ← Receive from ANY client directly
```

### Client Communication:

**TCP:**
```cpp
socket(AF_INET, SOCK_STREAM, 0);
connect();          ← Establish connection
send();             ← Send to connected server
```

**UDP:**
```cpp
socket(AF_INET, SOCK_DGRAM, 0);
// NO connect()!
sendto();           ← Send directly with address
```

---

## Advantages & Disadvantages

### TCP Advantages:
✅ Reliable - guaranteed delivery  
✅ Ordered - data arrives in sequence  
✅ Error checking - automatic retransmission  
✅ Flow control - prevents overwhelming receiver  

### TCP Disadvantages:
❌ Slower - overhead for reliability  
❌ Connection overhead - must establish connection  
❌ More memory - maintains connection state  

### UDP Advantages:
✅ Fast - minimal overhead  
✅ No connection setup - immediate send  
✅ Less memory - no connection state  
✅ Broadcast/Multicast - can send to multiple recipients  

### UDP Disadvantages:
❌ Unreliable - packets may be lost  
❌ Unordered - packets may arrive out of order  
❌ No flow control - may overwhelm receiver  
❌ No error recovery - application must handle  

---

## When to Use Each

### Use TCP When:
✅ Data integrity is critical (file transfer, web pages)  
✅ Order matters (chat messages, emails)  
✅ Can tolerate some latency  
✅ Need guaranteed delivery  

**Examples:** HTTP, FTP, SSH, Email (SMTP)

### Use UDP When:
✅ Speed is more important than reliability (gaming, streaming)  
✅ Can tolerate packet loss (video calls)  
✅ Low latency required (real-time applications)  
✅ Broadcast/multicast needed  

**Examples:** DNS, Video streaming, VoIP, Online gaming

---

## Visual Output Differences

### TCP Output Shows:
```
Server listening...        ← Connection setup
Client connected!          ← Connection established
Message from client: ...   ← Stream data
```

### UDP Output Shows:
```
Server ready (NO connection needed)  ← No connection
Datagram received!                   ← Individual packet
   From: 127.0.0.1:54321            ← Shows sender info
   Message: ...
```

**Key Visual Difference:** UDP always shows sender address because there's no established connection!

---

## Common Issues

### TCP:
- Must start server first
- Port must be free
- Only one connection at a time (in our simple version)

### UDP:
- Must start server first
- Port must be free
- Can receive from multiple clients without modification
- **Packets may be lost** - no error if client runs before server!

---

## Summary Table

| Aspect | TCP | UDP |
|--------|-----|-----|
| Socket type | `SOCK_STREAM` | `SOCK_DGRAM` |
| Connection | Required | Not required |
| `listen()` | ✓ Required | ✗ Not used |
| `accept()` | ✓ Required | ✗ Not used |
| `connect()` | ✓ Required | ✗ Not used |
| Send function | `send()` | `sendto()` |
| Receive function | `recv()` | `recvfrom()` |
| Port in example | 8080 | 9090 |
| Reliability | Guaranteed | Not guaranteed |
| Speed | Slower | Faster |

---

## Quick Test Commands

**TCP:**
```bash
# Terminal 1
cd TCP
./server

# Terminal 2
cd TCP
./client
```

**UDP:**
```bash
# Terminal 1
cd UDP
./server

# Terminal 2
cd UDP
./client
```

---

## Key Takeaways

🔑 **TCP = Phone Call** (establish connection, reliable, ordered)  
🔑 **UDP = Walkie-Talkie** (just send, fast, may lose messages)  

🔑 **TCP uses:** `send()`/`recv()` (connected)  
🔑 **UDP uses:** `sendto()`/`recvfrom()` (with addresses)  

🔑 **TCP:** Connection-oriented, reliable, slower  
🔑 **UDP:** Connectionless, unreliable, faster  
