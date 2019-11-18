#include "GameBehaviour.hpp"
#include "ScriptableBehaviour.hpp"

//STATIC VARIABLES DEFINITIONS
// std::list<Object *> GameBehaviour::_sceneObjects;
std::list<ScriptableBehaviour *> GameBehaviour::_sceneObjects;
std::list<Collider *> GameBehaviour::_sceneColliders;
std::deque<deque<bool>> GameBehaviour::_collisionMap;
float GameBehaviour::_deltaTime = 0.0f;
float GameBehaviour::_lastDeltaTime = -1.0f;
float GameBehaviour::_FPS = 0.0f;
Input *GameBehaviour::input = nullptr;
Window *GameBehaviour::window = nullptr;

GameBehaviour::~GameBehaviour() {}

GameBehaviour::GameBehaviour() {}

void  GameBehaviour::CreateContext(const char *windowTitle, vec2 windowSize){

    if (GameBehaviour::window == nullptr)
        GameBehaviour::window = new Window(windowTitle, windowSize);
    if (GameBehaviour::input == nullptr)
        GameBehaviour::input = new Input((*GameBehaviour::window->GetWindow()));
}

void GameBehaviour::BeginFrame(){
    if (GameBehaviour::window != nullptr)
        GameBehaviour::window->Clear();
}

void GameBehaviour::EndFrame(){
    if (GameBehaviour::window != nullptr)
        GameBehaviour::window->SwapBuffer();
    if (GameBehaviour::input != nullptr)
        GameBehaviour::input->PollEvents();
}

void GameBehaviour::Terminate(){
    if (GameBehaviour::input != nullptr)
    	GameBehaviour::input->Terminate();
    if (GameBehaviour::window != nullptr)
    	GameBehaviour::window->Terminate();   
}

void GameBehaviour::UpdateCollisions(){

    for (int pos_a = 0; pos_a < GameBehaviour::_collisionMap.size(); pos_a++){

            list<Collider *>::iterator col_a = next(GameBehaviour::_sceneColliders.begin(), pos_a);

            if ((*col_a)->isTrigger)
                continue;

        for (int pos_b = 0; pos_b < GameBehaviour::_collisionMap.at(pos_a).size(); pos_b++){

            list<Collider *>::iterator col_b = prev(GameBehaviour::_sceneColliders.end(), pos_b + 1);
            
            if ((*col_b)->isTrigger || (*col_a) == (*col_b))
                continue;

            bool collisionOccured = (*col_a)->CheckCollision((**col_b));

            if (collisionOccured){
                if (!GameBehaviour::_collisionMap[pos_a][pos_b]){
                    (*col_a)->transform->gameObject->OnColliderEnter((**col_b));
                    (*col_b)->transform->gameObject->OnColliderEnter((**col_a));
                }
                else {
                    (*col_a)->transform->gameObject->OnColliderStay((**col_b));
                    (*col_b)->transform->gameObject->OnColliderStay((**col_a));
                }
            }
            else {
                if (GameBehaviour::_collisionMap[pos_a][pos_b]){
                    (*col_a)->transform->gameObject->OnColliderExit((**col_b));
                    (*col_b)->transform->gameObject->OnColliderExit((**col_a));
                }
            }
            GameBehaviour::_collisionMap[pos_a][pos_b] = collisionOccured;
        }
    }
}

void GameBehaviour::UpdateObjectScripts(){

    for (std::list<ScriptableBehaviour *>::iterator it = GameBehaviour::_sceneObjects.begin();  it != GameBehaviour::_sceneObjects.end(); it++){
        (*it)->Update();
    }
}

void GameBehaviour::LateUpdateObjectScripts(){

    for (std::list<ScriptableBehaviour *>::iterator it = GameBehaviour::_sceneObjects.begin();  it != GameBehaviour::_sceneObjects.end(); it++){
        (*it)->LateUpdate();
    }
}

void GameBehaviour::AddObject(ScriptableBehaviour & object){

    GameBehaviour::_sceneObjects.push_back(&object);
}

void GameBehaviour::RemoveObject(ScriptableBehaviour & object){

    std::list<ScriptableBehaviour *>::iterator it = std::find(GameBehaviour::_sceneObjects.begin(), GameBehaviour::_sceneObjects.end(), &object);
    if (it == GameBehaviour::_sceneObjects.end())
        return;

    Collider *col = object.GetCollider();
    if (col != nullptr){
        GameBehaviour::RemoveCollider((*col));
    }
    GameBehaviour::_sceneObjects.erase(it);
}

void GameBehaviour::AddCollider(Collider & collider){

    GameBehaviour::_sceneColliders.push_back(&collider);
    GameBehaviour::_collisionMap.push_back(deque<bool>());

    for (int i = 0; i < GameBehaviour::_collisionMap.size(); i++){
        GameBehaviour::_collisionMap.at(i).push_front(false);
    }
}

void GameBehaviour::RemoveCollider(Collider & collider){

    std::list<Collider *>::iterator it = std::find(GameBehaviour::_sceneColliders.begin(), GameBehaviour::_sceneColliders.end(), &collider);
    if (it == GameBehaviour::_sceneColliders.end())
        return;

    int position = std::distance(GameBehaviour::_sceneColliders.begin(), it);

    int reversePosition = GameBehaviour::_sceneColliders.size() - position;

    GameBehaviour::_sceneColliders.erase(it);
    GameBehaviour::_collisionMap.erase(GameBehaviour::_collisionMap.begin() + position);

    for (int i = 0; i < GameBehaviour::_collisionMap.size(); i++) {
        if (reversePosition < GameBehaviour::_collisionMap[i].size())
            GameBehaviour::_collisionMap[i].erase(GameBehaviour::_collisionMap[i].begin() + reversePosition);
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