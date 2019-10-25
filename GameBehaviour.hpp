#ifndef GAMEBEHVIOUR_HPP
#define GAMEBEHVIOUR_HPP

#include "Collider.hpp"

class GameBehaviour {

private:


protected:

    static list<Collider *> _sceneColliders;

    virtual void    OnColliderEnter();
    virtual void    OnColliderStay();
    virtual void    OnColliderExit();

public:

    GameBehaviour();
    ~GameBehaviour();

    static void UpdateCollisions();

};

#endif