# UDP Chat Room - Task 2

## Build Instructions

```bash
cd Task_2
mkdir build
cd build
cmake ..
make
```

## Running the Chat Room

### 1. Start the Server
```bash
./chat_server 8080
```

### 2. Start Users (In separate terminals)

**Terminal 2 - Alice:**
```bash
./chat_user 127.0.0.1 8080
# Enter username: Alice
# > /join general
# > Hello everyone!
```

**Terminal 3 - Bob:**
```bash
./chat_user 127.0.0.1 8080
# Enter username: Bob
# > /join general
# > Hey Alice!
```

**Terminal 4 - Charlie:**
```bash
./chat_user 127.0.0.1 8080
# Enter username: Charlie
# > /join gaming
# > Anyone playing?
```

## Commands

- `/join <room>` - Join a chat room
- `/leave` - Leave current room
- `/quit` - Exit application
- Any other text - Send message to current room

## Expected Behavior

1. ✅ Alice and Bob in "general" can see each other's messages
2. ✅ Charlie in "gaming" cannot see "general" messages
3. ✅ Each user has a receiver thread (non-blocking input)
4. ✅ Server broadcasts messages to all room members
5. ✅ UDP sockets used for communication

## Project Structure

```
Task_2/
├── CMakeLists.txt
├── include/
│   ├── Message.h       # Message protocol (JOIN/CHAT/LEAVE)
│   ├── Room.h          # Room management with thread safety
│   └── Utils.h         # String parsing utilities
├── src/
│   ├── Message.cpp
│   ├── Room.cpp
│   └── Utils.cpp
├── server_main.cpp     # Server executable
└── user_main.cpp       # Client executable
```

## Key Features Implemented

- ✅ UDP socket communication
- ✅ Multiple chat rooms
- ✅ Thread-safe room management (mutex)
- ✅ Separate receiver thread for each user
- ✅ Message broadcasting to room members
- ✅ Simple pipe-delimited message protocol
