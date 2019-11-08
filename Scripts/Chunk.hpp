#ifndef CHUNKS_HPP
#define CHUNKS_HPP

#include "Object.hpp"

class PathGenerator;

class Chunk : public Object {

private:

    PathGenerator   *_generator;

    void            SetPivot();

public:
    Chunk();
    Chunk(PathGenerator & generator, const Object& rhs, bool render = true, bool collide = true);
    Chunk(PathGenerator & generator, Shader & shader, const char *objFile, bool render = true, bool collide = true);
    ~Chunk();

    bool            isTurning;
    static bool     isMoving;

    void            Move();
    void            Turn();

    //derived virtual methods
    virtual void    OnColliderEnter(Collider & collider);
    virtual void    OnColliderStay(Collider & collider);
    virtual void    OnColliderExit(Collider & collider);
    virtual void    Update();

};

#endif