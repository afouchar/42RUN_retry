#ifndef PATHGENERATOR_HPP
#define PATHGENERATOR_HPP

#include "ScriptableBehaviour.hpp"
#include "Chunk.hpp"
#include "Object.hpp"
#include "Obstacle.hpp"

#include <time.h>

#define MAX_MOVE_SPEED 120.0f


using namespace std;
using namespace glm;

class PathGenerator : public ScriptableBehaviour{

private:
    PathGenerator();

    unsigned int        _chunksAmount;
    float               _chunkLength;
    Shader              *_shader;
    Object              *_colliderCenter;
    int                 _chunksSwapped;
    float               _lastTurnAngle;
    float               _oldSpeed;
    bool                _pauseMove;

    Chunk               *_pathForward;
    Chunk               *_pathTurn;

    Obstacle            *_obstacleType;
    Obstacle            *_ammoType;
    Obstacle            *_healType;

    void                SetPositionFromParent(Chunk & chunk);
    Chunk               *RandomChunkFromLast();
    Chunk               *RandomChunk(Chunk & previousChunk);
    float               GetAngleOutOfRange(float baseAngle, float range);
    void                ClearChildrenOf(Chunk & chunk, int fromChildIndex);

public:
    PathGenerator(Shader & shader, unsigned int chunksAmount, float speed);
    ~PathGenerator();

    void                PauseMove();
    void                StopMove();
    void                ResumeMove();
    void                AddMoveSpeed(float addSpeed);
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