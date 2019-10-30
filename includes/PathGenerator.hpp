#ifndef PATHGENERATOR_HPP
#define PATHGENERATOR_HPP

#include "Object.hpp"

#include <time.h>
// #include <list>

using namespace std;
using namespace glm;

class PathGenerator {

private:
    PathGenerator();

    unsigned int        _chunksAmount;
    float               _chunkLength;
    Shader              *_shader;
    Object              pathForward;
    Object              pathTurn;
    //temp variables
    float               pathAngle;
    bool                firstTimeIn;

    void                SetPositionFromParent(Object &chunk);
    Object              RandomChunkFromLast();
    Object              RandomChunk(Object previousChunk);

public:
    PathGenerator(Shader *shader, unsigned int chunksAmount, float speed);
    ~PathGenerator();

    void                SwapFirstToLast();
    void                MovePath(Object &player, float deltaTime);
    float               GetChunkLength();
    float               GetHalfChunkLength();

    list<Object>        chunks;
    float               speed;

};

#endif