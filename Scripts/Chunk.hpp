#ifndef CHUNKS_HPP
#define CHUNKS_HPP

#include "Object.hpp"

class PathGenerator;

class Chunk : public Object {

private:

    PathGenerator   *_generator;

    void            SetPivot();
    static vec3     _pivot;
    static vec3     _upAxis;
    static float    _totalRotation;
    static bool     _allowSwap;

public:
    Chunk();
    Chunk(PathGenerator & generator, const Object& rhs, bool render = true, bool collide = true);
    Chunk(PathGenerator & generator, Shader & shader, const char *objFile, bool render = true, bool collide = true);
    ~Chunk();

    static bool     isTurning;
    static bool     isMoving;
    bool            isSelfTurning;
    vec3            cleanPosition;


    void            Move();
    void            Turn();

    //derived virtual methods
    virtual void    OnColliderEnter(Collider & collider);
    virtual void    OnColliderStay(Collider & collider);
    virtual void    OnColliderExit(Collider & collider);
    virtual void    Update();

};

#endif