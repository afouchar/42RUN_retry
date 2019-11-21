#include "ScriptableBehaviour.hpp"
#include "GameBehaviour.hpp"

ScriptableBehaviour::~ScriptableBehaviour(){
    // std::cerr << "ScriptableBehaviour DESTROY " << __PRETTY_FUNCTION__ << std::endl;
    GameBehaviour::RemoveObject((*this));
}

ScriptableBehaviour::ScriptableBehaviour(){
    GameBehaviour::AddObject((*this));
}