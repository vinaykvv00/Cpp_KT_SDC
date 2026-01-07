#include <iostream>
#include <cstdint>
#include <cstring>

template<typename T, size_t PoolSize>
class AdvancedPool {
private:
    struct Block {
        alignas(T) uint8_t data[sizeof(T)];
        Block* next;
        
        T* getObject() {
            return reinterpret_cast<T*>(data);
        }
    };
    
    Block blocks[PoolSize];
    Block* freeList;
    size_t usedCount;
    
public:
    AdvancedPool() : freeList(nullptr), usedCount(0) {
        std::cout << "Creating advanced pool with " << PoolSize << " blocks\n";
        std::cout << "Each block size: " << sizeof(Block) << " bytes\n";
        std::cout << "Total pool size: " << sizeof(blocks) << " bytes\n\n";
        
        for (size_t i = 0; i < PoolSize - 1; ++i) {
            blocks[i].next = &blocks[i + 1];
        }
        blocks[PoolSize - 1].next = nullptr;
        
        freeList = &blocks[0];
        
        std::cout << "Free list initialized:\n";
        std::cout << "  blocks[0] -> blocks[1] -> ... -> blocks[" << (PoolSize-1) << "] -> NULL\n\n";
    }
    
    T* allocate() {
        if (!freeList) {
            std::cout << "ERROR: Pool exhausted!\n";
            return nullptr;
        }
        
        Block* block = freeList;
        freeList = freeList->next;
        usedCount++;
        
        std::cout << "Allocated block at index " << (block - blocks) 
                  << ". Used: " << usedCount << "/" << PoolSize << "\n";
        
        return new (block->data) T();
    }
    
    void deallocate(T* ptr) {
        if (!ptr) return;
        
        ptr->~T();
        
        Block* block = reinterpret_cast<Block*>(reinterpret_cast<uint8_t*>(ptr) - offsetof(Block, data));
        block->next = freeList;
        freeList = block;
        usedCount--;
        
        std::cout << "Deallocated block at index " << (block - blocks)
                  << ". Used: " << usedCount << "/" << PoolSize << "\n";
    }
    
    size_t available() const {
        return PoolSize - usedCount;
    }
    
    void printStatus() const {
        std::cout << "\n--- Pool Status ---\n";
        std::cout << "Total blocks: " << PoolSize << "\n";
        std::cout << "Used blocks:  " << usedCount << "\n";
        std::cout << "Free blocks:  " << available() << "\n";
    }
};

struct Particle {
    float x, y, z;
    int lifetime;
    
    Particle() : x(0), y(0), z(0), lifetime(100) {
        std::cout << "  [Particle constructed]\n";
    }
    
    ~Particle() {
        std::cout << "  [Particle destructed]\n";
    }
};

int main() {
    std::cout << "=== STEP 3: Advanced Pool with Free List ===\n\n";
    
    AdvancedPool<Particle, 5> particlePool;
    
    std::cout << "--- Allocating 3 particles ---\n";
    Particle* p1 = particlePool.allocate();
    Particle* p2 = particlePool.allocate();
    Particle* p3 = particlePool.allocate();
    
    particlePool.printStatus();
    
    std::cout << "\n--- Using particles ---\n";
    if (p1) {
        p1->x = 10.5f;
        p1->lifetime = 50;
        std::cout << "Particle 1: x=" << p1->x << ", lifetime=" << p1->lifetime << "\n";
    }
    
    std::cout << "\n--- Deallocating particle 2 ---\n";
    particlePool.deallocate(p2);
    
    particlePool.printStatus();
    
    std::cout << "\n--- Allocating new particle (reuses p2's block) ---\n";
    Particle* p4 = particlePool.allocate();
    std::cout << "p2 address: " << p2 << "\n";
    std::cout << "p4 address: " << p4 << "\n";
    std::cout << "Same block? " << (p2 == p4 ? "YES" : "NO") << "\n";
    
    particlePool.printStatus();
    
    std::cout << "\n--- Deallocating all ---\n";
    particlePool.deallocate(p1);
    particlePool.deallocate(p3);
    particlePool.deallocate(p4);
    
    particlePool.printStatus();
    
    std::cout << "\n--- Key Concept ---\n";
    std::cout << "Free list is a linked list of available blocks.\n";
    std::cout << "Allocation/deallocation is O(1) - just pointer updates!\n";
    
    return 0;
}
