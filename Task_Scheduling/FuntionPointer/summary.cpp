// FUNCTION POINTER - KEY CONCEPTS SUMMARY
#include <iostream>
using namespace std;

/*
═══════════════════════════════════════════════════════════════
WHAT IS A FUNCTION POINTER?
═══════════════════════════════════════════════════════════════
A variable that stores the address of a function.

SYNTAX:
    return_type (*pointer_name)(parameter_types);

EXAMPLE:
    int (*funcPtr)(int, int);
     ↑    ↑         ↑
     |    |         Parameters
     |    Pointer name
     Return type

═══════════════════════════════════════════════════════════════
WHY USE FUNCTION POINTERS?
═══════════════════════════════════════════════════════════════

1. CALLBACKS
   - Pass behavior to functions
   - Event handlers
   
2. POLYMORPHISM (without classes)
   - Different functions, same signature
   - Strategy pattern
   
3. DYNAMIC BEHAVIOR
   - Choose function at runtime
   - Plugin systems

═══════════════════════════════════════════════════════════════
COMMON PATTERNS
═══════════════════════════════════════════════════════════════

Pattern 1: Callback
    void process(int data, int (*callback)(int))

Pattern 2: Array of functions
    void (*handlers[])() = {func1, func2, func3}

Pattern 3: Typedef for clarity
    typedef void (*Handler)();

═══════════════════════════════════════════════════════════════
REAL WORLD USES
═══════════════════════════════════════════════════════════════

✓ Event systems (onClick, onLoad)
✓ Sorting algorithms (qsort, custom comparators)
✓ State machines (different handlers per state)
✓ Menu systems (each option = function)
✓ Plugin architecture (load functions dynamically)
✓ Interrupt handlers (embedded systems)
✓ Signal handlers (UNIX/Linux)

═══════════════════════════════════════════════════════════════
FUNCTION POINTER vs STD::FUNCTION
═══════════════════════════════════════════════════════════════

Function Pointer:
  ✓ Faster (no overhead)
  ✓ C compatible
  ✗ Only regular functions
  ✗ Complex syntax

std::function:
  ✓ Can store lambdas
  ✓ Cleaner syntax
  ✓ More flexible
  ✗ Slight overhead

Recommendation: Use std::function for new code

═══════════════════════════════════════════════════════════════
QUICK REFERENCE
═══════════════════════════════════════════════════════════════
*/

// Example 1: Simple function pointer
void simpleExample() {
    int (*fp)(int, int);  // Declare
    // fp = someFunction;  // Assign
    // int result = fp(5, 3);  // Call
}

// Example 2: As parameter (callback)
void callbackExample() {
    // void process(int data, void (*callback)(int));
}

// Example 3: Array of pointers
void arrayExample() {
    // void (*functions[])() = {func1, func2, func3};
    // functions[0]();  // Call first function
}

// Example 4: Typedef
void typedefExample() {
    // typedef void (*Handler)();
    // Handler h = someFunction;
}

int main() {
    cout << "Function Pointer Concepts - See code comments" << endl;
    return 0;
}

/*
═══════════════════════════════════════════════════════════════
REMEMBER
═══════════════════════════════════════════════════════════════
1. Function pointers store addresses of functions
2. Main use: callbacks and dynamic behavior
3. Modern C++: prefer std::function
4. Real world: events, sorting, plugins
═══════════════════════════════════════════════════════════════
*/
