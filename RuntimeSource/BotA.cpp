#include "../include/AI.hpp"
#include <cmath>
#include <iostream>


class BotA : public AI {
public:
    
    BotA(int speed){
        this->speed = speed;
        pos = Vector2(0,0);
    }

    virtual void updateValues() {
        speed = 3;
    }

    virtual void update(){
        pos.x += speed;
        pos.y += speed;
    }

};


// the class factories
extern "C" AI* create(int x) {
    return new BotA(x);
}

extern "C" void destroy(AI* p) {
    delete p;
}
