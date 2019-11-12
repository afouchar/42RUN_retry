#ifndef PATHGENERATOR_HPP
#define PATHGENERATOR_HPP

#include "Chunk.hpp"
#include "Object.hpp"

#include <time.h>


using namespace std;
using namespace glm;

class PathGenerator {

private:
    PathGenerator();

    unsigned int        _chunksAmount;
    float               _chunkLength;
    Shader              *_shader;
    Chunk               *_pathForward;
    Chunk               *_pathTurn;
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
};

#endif