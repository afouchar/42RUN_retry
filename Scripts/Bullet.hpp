#ifndef BULLET_HPP
#define BULLET_HPP

#include "Object.hpp"

// #define MAX_DISTANCE 80

class Bullet : public Object {

private:

    float       _speed;
    bool        _delete;

public:
    ~Bullet();
    Bullet();
    Bullet(const Object& rhs, bool render = true, bool collide = true);
    Bullet(Shader & shader, const char *objFile, bool render = true, bool collide = true);


    //derived virtual methods
    virtual void    OnColliderEnter(Collider & collider);
    virtual void    OnColliderStay(Collider & collider);
    virtual void    OnColliderExit(Collider & collider);
    virtual void    Update();
    virtual void    LateUpdate();
};


#endif