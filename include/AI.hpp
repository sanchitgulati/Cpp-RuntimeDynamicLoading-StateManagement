#ifndef AI_H
#define AI_H
#include "../StateManager.hpp"
#include "Vector2.h"
#include "GameObject.hpp"
#include <string>
#include <sstream>

class AI : public GameObject {
protected:
    int speed;
    Vector2 pos;
public:
    AI(){}

    virtual void updateValues(){
        speed = 0;
        pos = Vector2(0,0);
    };

    virtual ~AI() {}

    void set_speed(int speed) {
        speed = speed;
    }

    void set_position(Vector2 position) {
        pos = position;
    }

    virtual void printInfo(){
        std::cout << typeid(this).name() << " speed " << speed << " x " << pos.x << " y " << pos.y << std::endl;
    }

    virtual void update(){
    }

    //
    // For this POC, serialization & unserialization have been kept ad hoc/ hack.
    // More generic solutions should be used in real-life applications
    //

    virtual void serialize(StateManager *sm){
        std::stringstream ss;
        std::string classname = typeid(this).name();
        ss << speed << ":"<< pos.x << ":" << pos.y;
        std::string s = ss.str();
        sm->save(classname,s);
    }

    virtual void unserialize(StateManager *sm){
        std::string line;
        std::string classname = typeid(this).name();
        std::stringstream str;
        str << sm->load(classname);
        getline (str,line,':');
        speed = std::atoi(line.c_str());
        getline (str,line,':');
        pos.x = std::atoi(line.c_str());
        getline (str,line,':');
        pos.y = std::atoi(line.c_str());
    }
};


#endif
