#include "Ship.hpp"
#include "GameBehaviour.hpp"
#include "RenderPipeline.hpp"

Ship *Ship::instance = nullptr;


Ship::~Ship(){}

Ship::Ship(){

    if (Ship::instance == nullptr)
        Ship::instance = this;
}

Ship::Ship(const Object& rhs, bool render, bool collide) : Object(rhs, render, collide){

    if (Ship::instance == nullptr)
        Ship::instance = this;

    this->_speed = 80.0f;
    this->healthPoint = MAX_HEALTH;
    this->ammunitions = MAX_AMMO;
    this->_shootTimer = SHOOT_COOLDOWN;
    this->_totalScore = 0;
    this->score = 0;
    this->_allowShoot = true;
    this->_bullet = new Bullet((*this->shader), "Models/Colliders/rocket.obj", false, false);
    this->_bullet->transform.SetTag("Bullet");
}

Ship::Ship(Shader & shader, const char *objFile, bool render, bool collide) : Object(shader, objFile, render, collide){

    if (Ship::instance == nullptr)
        Ship::instance = this;

    this->_speed = 80.0f;
    this->healthPoint = MAX_HEALTH;
    this->ammunitions = MAX_AMMO;
    this->_shootTimer = SHOOT_COOLDOWN;
    this->_totalScore = 0;
    this->score = 0;
    this->_allowShoot = true;
    this->_bullet = new Bullet((*this->shader), "Models/Colliders/rocket.obj", false, false);
    this->_bullet->transform.SetTag("Bullet");
}

void Ship::OnColliderEnter(Collider & collider){

    if (collider.transform->GetTag() == "Turn")
        this->_allowShoot = false;

    if (collider.transform->GetTag() == "Obstacle"){
        // std::cerr << "Player Collide with Obstacle [" << collider.transform->gameObject->ID << "] __ scale : " << collider.bound.scale.x << " | " << collider.bound.scale.y << " | " << collider.bound.scale.z << std::endl;
        Damage();
    }
    else if (collider.transform->GetTag() == "Heal"){
        collider.isTrigger = true;
        RenderPipeline::RemoveObject((*collider.transform->gameObject), false);
        Heal();
    }
    else if (collider.transform->GetTag() == "Ammo"){
        collider.isTrigger = true;
        RenderPipeline::RemoveObject((*collider.transform->gameObject), false);
        ReloadAmmunitions();
    }
}

void Ship::OnColliderStay(Collider & collider){}

void Ship::OnColliderExit(Collider & collider){
    if (collider.transform->GetTag() == "Turn")
        this->_allowShoot = true;
}

void Ship::Update(){

    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_A)){
        this->transform.RotateAround(vec3_zero, vec3_forward, this->_speed * GameBehaviour::DeltaTime());
    }
    if (GameBehaviour::input->GetKeyPressed(GLFW_KEY_D)){
        this->transform.RotateAround(vec3_zero, vec3_back, this->_speed * GameBehaviour::DeltaTime());
    }
    if (this->_allowShoot && GameBehaviour::input->GetKeyPressed(GLFW_KEY_SPACE)){
        Shoot();
    }
    else if (GameBehaviour::input->GetKeyReleased(GLFW_KEY_SPACE)){
        this->_shootTimer = SHOOT_COOLDOWN;
    }

    Score(GameBehaviour::DeltaTime());
}

void Ship::Damage(){

    this->healthPoint--;
    this->healthPoint = this->healthPoint <= 0 ? 0 : this->healthPoint;
    if (this->healthPoint > 0){
        std::cerr << "PAF ! (" << this->healthPoint << "/" << MAX_HEALTH << ")" << std::endl;
    }
    else{
        std::cerr << "YOU ARE DEAD (" << this->healthPoint << "/" << MAX_HEALTH << ")" << std::endl;
        //call death screen
    }
}

void Ship::Heal(){
    
    this->healthPoint += 10;
    this->healthPoint = this->healthPoint > MAX_HEALTH ? MAX_HEALTH : this->healthPoint;
    if (this->healthPoint >= MAX_HEALTH)
        std::cerr << "MAX HEALTH REACHED ! (" << this->healthPoint << "/" << MAX_HEALTH << ")" << std::endl;
    else
        std::cerr << "HEAL ! (" << this->healthPoint << "/" << MAX_HEALTH << ")" << std::endl;
    Score(3.0f);
    std::cerr << "____________________________Score : " << this->_totalScore << " -> " << this->score << std::endl;
}

void Ship::ReloadAmmunitions(){

    this->ammunitions += 20;
    this->ammunitions = this->ammunitions > MAX_AMMO ? MAX_AMMO : this->ammunitions;
    if (this->ammunitions >= MAX_AMMO)
        std::cerr << "MAX AMMO REACHED ! (" << this->ammunitions << "/" << MAX_AMMO << ")" << std::endl;
    else
        std::cerr << "RELOAD ! (" << this->ammunitions << "/" << MAX_AMMO << ")" << std::endl;
    Score(3.0f);
    std::cerr << "____________________________Score : " << this->_totalScore << " -> " << this->score << std::endl;
}

void Ship::Shoot(){
    

    // Object *rayObject = GameBehaviour::Raycast(this->transform.WorldPosition(), this->transform.Forward(), 100.0f, "Obstacle");
    // if (rayObject != nullptr)
    //     std::cerr << "RAYCAST OBJECT ----------> [" << rayObject->ID << "] " << rayObject->GetTag() << std::endl;
    // else
    //     std::cerr << "NO RAYCAST !!!" << std::endl;

    this->_shootTimer += GameBehaviour::DeltaTime();

    if (this->_shootTimer >= SHOOT_COOLDOWN){
        this->_shootTimer = 0;

        if (this->ammunitions > 0){
            // this->ammunitions--;
            Bullet *shoot = new Bullet((*this->_bullet));
            shoot->transform.position = (this->transform.position + (vec3_forward * 2.0f));
            shoot->transform.scale = vec3_one * 3.0f;

            std::cerr << "PIOU ! (" << this->ammunitions << "/" << MAX_AMMO << ")" << std::endl;
        }
        else
            std::cerr << "YOU NEED TO RELOAD ! (" << this->ammunitions << "/" << MAX_AMMO << ")" << std::endl;
    }
}

void Ship::Score(float addValue){

    this->_totalScore += addValue;
    this->score = floor(this->_totalScore);
    // std::cerr << "Score : " << this->_totalScore << " -> " << this->score << std::endl;
}