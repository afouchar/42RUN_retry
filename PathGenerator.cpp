#include "PathGenerator.hpp"

PathGenerator::~PathGenerator(){}

PathGenerator::PathGenerator(){}

PathGenerator::PathGenerator(Shader *shader, unsigned int chunksAmount, float speed){

    if (chunksAmount < 3)
        chunksAmount = 3;

    this->_chunksAmount = chunksAmount;
    this->speed = speed;
    this->_shader = shader;

    //GET LENGTH CHUNK PROPERLY !!!
    this->_chunkLength = 40.0f;

    pathForward = Object(this->_shader, "models/tubecube/tubecube.obj");
    pathForward.SetTag("forward");
    pathTurn = Object(this->_shader, "models/tubecube/tubecube_turn.obj");
    pathTurn.SetTag("turn");

    srand(time(0));

    for (int i = 0; i < this->_chunksAmount; i++){

        if (i == 0)
            this->chunks.push_back(Object(pathForward));
        else
            this->chunks.push_back(RandomChunkFromLast());
    }
    
    //parenting
    for (list<Object>::iterator it = this->chunks.begin(); it != this->chunks.end(); it++){
        if (it != this->chunks.begin()){
            list<Object>::iterator it_previous = prev(it);
            it->transform.parent = &it_previous->transform;
        }
        if (it != prev(this->chunks.end())){
            list<Object>::iterator it_next = next(it, +1);
            it->transform.child = &it_next->transform;
        }
    }

    //position relative to parent
    for (list<Object>::iterator it = next(this->chunks.begin(), 1); it != this->chunks.end(); it++){
        SetPositionFromParent((*it));
    }

    this->chunks.begin()->transform.UpdateMatrix();
    this->_pivotPoint = Transform(this->chunks.begin()->transform);
    this->_pivotPoint.Translate(vec3_right / 2.0f);
    this->_pivotPoint.Translate(vec3_back / 2.0f);
}

void PathGenerator::SwapFirstToLast(){

    RenderPipeline::ClearBuffers(this->chunks.begin()->shader, this->chunks.begin()->meshes, false);
    this->chunks.erase(this->chunks.begin());

    this->chunks.push_back(RandomChunkFromLast());

    //parenting
    this->chunks.begin()->transform.parent = nullptr;
    next(this->chunks.end(), -2)->transform.child = & next(this->chunks.end(), -1)->transform; 
    next(this->chunks.end(), -1)->transform.parent = & next(this->chunks.end(), -2)->transform; 
    next(this->chunks.end(), -1)->transform.child = nullptr;

    this->chunks.begin()->transform.position = this->chunks.begin()->transform.LocalToWorldPosition();
    this->chunks.begin()->transform.eulerAngles = this->chunks.begin()->transform.LocalToWorldRotation();

    list<Object>::iterator it = next(this->chunks.end(), -1);

    SetPositionFromParent((*it));
    this->_pivotPoint = Transform(this->chunks.begin()->transform);
    this->_pivotPoint.Rotate(vec3_up, 90.0f);
    this->_pivotPoint.Translate(vec3_right / 2.0f);
    this->_pivotPoint.Translate(vec3_back / 2.0f);

    int i = 0;
    for (list<Object>::iterator it = this->chunks.begin(); it != this->chunks.end(); it++){
        std::cout << "[" << i << "] " << it->transform.GetTag() << std::endl;
        i++;
    }
    std::cout << "==========================" << std::endl << std::endl;
}

void PathGenerator::MovePath(float deltaTime){

    if (this->chunks.begin()->transform.GetTag() == "turn" && this->chunks.begin()->transform.eulerAngles.y < 90.0f){
        // this->chunks.begin()->transform.Interpolate(this->_pivotPoint.GetQuaternion(), this->speed * deltaTime);
        this->chunks.begin()->transform.RotateAround(this->_pivotPoint.position, vec3_up, 0.5f * this->speed * deltaTime);
    }
    else{
        this->chunks.begin()->transform.Translate(vec3_back * this->speed * deltaTime);
    }
}

float PathGenerator::GetChunkLength(){
    return this->_chunkLength;
}

float PathGenerator::GetHalfChunkLength(){
    return this->_chunkLength / 2.0f;
}

void PathGenerator::SetPositionFromParent(Object &chunk){

    // chunk.transform.Translate(chunk.transform.parent->Forward() * this->_chunkLength);
    if (chunk.transform.parent->GetTag() == "turn"){
        // chunk.transform.Rotate(vec3_up, -90.0f);
        // chunk.transform.Translate(vec3_right * this->_chunkLength);
        chunk.transform.Translate(vec3_forward * this->_chunkLength);
        chunk.transform.RotateAround(vec3_zero, vec3_up, -90.0f);
    }
    else {
        chunk.transform.Translate(vec3_forward * this->_chunkLength);    
    }
    
    if (chunk.transform.GetTag() == "turn"){
        chunk.transform.Rotate(vec3_forward, rand() % 360);
    }

}

Object PathGenerator::RandomChunkFromLast(){
    return RandomChunk((*next(this->chunks.end(), -1)));
}

Object PathGenerator::RandomChunk(Object previousChunk){

    Object objectChunk;

    if (previousChunk.GetTag() != "turn" && rand() % 100 < 15)
        objectChunk = Object(pathTurn);
    else
        objectChunk = Object(pathForward);

    return objectChunk;    
}