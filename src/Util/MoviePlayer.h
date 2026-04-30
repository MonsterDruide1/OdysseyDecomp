#pragma once

#include <basis/seadTypes.h>
#include <heap/seadDisposer.h>

#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace agl {
class DrawContext;
class TextureData;

template <typename T>
class GPUMemBlock;

}  // namespace agl

class MovieDecoderPlayer;

class MoviePlayer : public al::ISceneObj, public sead::IDisposer {
public:
    static constexpr s32 sSceneObjId = SceneObjID_MoviePlayer;

    MoviePlayer();

    void update();
    void draw(agl::DrawContext* drawContext) const;
    const agl::TextureData* getTexture() const;
    void play(const char* moviePath);
    void stop();
    bool isPlay() const;
    bool isDecode() const;
    bool isLooped() const;

private:
    MovieDecoderPlayer* mDecoderPlayer = nullptr;
    agl::TextureData* mTextureData = nullptr;
    agl::GPUMemBlock<u8>* mGpuMemBlock = nullptr;
    bool mIsDecode = false;
    s32 mFrame = 0;
};

static_assert(sizeof(MoviePlayer) == 0x48);
