#ifndef SHIP_HPP
#define SHIP_HPP

#include "Object.hpp"

#define MAX_HEALTH 30
#define MAX_AMMO 100

class Ship : public Object{

private:

    float       _speed;

public:
    ~Ship();
    Ship();
    Ship(const Object& rhs, bool render = true, bool collide = true);
    Ship(Shader & shader, const char *objFile, bool render = true, bool collide = true);

    int         healthPoint;
    int         ammunitions;

    //derived virtual methods
    virtual void    OnColliderEnter(Collider & collider);
    virtual void    OnColliderStay(Collider & collider);
    virtual void    OnColliderExit(Collider & collider);
    virtual void    Update();
};

#endif