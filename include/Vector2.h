#ifndef VECTOR2_H
#define VECTOR2_H


struct Vector2{
    public:
    int x,y;

    Vector2(){
        x = 0,y = 0;
    }

    Vector2(int x){
        this->x = x;
        this->y = x;
    }
    Vector2(int x,int y){
        this->x = x;
        this->y = y;
    }
    const Vector2 operator + (Vector2 const &v2){
        Vector2 ret = Vector2();
        ret.x = x + v2.x;
        ret.y = y + v2.y;
        return ret;
    }
};

#endif