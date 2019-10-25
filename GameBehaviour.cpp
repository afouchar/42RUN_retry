#include "GameBehaviour.hpp"

//STATIC VARIABLES DEFINITIONS
std::list<Collider *> GameBehaviour::_sceneColliders;


GameBehaviour::~GameBehaviour() {}

GameBehaviour::GameBehaviour() {}

void GameBehaviour::UpdateCollisions(){

    for (list<Collider *>::iterator it_i = GameBehaviour::_sceneColliders.begin(); it_i != GameBehaviour::_sceneColliders.end(); it_i++){
        for (list<Collider *>::iterator it_j = GameBehaviour::_sceneColliders.begin(); it_j != GameBehaviour::_sceneColliders.end(); it_j++){
            bool collisionOccured = (*it_i)->CheckCollision((**it_j));

            if (collisionOccured){
                //check if first time
                // OnColliderEnter();
                //else
                (*it_i)->OnColliderStay();
            }
            else{
                //check if was colliding
                // OnColliderExit();
                //else
                //nothing
            }
        }
    }
}

void GameBehaviour::OnColliderEnter(){

}

void GameBehaviour::OnColliderStay(){
    
}

void GameBehaviour::OnColliderExit(){
    
}