#ifndef GAMEBEHVIOUR_HPP
#define GAMEBEHVIOUR_HPP


#include "Object.hpp"

#include <glfw3.h>
#include <vector>
#include <deque>
#include <list>

using namespace std;

class GameBehaviour {

private:

    static float _lastDeltaTime;
    static float _deltaTime;
    static float _FPS;

    static void     ComputeDeltaTime();
    static void     ComputeFPS();

protected:

    static list<Collider *> _sceneColliders;
    // static vector<vector<bool>> _collisionMap;
    static deque<deque<bool>> _collisionMap;

    // virtual void    OnColliderEnter(Collider & collider);
    // virtual void    OnColliderStay(Collider & collider);
    // virtual void    OnColliderExit(Collider & collider);

public:

    GameBehaviour();
    ~GameBehaviour();

    static void     UpdateCollisions();
    static void     AddCollider(Collider & collider);
    static void     RemoveCollider(Collider & collider);
    static void     Clock();
    static float    DeltaTime();

};

#endif

/*
Collision Map

  | A | B | C | D
--+---+---+---+--
D | 0 | 0 | 0 | .
--+---+---+---+--
C | 0 | 0 | . |  
--+---+---+---+--
B | 0 | . |   |  
--+---+---+---+--
A | . |   |   |  

*/