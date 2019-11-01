#ifndef CHUNKS_HPP
#define CHUNKS_HPP

#include "Object.hpp"

class Chunk : public Object {

private:

public:
    Chunk();
    Chunk(const Object& rhs, bool render);
    Chunk(Shader & shader, const char *objFile, bool render);
    ~Chunk();

    //derived virtual methods
    virtual void    OnColliderEnter(Collider & collider);
    virtual void    OnColliderStay(Collider & collider);
    virtual void    OnColliderExit(Collider & collider);
};

#endif