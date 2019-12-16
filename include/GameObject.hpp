#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject {
protected:
public:
    GameObject(){}

    virtual ~GameObject() {}
    virtual void updateValues() = 0;
    virtual void update() = 0;
    virtual void printInfo() = 0;
    virtual void serialize(StateManager *sm) = 0;
    virtual void unserialize(StateManager *sm) = 0;

    typedef GameObject * create_t(int);
    typedef void destroy_t(GameObject *);

};


#endif
