#pragma once

namespace al {
class Resource;

class ModelCtrl {
public:
    ModelCtrl();
    ~ModelCtrl();

    void initResource(Resource*, Resource*);
    void show();
    void hide();
};
}  // namespace al
