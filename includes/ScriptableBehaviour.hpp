#ifndef SCRIPTABLEBEHAVIOUR_HPP
#define SCRIPTABLEBEHAVIOUR_HPP

// #include "Collider.hpp"

class Collider;

class ScriptableBehaviour {

private:

public:
    ScriptableBehaviour();
    ~ScriptableBehaviour();

    //virtual methods
    virtual void        OnColliderEnter(Collider & collider){};
    virtual void        OnColliderStay(Collider & collider){};
    virtual void        OnColliderExit(Collider & collider){};
    virtual void        Update(){};
    virtual void        LateUpdate(){};

    virtual Collider    *GetCollider(){return nullptr;};
};


#endif