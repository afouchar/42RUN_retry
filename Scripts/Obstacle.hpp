#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "Object.hpp"


class Obstacle : public Object {

private:

public:
    Obstacle();
    Obstacle(const Object& rhs, bool render = true, bool collide = true);
    Obstacle(Shader & shader, const char *objFile, bool render = true, bool collide = true);
    ~Obstacle();



    //derived virtual methods
    virtual void    OnColliderEnter(Collider & collider);
    virtual void    OnColliderStay(Collider & collider);
    virtual void    OnColliderExit(Collider & collider);
    virtual void    Update();
    virtual void    LateUpdate();
};

#endif