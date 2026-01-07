#include <iostream>
#include <vector>
#include <cstdint>

template<typename T, size_t PoolSize>
class ObjectPool {
private:
    struct Block {
        alignas(T) uint8_t data[sizeof(T)];
        Block* next;
    };
    
    Block blocks[PoolSize];
    Block* freeList;
    size_t activeCount;
    
public:
    ObjectPool() : freeList(nullptr), activeCount(0) {
        for (size_t i = 0; i < PoolSize - 1; ++i) {
            blocks[i].next = &blocks[i + 1];
        }
        blocks[PoolSize - 1].next = nullptr;
        freeList = &blocks[0];
    }
    
    T* allocate() {
        if (!freeList) {
            return nullptr;
        }
        
        Block* block = freeList;
        freeList = freeList->next;
        activeCount++;
        
        return new (block->data) T();
    }
    
    void deallocate(T* ptr) {
        if (!ptr) return;
        
        ptr->~T();
        
        Block* block = reinterpret_cast<Block*>(reinterpret_cast<uint8_t*>(ptr) - offsetof(Block, data));
        block->next = freeList;
        freeList = block;
        activeCount--;
    }
    
    size_t getActiveCount() const { return activeCount; }
};

struct Bullet {
    float x, y;
    float velocityX, velocityY;
    int damage;
    bool active;
    
    Bullet() : x(0), y(0), velocityX(0), velocityY(0), damage(10), active(true) {
    }
    
    void fire(float startX, float startY, float vx, float vy) {
        x = startX;
        y = startY;
        velocityX = vx;
        velocityY = vy;
        active = true;
    }
    
    void update() {
        if (active) {
            x += velocityX;
            y += velocityY;
            
            if (x < 0 || x > 1000 || y < 0 || y > 1000) {
                active = false;
            }
        }
    }
    
    void print() const {
        std::cout << "  Bullet at (" << x << ", " << y << ") "
                  << "velocity=(" << velocityX << ", " << velocityY << ") "
                  << (active ? "ACTIVE" : "DEAD") << "\n";
    }
};

class BulletManager {
private:
    ObjectPool<Bullet, 100> pool;
    std::vector<Bullet*> activeBullets;
    
public:
    void fireBullet(float x, float y, float vx, float vy) {
        Bullet* bullet = pool.allocate();
        if (bullet) {
            bullet->fire(x, y, vx, vy);
            activeBullets.push_back(bullet);
            std::cout << "Fired bullet! Active: " << pool.getActiveCount() << "/100\n";
        } else {
            std::cout << "Pool exhausted! Cannot fire bullet.\n";
        }
    }
    
    void update() {
        for (size_t i = 0; i < activeBullets.size(); ) {
            activeBullets[i]->update();
            
            if (!activeBullets[i]->active) {
                std::cout << "Bullet went off-screen, returning to pool\n";
                pool.deallocate(activeBullets[i]);
                activeBullets[i] = activeBullets.back();
                activeBullets.pop_back();
            } else {
                ++i;
            }
        }
    }
    
    void printStatus() const {
        std::cout << "\n=== Bullet Manager Status ===\n";
        std::cout << "Active bullets: " << pool.getActiveCount() << "\n";
        for (const auto* bullet : activeBullets) {
            bullet->print();
        }
    }
};

int main() {
    std::cout << "=== STEP 4: Real-World Example - Game Bullets ===\n\n";
    
    BulletManager bulletMgr;
    
    std::cout << "--- Player fires 3 bullets ---\n";
    bulletMgr.fireBullet(100, 100, 10, 5);
    bulletMgr.fireBullet(150, 200, 15, 10);
    bulletMgr.fireBullet(200, 300, 20, 15);
    
    bulletMgr.printStatus();
    
    std::cout << "\n--- Update (bullets move) ---\n";
    for (int i = 0; i < 50; ++i) {
        bulletMgr.update();
    }
    
    bulletMgr.printStatus();
    
    std::cout << "\n--- Fire more bullets (reusing returned blocks) ---\n";
    bulletMgr.fireBullet(50, 50, 5, 5);
    bulletMgr.fireBullet(75, 75, 8, 8);
    
    bulletMgr.printStatus();
    
    std::cout << "\n--- Key Benefits ---\n";
    std::cout << "1. No new/delete during gameplay (predictable performance)\n";
    std::cout << "2. Pool prevents memory fragmentation\n";
    std::cout << "3. Automatic reuse of dead bullets\n";
    std::cout << "4. Cache-friendly (bullets stored contiguously)\n";
    
    return 0;
}
