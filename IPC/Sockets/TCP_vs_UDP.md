# TCP vs UDP - Quick Comparison

## Visual Difference in Output

### TCP Output (Connection-Based):
```
SERVER:                          CLIENT:
Server listening...              Client socket created
⏳ Waiting for connection...     Connected to server!
Client connected!                Message sent: Hello, server!
Message received: Hello, server! Connection closed
Server closed
```

### UDP Output (No Connection):
```
SERVER:                          CLIENT:
Server ready (NO connection)     Ready to send (NO connection)
⏳ Waiting for datagrams...      📤 Datagram sent!
📦 Datagram received!               To: 127.0.0.1:9090
   From: 127.0.0.1:54321            Bytes: 23
   Message: Hello from UDP!      📦 Response received!
📤 Response sent                     From: 127.0.0.1:9090
Server closed                     Client closed
```

---

## Code Differences

### Socket Creation:
```cpp
// TCP
socket(AF_INET, SOCK_STREAM, 0);

// UDP  
socket(AF_INET, SOCK_DGRAM, 0);
```

### Server Setup:
```cpp
// TCP
socket() → bind() → listen() → accept() → recv()

// UDP
socket() → bind() → recvfrom()
```

### Client Setup:
```cpp
// TCP
socket() → connect() → send()

// UDP
socket() → sendto()
```

### Send/Receive Functions:
```cpp
// TCP (to/from connected peer)
send(socket, data, len, 0);
recv(socket, buffer, len, 0);

// UDP (specify destination/get source)
sendto(socket, data, len, 0, &address, addrlen);
recvfrom(socket, buffer, len, 0, &address, &addrlen);
```

---

## Key Differences You'll See

| What You'll Notice | TCP | UDP |
|-------------------|-----|-----|
| Output messages | "Client connected" | "NO connection needed" |
| Server functions | `listen()`, `accept()` | None (just `recvfrom()`) |
| Client functions | `connect()` | None (just `sendto()`) |
| Sender info | Not shown (already connected) | Shows sender IP:port |
| Port in examples | 8080 | 9090 |
| Emoji in output | ⏳ ✓ | 📤 📦 |

---

## Analogy

**TCP = Phone Call 📞**
- Must dial and connect first
- Guaranteed delivery
- Ordered conversation
- Both know when connected

**UDP = Walkie-Talkie 📻**
- Just press button and talk
- May not hear everything
- Can talk to anyone without connecting
- Don't know if they received it

---

## How to Test Both

### TCP (Port 8080):
```bash
# Terminal 1
cd TCP
./server

# Terminal 2
cd TCP
./client
```

### UDP (Port 9090):
```bash
# Terminal 1
cd UDP
./server

# Terminal 2
cd UDP
./client
```

---

## Quick Reference

| Feature | TCP | UDP |
|---------|-----|-----|
| Type | `SOCK_STREAM` | `SOCK_DGRAM` |
| Connection | Yes | No |
| Reliable | Yes | No |
| Ordered | Yes | No |
| Speed | Slower | Faster |
| Functions | `send`/`recv` | `sendto`/`recvfrom` |
| Use case | Web, Email | Streaming, Gaming |
