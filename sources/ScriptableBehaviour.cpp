#include "ScriptableBehaviour.hpp"
#include "GameBehaviour.hpp"

ScriptableBehaviour::~ScriptableBehaviour(){
    GameBehaviour::RemoveObject((*this));
}

ScriptableBehaviour::ScriptableBehaviour(){
    GameBehaviour::AddObject((*this));
}