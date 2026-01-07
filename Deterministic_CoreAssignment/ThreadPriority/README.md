# Thread Priority in C++ - Complete Guide

## 📖 What is Thread Priority?

Thread priority tells the OS scheduler which threads are more important. Higher priority threads get:
- More CPU time
- Run before lower priority threads
- Better response time

Think of it like airport boarding: First class (high priority) boards before economy (low priority).

---

## 🎯 Key Concepts

### 1. **Priority Values**
```
Real-Time Priorities (SCHED_FIFO/SCHED_RR):
┌─────────────────────────────────┐
│ 99 - Emergency/Critical          │ ← Highest
│ 80-90 - Very High Priority       │
│ 50-70 - High Priority            │
│ 20-40 - Medium Priority          │
│ 1-10 - Low Priority              │ ← Lowest
└─────────────────────────────────┘

Normal Priority (SCHED_OTHER):
Usually 0, managed dynamically by OS
```

### 2. **Scheduling Policies**

| Policy | Description | Use Case |
|--------|-------------|----------|
| **SCHED_OTHER** | Default, time-sharing | Normal applications |
| **SCHED_FIFO** | Real-time, runs until done | Critical tasks |
| **SCHED_RR** | Real-time, time slices | Multiple real-time tasks |

### 3. **When to Use Priority**

✅ **Use HIGH priority for:**
- Real-time data processing
- Video/Audio streaming
- Network packet handling
- Safety-critical operations

✅ **Use LOW priority for:**
- Background logging
- File indexing
- Statistics collection
- Cleanup tasks

---

## 📝 Program Examples

### **01_basic_priority.cpp**
- **Level**: Beginner
- **Concept**: Basic priority setting
- **What it shows**: High priority thread finishes faster
- **Run**: `g++ -pthread 01_basic_priority.cpp -o basic && sudo ./basic`

### **02_priority_levels.cpp**
- **Level**: Intermediate
- **Concept**: Multiple priority levels
- **What it shows**: 3 threads with CRITICAL, NORMAL, BACKGROUND priorities
- **Run**: `g++ -pthread 02_priority_levels.cpp -o levels && sudo ./levels`

### **03_scheduling_policies.cpp**
- **Level**: Advanced
- **Concept**: SCHED_OTHER vs SCHED_FIFO vs SCHED_RR
- **What it shows**: Different scheduling behaviors
- **Run**: `g++ -pthread 03_scheduling_policies.cpp -o policies && sudo ./policies`

### **04_real_world_example.cpp**
- **Level**: Practical
- **Concept**: Video processing simulation
- **What it shows**: Real application of priorities
- **Run**: `g++ -pthread 04_real_world_example.cpp -o video && sudo ./video`

### **05_priority_inversion.cpp**
- **Level**: Advanced Problem
- **Concept**: Priority inversion issue
- **What it shows**: When high priority waits for low priority
- **Run**: `g++ -pthread 05_priority_inversion.cpp -o inversion && sudo ./inversion`

---

## ⚙️ How to Set Thread Priority

```cpp
#include <pthread.h>
#include <thread>

// 1. Create thread
std::thread my_thread(task_function);

// 2. Set priority
sched_param param;
param.sched_priority = 70;  // 1-99 for real-time

// 3. Apply to thread
pthread_setschedparam(
    my_thread.native_handle(),  // Thread handle
    SCHED_FIFO,                 // Policy
    &param                      // Priority parameter
);
```

---

## 🚨 Important Points

### **Permissions Required**
- Setting real-time priorities (SCHED_FIFO/SCHED_RR) requires **root/sudo**
- Without sudo, priority setting silently fails
- Run with: `sudo ./program`

### **Priority Inversion**
**Problem**: High priority thread waits for low priority thread holding a lock

**Example**:
```
LOW priority:  [====LOCK HELD====]
MEDIUM:            [====RUNNING====]
HIGH priority:         ⏳ WAITING (blocked!)
```

**Solutions**:
1. Priority inheritance (boost LOW to HIGH temporarily)
2. Priority ceiling (run with highest priority while holding lock)
3. Avoid long critical sections

### **CPU Affinity vs Priority**
- **Affinity**: Which core to run on (location)
- **Priority**: How much CPU time to get (importance)
- Can combine both for deterministic behavior

---

## 🎓 Learning Path

1. **Start with**: `01_basic_priority.cpp` - Understand the concept
2. **Then**: `02_priority_levels.cpp` - See multiple priorities
3. **Next**: `03_scheduling_policies.cpp` - Learn policies
4. **Apply**: `04_real_world_example.cpp` - See practical use
5. **Advanced**: `05_priority_inversion.cpp` - Understand problems

---

## 🔧 Compilation Commands

```bash
# Basic compilation
g++ -pthread -o output filename.cpp

# Run with sudo (needed for priority setting)
sudo ./output

# Compile all examples
g++ -pthread 01_basic_priority.cpp -o basic
g++ -pthread 02_priority_levels.cpp -o levels
g++ -pthread 03_scheduling_policies.cpp -o policies
g++ -pthread 04_real_world_example.cpp -o video
g++ -pthread 05_priority_inversion.cpp -o inversion
```

---

## 📊 Expected Behavior

### Without Priority:
```
Thread 1: ████████
Thread 2: ████████
Thread 3: ████████
(All get equal CPU time)
```

### With Priority:
```
High:   ████████████████  (90% CPU)
Medium: ████              (7% CPU)
Low:    █                 (3% CPU)
```

---

## 🌟 Real-World Applications

1. **Medical Devices**: Patient monitoring (HIGH priority)
2. **Automotive**: Brake control (CRITICAL priority)
3. **Multimedia**: Video frame processing (HIGH), logging (LOW)
4. **Gaming**: Rendering (HIGH), telemetry (LOW)
5. **Industrial**: Safety systems (CRITICAL), diagnostics (LOW)

---

## ⚠️ Common Mistakes

1. **Not checking return values** - Priority setting can fail silently
2. **Forgetting sudo** - Real-time priorities need root
3. **Setting all threads HIGH** - Defeats the purpose
4. **Long critical sections** - Causes priority inversion
5. **Using on battery-powered devices** - Drains battery faster

---

## 📚 Summary

| Aspect | Description |
|--------|-------------|
| **Purpose** | Control CPU time allocation |
| **Range** | 1-99 (real-time), 0 (normal) |
| **Policies** | SCHED_OTHER, SCHED_FIFO, SCHED_RR |
| **Requires** | Root/sudo for real-time priorities |
| **Best For** | Time-critical, deterministic systems |

**Remember**: Use priorities wisely. Not every thread needs high priority!
