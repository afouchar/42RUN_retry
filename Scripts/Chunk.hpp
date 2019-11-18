#ifndef CHUNKS_HPP
#define CHUNKS_HPP

#include "Obstacle.hpp"
//#include "Object.hpp"

class PathGenerator;

class Chunk : public Object {

private:

    PathGenerator       *_generator;
    static vec3         _pivot;
    static vec3         _upAxis;
    static vec3         _leftAxis;
    static vec3         _endPosition;
    static float        _totalRotation;
    static bool         _allowSwap;

    void                SetPivot();

public:
    Chunk();
    Chunk(PathGenerator & generator, const Object& rhs, bool render = true, bool collide = true);
    Chunk(PathGenerator & generator, Shader & shader, const char *objFile, bool render = true, bool collide = true);
    ~Chunk();

    static bool         isTurning;
    static bool         isMoving;
    bool                isSelfTurning;
    vec3                cleanPosition;
    list<Obstacle *>    obstacles;

    void                Move();
    void                Turn();
    void                GenerateObstacles(unsigned int min, unsigned int max);
    void                ClearObstacles();

    //derived virtual methods
    virtual void        OnColliderEnter(Collider & collider);
    virtual void        OnColliderStay(Collider & collider);
    virtual void        OnColliderExit(Collider & collider);
    virtual void        Update();
    virtual void        LateUpdate(){};

};

#endif