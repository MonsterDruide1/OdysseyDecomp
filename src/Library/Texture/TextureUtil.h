#pragma once

#include <basis/seadTypes.h>

namespace agl {
class TextureData;
}  // namespace agl

namespace al {
class LiveActor;

class TextureReplacer {
public:
    TextureReplacer();
    explicit TextureReplacer(const agl::TextureData* textureData);

    void setup(const agl::TextureData* textureData);
    void replace(LiveActor* actor, const char* sourceTextureName, const char* replaceTextureName);
    void update();

private:
    u8 _0[0x20];
};

const agl::TextureData* getWhite2DTexture();
}  // namespace al

static_assert(sizeof(al::TextureReplacer) == 0x20);
