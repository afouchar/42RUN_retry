#ifndef GAMEBEHVIOUR_HPP
#define GAMEBEHVIOUR_HPP

// #include "Collider.hpp"
#include "Object.hpp"

#include <vector>
#include <list>

using namespace std;

class GameBehaviour {

private:


protected:

    static list<Collider *> _sceneColliders;
    static vector<vector<bool>> _collisionMap;

    // virtual void    OnColliderEnter(Collider & collider);
    // virtual void    OnColliderStay(Collider & collider);
    // virtual void    OnColliderExit(Collider & collider);

public:

    GameBehaviour();
    ~GameBehaviour();

    static void UpdateCollisions();
    static void AddCollider(Collider & collider);
    static void RemoveCollider(Collider & collider);

};

#endif