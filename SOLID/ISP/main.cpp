#include "Bicycle.hpp"
#include "Boat.hpp"
#include "Airplane.hpp"

int main() {
    IDriveable* bicycle = new Bicycle(20.0);
    ISailable* boat = new Boat(15.5);
    Airplane* plane = new Airplane(30.0);

    bicycle->drive();
    boat->sail();
    plane->drive(); 
    plane->fly();

    delete bicycle;
    delete boat;
    delete plane;

    return 0;
}
