#pragma once

#include <math/seadMatrix.h>

namespace al {
class Resource;

class ModelCtrl {
public:
    ModelCtrl();
    ~ModelCtrl();

    void initialize(nn::g3d::ResModel*, int, int, sead::Head*, al::ShaderHolder);
    
    void show();
    void hide();
    void initResource(al::Resource*, al::Resource*);
    
    sead::Matrix34f* mMatrix;
};

}