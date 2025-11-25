# 🚀 Quick Start Guide - UDP Chat Room

## Build (One Time)
```bash
cd /mnt/c/Users/z00542kh/Desktop/C++_KT/Task_2/build
cmake ..
make
```

## Run

### 1. Start Server (Terminal 1)
```bash
cd /mnt/c/Users/z00542kh/Desktop/C++_KT/Task_2/build
./chat_server 8080
```

### 2. Start Users (New Terminals)
```bash
cd /mnt/c/Users/z00542kh/Desktop/C++_KT/Task_2/build
./chat_user 127.0.0.1 8080
```

## Commands
- `/join <room_name>` - Join a chat room
- `/leave` - Leave current room
- `/quit` - Exit
- Any text - Send message to current room

## Quick Test Scenario

**Terminal 1 - Server:**
```bash
./chat_server 8080
# Server starts listening...
```

**Terminal 2 - Alice:**
```bash
./chat_user 127.0.0.1 8080
Alice
> /join general
> Hello!
```

**Terminal 3 - Bob:**
```bash
./chat_user 127.0.0.1 8080
Bob
> /join general
[Alice]: Hello!  ← Should see this
> Hi Alice!
```

**Terminal 4 - Charlie:**
```bash
./chat_user 127.0.0.1 8080
Charlie
> /join gaming
> Anyone here?
# Alice and Bob won't see this (different room) ✓
```

## Expected Behavior
✅ Users in same room see each other's messages  
✅ Users in different rooms are isolated  
✅ Can type while receiving messages (separate threads)  
✅ Server shows all activity  

## Files Location
- **Source**: `/mnt/c/Users/z00542kh/Desktop/C++_KT/Task_2/`
- **Executables**: `/mnt/c/Users/z00542kh/Desktop/C++_KT/Task_2/build/`

## Troubleshooting
- **Port already in use**: Change 8080 to another port (e.g., 9000)
- **Can't connect**: Make sure server is running first
- **Messages not showing**: Check you're in the same room

---
**Status: ✅ WORKING**
