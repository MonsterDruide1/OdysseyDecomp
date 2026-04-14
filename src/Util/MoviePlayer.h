#pragma once

#include <basis/seadTypes.h>
#include <heap/seadDisposer.h>

#include "Library/Scene/ISceneObj.h"

namespace agl {
class DrawContext;
class TextureData;
}  // namespace agl

class MoviePlayer : public al::ISceneObj, public sead::IDisposer {
public:
    static const s32 sSceneObjId = 36;

    MoviePlayer();
    ~MoviePlayer() override;

    void update();
    void draw(agl::DrawContext* drawContext) const;
    const agl::TextureData* getTexture() const;
    void play(const char* moviePath);
    void stop();
    bool isPlay() const;
    bool isDecode() const;
    bool isLooped() const;

private:
    void* mDecoderPlayer = nullptr;
    void* mTextureData = nullptr;
    void* mGpuMemBlock = nullptr;
    bool mIsPlaying = false;
    u8 _41[3];
    s32 mDecodeSlot = 0;
};

static_assert(sizeof(MoviePlayer) == 0x48);
