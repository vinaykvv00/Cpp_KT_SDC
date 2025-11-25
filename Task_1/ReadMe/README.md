# Simple Pub-Sub System

## Overview
This is a **simple, easy-to-understand** implementation of a Producer-Consumer system using the Publish-Subscribe pattern.

## What's Changed - SIMPLIFIED CODE!

### ❌ Removed Complex Features:
- No more `std::atomic` with advanced memory ordering
- No more `std::shared_ptr` and complex pointers
- No more `std::function` callbacks  
- No more TopicManager (unnecessary complexity)
- No more C++20 features (now uses C++11)
- No more advanced error handling code

### ✅ Simple Features:
- **Message**: Just stores ID and data
- **Topic**: Central hub for pub-sub with simple mutex
- **Producer**: Creates messages and publishes them
- **Consumer**: Waits and receives ALL messages
- **Utils**: Thread-safe printing

## File Structure
```
Task_1/
├── CMakeLists.txt          # Build configuration (C++11)
├── main.cpp                # Simple test program
├── include/
│   ├── Message.h          # Simple message class
│   ├── Topic.h            # Pub-sub hub
│   ├── Producer.h         # Message producer
│   ├── Consumer.h         # Message consumer
│   └── Utils.h            # Thread-safe print
└── src/
    ├── Message.cpp        # ~10 lines
    ├── Topic.cpp          # ~50 lines
    ├── Producer.cpp       # ~35 lines
    ├── Consumer.cpp       # ~40 lines
    └── Utils.cpp          # ~8 lines
```

## Key Concepts (SIMPLE!)

### 1. Message
```cpp
Message msg(1, "Hello");  // ID and data - that's it!
```

### 2. Topic (Pub-Sub Hub)
- Stores messages in a list (`std::deque`)
- Each consumer has an "offset" (which message they're on)
- Uses `std::mutex` for thread safety
- Uses `std::condition_variable` to wake up waiting consumers

### 3. Producer
```cpp
Producer p1(1, "P1");
p1.start(topic, 5);  // Send 5 messages to topic
p1.stop();           // Wait for completion
```

### 4. Consumer  
```cpp
Consumer c1(1, "C1");
c1.start(topic, 10); // Receive 10 messages from topic
c1.stop();           // Wait for completion
```

## How It Works

1. **Consumers register** with Topic (get an offset starting at 0)
2. **Producers publish** messages to Topic
3. **Topic stores** all messages in a list
4. **Each consumer** reads from their own offset
5. **All consumers get ALL messages** (that's pub-sub!)

## Build & Run

```bash
cd Task_1
mkdir build && cd build
cmake ..
make
./pubsub_example
```

## Expected Output

```
=== Simple Pub-Sub System ===

--- Starting Consumers ---

--- Starting Producers ---
[Producer P1] Published: 1000 - P1_msg_0
[Consumer C1] Received: 1000 - P1_msg_0
[Consumer C2] Received: 1000 - P1_msg_0
[Consumer C3] Received: 1000 - P1_msg_0
...
--- Producers Finished ---

=== Results ===
Consumer 1 received: 10 messages
Consumer 2 received: 10 messages
Consumer 3 received: 10 messages
Total messages sent: 10

✓ Pub-Sub Pattern: Each consumer gets ALL messages
```

## What Makes This SIMPLE?

### Before (Complex):
- `std::atomic<uint64_t>` with `memory_order_acquire/release`
- `std::shared_ptr<SubscriberEntry>` with custom queues
- `std::function<MessagePtr()>` callbacks
- `std::unordered_map` for subscriber management
- C++20 features like `= default` constructors

### After (Simple):
- Regular `bool running_` flag
- Simple `std::deque<Message>` for storage
- Direct function calls, no callbacks
- `std::vector` for consumer tracking
- Basic C++11 features only

## Thread Safety (SIMPLE!)

1. **One mutex per Topic** - protects message list and offsets
2. **One mutex for printing** - prevents messy output
3. **condition_variable** - wakes up waiting consumers

## The Pub-Sub Pattern

**Key Difference from Queue:**
- **Queue**: 1 message → 1 consumer (load balancing)
- **Pub-Sub**: 1 message → ALL consumers (broadcasting)

This implementation uses **Pub-Sub (broadcasting)**!

## Fixed Errors

### Original Error:
```
This declaration has no storage class or type specifier
SubscriberEntry::SubscriberEntry
```

**Problem**: `SubscriberEntry` was a struct in header, but had constructor in .cpp file

**Solution**: Removed `SubscriberEntry` completely! Used simple `std::vector` instead.

### Other Fixes:
- Removed all `std::atomic` with complex memory ordering
- Removed `std::shared_ptr` complexity
- Removed TopicManager (not needed)
- Changed C++20 to C++11
- Added pthread library to CMakeLists.txt

## Learn More

- **Mutex**: Lock that only one thread can hold at a time
- **Condition Variable**: Way to wake up waiting threads
- **Deque**: Double-ended queue (like a list)
- **Thread**: Separate execution path

## Summary

✅ **No syntax errors**
✅ **Simple code** (< 50 lines per file)
✅ **Easy to understand**
✅ **Pub-sub pattern working correctly**
✅ **All consumers get all messages**
