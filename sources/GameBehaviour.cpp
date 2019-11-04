#include "GameBehaviour.hpp"

//STATIC VARIABLES DEFINITIONS
std::list<Collider *> GameBehaviour::_sceneColliders;
std::vector<vector<bool>> GameBehaviour::_collisionMap;
float GameBehaviour::_deltaTime = 0.0f;
float GameBehaviour::_lastDeltaTime = -1.0f;
float GameBehaviour::_FPS = 0.0f;

GameBehaviour::~GameBehaviour() {}

GameBehaviour::GameBehaviour() {}

void GameBehaviour::UpdateCollisions(){

    for (list<Collider *>::iterator col_a = GameBehaviour::_sceneColliders.begin(); col_a != GameBehaviour::_sceneColliders.end(); col_a++){
        for (list<Collider *>::iterator col_b = GameBehaviour::_sceneColliders.begin(); col_b != GameBehaviour::_sceneColliders.end(); col_b++){
            if ((*col_a)->isTrigger || (*col_b)->isTrigger || (*col_a) == (*col_b))
                continue;
            bool collisionOccured = (*col_a)->CheckCollision((**col_b));

            int pos_a = std::distance(GameBehaviour::_sceneColliders.begin(), col_a);
            int pos_b = std::distance(GameBehaviour::_sceneColliders.begin(), col_b);

            if (collisionOccured){
                if (!GameBehaviour::_collisionMap[pos_a][pos_b] || !GameBehaviour::_collisionMap[pos_b][pos_a]){
                    (*col_a)->transform->gameObject->OnColliderEnter((**col_b));
                    (*col_b)->transform->gameObject->OnColliderEnter((**col_a));
                }
                else{
                    (*col_a)->transform->gameObject->OnColliderStay((**col_b));
                    (*col_b)->transform->gameObject->OnColliderStay((**col_a));
                }
            }
            else{
                if (GameBehaviour::_collisionMap[pos_a][pos_b] || GameBehaviour::_collisionMap[pos_b][pos_a])
                    (*col_a)->transform->gameObject->OnColliderExit((**col_b));
                    (*col_b)->transform->gameObject->OnColliderExit((**col_a));
            }
            //// !!! checking collision twice (not really optimized) !!! ////
            GameBehaviour::_collisionMap[pos_a][pos_b] = collisionOccured;
            GameBehaviour::_collisionMap[pos_b][pos_a] = collisionOccured;
        }
    }
}

void GameBehaviour::AddCollider(Collider & collider){

    GameBehaviour::_sceneColliders.push_back(&collider);
    GameBehaviour::_collisionMap.push_back(std::vector<bool>());
    for (int i = 0; i < GameBehaviour::_collisionMap.size(); i++){
        GameBehaviour::_collisionMap[GameBehaviour::_collisionMap.size() - 1].push_back(false);
        if (i < GameBehaviour::_collisionMap.size() - 1)
            GameBehaviour::_collisionMap[i].push_back(false);
    }
}

void GameBehaviour::RemoveCollider(Collider & collider){

    std::list<Collider *>::iterator it = std::find(GameBehaviour::_sceneColliders.begin(), GameBehaviour::_sceneColliders.end(), &collider);
    int position = std::distance(GameBehaviour::_sceneColliders.begin(), it);
    GameBehaviour::_sceneColliders.erase(it);

    GameBehaviour::_collisionMap.erase(GameBehaviour::_collisionMap.begin() + position);
    for (int i = 0; i < GameBehaviour::_collisionMap.size(); i++) {
        GameBehaviour::_collisionMap[i].erase(GameBehaviour::_collisionMap[i].begin() + position);
    }
}

void GameBehaviour::Clock(){
    GameBehaviour::ComputeDeltaTime();
    GameBehaviour::ComputeFPS();
}

void GameBehaviour::ComputeDeltaTime(){

    if (GameBehaviour::_lastDeltaTime < 0)
        GameBehaviour::_lastDeltaTime = glfwGetTime();

    double currentTime = glfwGetTime();
	GameBehaviour::_deltaTime = float(currentTime - GameBehaviour::_lastDeltaTime);
    GameBehaviour::_lastDeltaTime = currentTime;
}

void GameBehaviour::ComputeFPS(){
    GameBehaviour::_FPS = (1.0f / GameBehaviour::_deltaTime) * 1000.0f;
}

float GameBehaviour::DeltaTime(){
    return GameBehaviour::_deltaTime;
}