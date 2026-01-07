#include <iostream>
#include <vector>

template<typename T>
class SimplePool {
private:
    std::vector<T*> pool;
    std::vector<T*> available;
    
public:
    SimplePool(size_t size) {
        std::cout << "Creating pool with " << size << " objects\n";
        
        for (size_t i = 0; i < size; ++i) {
            T* obj = new T();
            pool.push_back(obj);
            available.push_back(obj);
        }
        
        std::cout << "Pool ready! " << available.size() << " objects available\n";
    }
    
    T* get() {
        if (available.empty()) {
            std::cout << "Pool exhausted!\n";
            return nullptr;
        }
        
        T* obj = available.back();
        available.pop_back();
        std::cout << "Got object from pool. Remaining: " << available.size() << "\n";
        return obj;
    }
    
    void release(T* obj) {
        available.push_back(obj);
        std::cout << "Returned object to pool. Available: " << available.size() << "\n";
    }
    
    ~SimplePool() {
        std::cout << "Destroying pool...\n";
        for (T* obj : pool) {
            delete obj;
        }
    }
};

struct Player {
    int health = 100;
    int score = 0;
    
    Player() {
        std::cout << "  [Player created]\n";
    }
    
    ~Player() {
        std::cout << "  [Player destroyed]\n";
    }
};

int main() {

    SimplePool<Player> playerPool(5);
    
    Player* p1 = playerPool.get();
    Player* p2 = playerPool.get();
    Player* p3 = playerPool.get();
    
    if (p1) {
        p1->health = 80;
        p1->score = 100;
        std::cout << "Player1: health=" << p1->health << ", score=" << p1->score << "\n";
    }
    
    playerPool.release(p1);
    
    Player* p4 = playerPool.get();
    std::cout << "Player4: health=" << p4->health << ", score=" << p4->score << "\n";
    std::cout << "p4 address: " << p4 << "\n";
    std::cout << "p1 address: " << p1 << " (same? " << (p4 == p1 ? "YES" : "NO") << ")\n";
    
    playerPool.release(p2);
    playerPool.release(p3);
    playerPool.release(p4);
    
    return 0;
}
