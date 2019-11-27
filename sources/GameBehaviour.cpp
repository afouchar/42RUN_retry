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

Object *GameBehaviour::Raycast(vec3 origin, vec3 direction, float maxDistance){

    for (int pos_a = 0; pos_a < GameBehaviour::_collisionMap.size(); pos_a++){

            list<Collider *>::iterator col_a = next(GameBehaviour::_sceneColliders.begin(), pos_a);

            float distance = glm::distance((*col_a)->transform->WorldPosition(), origin);

            if ((*col_a)->isTrigger || distance > maxDistance)
                continue;

            if (OBBRayIntersection(origin, direction, maxDistance, (**col_a)))
                return (*col_a)->transform->gameObject;
    }
    return nullptr;
}

Object *GameBehaviour::Raycast(vec3 origin, vec3 direction, float maxDistance, string byTag) {

    for (int pos_a = 0; pos_a < GameBehaviour::_collisionMap.size(); pos_a++){

            list<Collider *>::iterator col_a = next(GameBehaviour::_sceneColliders.begin(), pos_a);

            float distance = glm::distance((*col_a)->transform->WorldPosition(), origin);

            if ((*col_a)->isTrigger || distance > maxDistance || (*col_a)->transform->GetTag() != byTag){
                continue;
            }

            if (OBBRayIntersection(origin, direction, maxDistance, (**col_a)))
                return (*col_a)->transform->gameObject;
    }
    return nullptr;
}

Object *GameBehaviour::Raycast(vec3 origin, vec3 direction, float maxDistance, string byTag, string byTagAlt) {

    for (int pos_a = 0; pos_a < GameBehaviour::_collisionMap.size(); pos_a++){

            list<Collider *>::iterator col_a = next(GameBehaviour::_sceneColliders.begin(), pos_a);

            float distance = glm::distance((*col_a)->transform->WorldPosition(), origin);

            if ((*col_a)->isTrigger || distance > maxDistance){
                continue;
            }
            if ((*col_a)->transform->GetTag() == byTag || (*col_a)->transform->GetTag() == byTagAlt){
                if (OBBRayIntersection(origin, direction, maxDistance, (**col_a)))
                    return (*col_a)->transform->gameObject;
            }
    }
    return nullptr;
}

bool GameBehaviour::OBBRayIntersection(vec3 origin, vec3 direction, float maxDistance, Collider & collider) {

    float tMin = 0.0f;
    float tMax = maxDistance;

    collider.UpdateCollider();
    collider.bound.UpdateBoundValues((*collider.transform));

    vec3 OBBposition_worldspace = collider.GetOffsetWorldPosition();
    mat4 modelMatrix = collider.transform->modelMatrix;
    vec3 delta = OBBposition_worldspace - origin;

    vec3 xAxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
    vec3 yAxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
    vec3 zAxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);

    vec3 boundMin = (collider.bound.min * collider.bound.scale) * collider.bound.scale;
    vec3 boundMax = (collider.bound.max * collider.bound.scale) * collider.bound.scale;

    if (!OBBRayIntersectPlane(boundMin.x, boundMax.x, tMin, tMax, direction, xAxis, delta))
        return false;
    if (!OBBRayIntersectPlane(boundMin.y, boundMax.y, tMin, tMax, direction, yAxis, delta))
        return false;
    if (!OBBRayIntersectPlane(boundMin.z, boundMax.z, tMin, tMax, direction, zAxis, delta))
        return false;

    return true;
}

bool GameBehaviour::OBBRayIntersectPlane(float boundMin, float boundMax, float distMin, float distMax, vec3 direction, vec3 planeAxis, vec3 delta){

    float e = glm::dot(planeAxis, delta);
    float f = glm::dot(direction, planeAxis);

    if (fabs(f) > 0.001f){

        float t1 = (e + boundMin) / f;
        float t2 = (e + boundMax) / f;

        if (t1 > t2){
            std::swap(t1, t2);
        }

        if (t2 < distMax)
            distMax = t2;
        if (t1 > distMin)
            distMin = t1;

        if (distMax < distMin)
            return false;
    }
    else {
        if(-e + boundMin > 0.0f || -e + boundMax < 0.0f)
            return false;
    }
    return true;
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