#ifndef PATHGENERATOR_HPP
#define PATHGENERATOR_HPP

#include "Object.hpp"
#include "RenderPipeline.hpp"

#include <list>
#include <time.h>

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
    Transform            _pivotPoint;

    void                SetPositionFromParent(Object &chunk);
    Object              RandomChunkFromLast();
    Object              RandomChunk(Object previousChunk);


public:
    PathGenerator(Shader *shader, unsigned int chunksAmount, float speed);
    ~PathGenerator();

    // void                SwapFirstToLast(RenderPipeline *renderPipeline);
    void                SwapFirstToLast();
    void                MovePath(float deltaTime);
    float               GetChunkLength();
    float               GetHalfChunkLength();

    list<Object>        chunks;
    float               speed;

};

#endif