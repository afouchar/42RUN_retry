#ifndef SHIP_HPP
#define SHIP_HPP

#include "Object.hpp"

class Ship : public Object{

private:

    float       _speed;

public:
    ~Ship();
    Ship();
    Ship(const Object& rhs, bool render = true, bool collide = true);
    Ship(Shader & shader, const char *objFile, bool render = true, bool collide = true);

    //derived virtual methods
    virtual void    OnColliderEnter(Collider & collider);
    virtual void    OnColliderStay(Collider & collider);
    virtual void    OnColliderExit(Collider & collider);
    virtual void    Update();
};

#endif