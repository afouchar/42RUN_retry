#ifndef GAMEBEHVIOUR_HPP
#define GAMEBEHVIOUR_HPP


#include "Object.hpp"
#include "Input.hpp"
#include "Window.hpp"

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

    static list<Object *> _sceneObjects;
    static list<Collider *> _sceneColliders;
    static deque<deque<bool>> _collisionMap;

public:

    GameBehaviour();
    ~GameBehaviour();

    static Input    *input;
    static Window   *window;

    static void     CreateContext(const char *windowTitle, vec2 windowSize);
    static void     BeginFrame();
    static void     EndFrame();
    static void     Terminate();
    static void     UpdateObjectScripts();
    static void     AddObject(Object & object);
    static void     RemoveObject(Object & object);
    static void     UpdateCollisions();
    static void     AddCollider(Collider & collider);
    static void     RemoveCollider(Collider & collider);
    static void     Clock();
    static float    DeltaTime();

};

#endif

/*
Collision Map example

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