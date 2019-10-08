#ifndef PATHGENERATOR_HPP
#define PATHGENERATOR_HPP

#include "Object.hpp"
#include "RenderPipeline.hpp"

using namespace std;
using namespace glm;

class PathGenerator {

private:
    PathGenerator();

    unsigned int        _chunksAmount;
    float               _chunkLength;
    Shader              *_shader;
    Object              pathPart;

public:
    PathGenerator(Shader *shader, unsigned int chunksAmount, float speed);
    ~PathGenerator();

    // void                SwapFirstToLast(RenderPipeline *renderPipeline);
    void                SwapFirstToLast();
    float               GetChunkLength();

    vector<Object>      chunks;
    float        speed;

};

#endif