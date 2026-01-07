# 🎯 SUPER SIMPLE INSTRUCTIONS

## What htop Shows You

htop shows you **which CPU cores are busy**.

Think of it like this:

```
Your Computer has 28 Cores (like 28 workers)

htop shows you:
  Core 1: [████████] 100% ← This worker is BUSY
  Core 2: [████████] 100% ← This worker is BUSY
  Core 3: [██      ]  25% ← This worker is RELAXING
  Core 4: [        ]   0% ← This worker is SLEEPING
```

## What You Need to Do

### Step 1: Run the program
```bash
./simple_demo
```

### Step 2: Open htop
Open a NEW terminal and type:
```bash
htop
```

### Step 3: Look at the TOP
Just look at the very top where you see:
```
1 [████████]
2 [████████]
3 [████████]
4 [████████]
```

**Question: Are 4 bars FULL?**
- YES → Your program works! ✅
- NO → Something wrong ❌

### Step 4: Close htop
Press the letter: **q**

### Step 5: Stop program
Go back to first terminal
Press: **Ctrl+C**

## That's ALL You Need to Know! 🎯

htop = Shows which cores are busy
Full bar = Core is working 100%
Empty bar = Core is idle

**Don't worry about anything else in htop!**
