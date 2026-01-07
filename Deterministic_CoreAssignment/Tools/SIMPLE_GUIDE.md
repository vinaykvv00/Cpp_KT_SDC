# 🎯 SIMPLE HTOP GUIDE - What to Look At

## Step 1: Run Your Program
```bash
cd /home/z00542kh/cpp_KT/C++_KT/Deterministic_CoreAssignment/Tools
./moniter
```

## Step 2: Open htop in Another Terminal
```bash
htop
```

## Step 3: Look ONLY at the TOP PART

```
┌──────────────────────────────────────┐
│  1  [|||||||||||||100%]  ← Core 0    │  ← LOOK HERE!
│  2  [|||||||||||||100%]  ← Core 1    │  ← LOOK HERE!
│  3  [|||||||||||||100%]  ← Core 2    │  ← LOOK HERE!
│  4  [|||||||||||||100%]  ← Core 3    │  ← LOOK HERE!
│  5  [||10%            ]  ← Core 4    │
│  6  [||8%             ]  ← Core 5    │
└──────────────────────────────────────┘
       ↑
  This shows CPU usage per core
```

## What You Should See:

### ✅ CORRECT (Working):
```
1  [████████████████100%]  ← Core 0 is FULL (Task 1)
2  [████████████████100%]  ← Core 1 is FULL (Task 2)
3  [████████████████100%]  ← Core 2 is FULL (Task 3)
4  [████████████████100%]  ← Core 3 is FULL (Task 4)
5  [░10%               ]  ← Core 4 is IDLE
```

### ❌ WRONG (Not Working):
```
1  [████50%            ]  ← Not fully used
2  [████50%            ]  
3  [░10%               ]  ← Should be 100%
4  [░10%               ]
```

## IGNORE Everything Else!

**DON'T LOOK AT:**
- The list of processes (bottom part)
- Memory bars
- Swap
- Process names

**ONLY LOOK AT:**
- The numbered bars at the top (1, 2, 3, 4...)
- Are they at 100%?
- How many are at 100%?

## That's It!

**Simple Rule:**
```
If you have 4 threads:
→ You should see 4 cores at 100%

If you have 2 threads:
→ You should see 2 cores at 100%
```

## Press 'q' to Quit htop
```
Just press: q
```
