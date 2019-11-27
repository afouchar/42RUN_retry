#ifndef GAMEBEHVIOUR_HPP
#define GAMEBEHVIOUR_HPP


#include "Object.hpp"
// #include "ScriptableBehaviour.hpp"
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

    static bool     OBBRayIntersection(vec3 origin, vec3 direction, float maxDistance, Collider & collider);
    static bool     OBBRayIntersectPlane(float min, float max, float distMin, float distMax, vec3 direction, vec3 planeAxis, vec3 delta); 
    static void     ComputeDeltaTime();
    static void     ComputeFPS();

protected:

    // static list<Object *> _sceneObjects;
    static list<ScriptableBehaviour *> _sceneObjects;
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
    static void     LateUpdateObjectScripts();
    static void     AddObject(ScriptableBehaviour & object);
    static void     RemoveObject(ScriptableBehaviour & object);

    static void     UpdateCollisions();
    static void     AddCollider(Collider & collider);
    static void     RemoveCollider(Collider & collider);

    static Object   *Raycast(vec3 origin, vec3 direction, float maxDistance);
    static Object   *Raycast(vec3 origin, vec3 direction, float maxDistance, string byTag);
    static Object   *Raycast(vec3 origin, vec3 direction, float maxDistance, string byTag, string byTagAlt);

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