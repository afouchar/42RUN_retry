#ifndef PATHGENERATOR_HPP
#define PATHGENERATOR_HPP

#include "ScriptableBehaviour.hpp"
#include "Chunk.hpp"
#include "Object.hpp"

#include <time.h>


using namespace std;
using namespace glm;

class PathGenerator : public ScriptableBehaviour{

private:
    PathGenerator();

    unsigned int        _chunksAmount;
    float               _chunkLength;
    Shader              *_shader;
    Chunk               *_pathForward;
    Chunk               *_pathTurn;
    Object              *_colliderCenter;
    int                 _chunksSwapped;

    //temp variables
    // float               pathAngle;
    // bool                firstTimeIn;

    void                SetPositionFromParent(Chunk & chunk);
    Chunk              *RandomChunkFromLast();
    Chunk              *RandomChunk(Chunk & previousChunk);

public:
    PathGenerator(Shader & shader, unsigned int chunksAmount, float speed);
    ~PathGenerator();

    void                SwapFirstToLast();
    float               GetChunkLength();
    float               GetHalfChunkLength();

    list<Chunk *>       chunks;
    float               speed;
    int                 chunksBeforeSwap;

    //derived virtual methods
    virtual void    OnColliderEnter(Collider & collider){};
    virtual void    OnColliderStay(Collider & collider){};
    virtual void    OnColliderExit(Collider & collider){};
    virtual void    Update();
    virtual void    LateUpdate(){};
};

#endif