#include <iostream>
#include <vector>
#include <chrono>

template<typename T>
class SimplePool {
private:
    std::vector<T*> pool;
    std::vector<T*> available;
    
public:
    SimplePool(size_t size) {
        for (size_t i = 0; i < size; ++i) {
            T* obj = new T();
            pool.push_back(obj);
            available.push_back(obj);
        }
    }
    
    T* get() {
        if (available.empty()) {
            return nullptr;
        }
        T* obj = available.back();
        available.pop_back();
        return obj;
    }
    
    void release(T* obj) {
        available.push_back(obj);
    }
    
    ~SimplePool() {
        for (T* obj : pool) {
            delete obj;
        }
    }
};

struct GameObject {
    int x, y, z;
    float velocity;
    int health;
};

void test_normal_allocation(int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        GameObject* obj1 = new GameObject();
        GameObject* obj2 = new GameObject();
        GameObject* obj3 = new GameObject();
        
        obj1->x = i;
        obj2->y = i;
        obj3->z = i;
        
        delete obj1;
        delete obj2;
        delete obj3;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Normal new/delete: " << duration.count() << " microseconds\n";
}

void test_pool_allocation(int iterations) {
    SimplePool<GameObject> pool(3);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        GameObject* obj1 = pool.get();
        GameObject* obj2 = pool.get();
        GameObject* obj3 = pool.get();
        
        obj1->x = i;
        obj2->y = i;
        obj3->z = i;
        
        pool.release(obj1);
        pool.release(obj2);
        pool.release(obj3);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Memory Pool:       " << duration.count() << " microseconds\n";
}

int main() {
    std::cout << "=== STEP 2: Performance Comparison ===\n\n";
    
    int iterations = 100000;
    std::cout << "Testing " << iterations << " allocations...\n\n";
    
    test_normal_allocation(iterations);
    test_pool_allocation(iterations);
    
    std::cout << "\n--- Explanation ---\n";
    std::cout << "Memory pool is MUCH faster because:\n";
    std::cout << "1. No OS calls (new/delete talk to OS)\n";
    std::cout << "2. No fragmentation checking\n";
    std::cout << "3. Simple pointer swap from vector\n";
    
    return 0;
}
