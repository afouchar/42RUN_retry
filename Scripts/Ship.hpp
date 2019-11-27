#ifndef SHIP_HPP
#define SHIP_HPP


#include "Bullet.hpp"
#include <cmath>

#define MAX_HEALTH 30
#define MAX_AMMO 100
#define SHOOT_COOLDOWN 0.20f

class Ship : public Object{

private:

    float           _speed;
    Bullet          *_bullet;
    float           _shootTimer;
    float           _totalScore;
    bool            _allowShoot;

public:
    ~Ship();
    Ship();
    Ship(const Object & rhs, bool render = true, bool collide = true);
    Ship(Shader & shader, const char *objFile, bool render = true, bool collide = true);

    static Ship     *instance;
    int             healthPoint;
    int             ammunitions;
    int             score;;

    void            Damage();
    void            Heal();
    void            ReloadAmmunitions();
    void            Shoot();
    void            Score(float addValue);

    //derived virtual methods
    virtual void    OnColliderEnter(Collider & collider);
    virtual void    OnColliderStay(Collider & collider);
    virtual void    OnColliderExit(Collider & collider);
    virtual void    Update();
};

#endif