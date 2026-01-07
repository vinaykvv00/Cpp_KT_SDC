#include <iostream>
using namespace std;

class Rectangle {
public:
    virtual void setWidth(int w) {
        width = w;
    }

    virtual void setHeight(int h) {
        height = h;
    }

    int area() const {
        return width * height;
    }

protected:
    int width = 0;
    int height = 0;
};

class Square : public Rectangle {
public:
    void setWidth(int w) override {
        width = height = w;      
    }

    void setHeight(int h) override {
        width = height = h;    
    }
};

void testShape(Rectangle* r) {
    r->setWidth(4);
    r->setHeight(5);

    cout << "Expected area = 20, Got: " << r->area() << endl;
}

int main() {
    Rectangle rect;
    Square sq;

    cout << "--Rectangle Test--\n";
    testShape(&rect); 

    cout << "\n--Square Test--\n";
    testShape(&sq);    

    return 0;
}
