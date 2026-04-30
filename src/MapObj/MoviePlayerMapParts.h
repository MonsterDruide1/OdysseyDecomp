#pragma once

#include <prim/seadSafeString.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class TextureReplacer;
}  // namespace al

class MoviePlayer;

class MoviePlayerMapParts : public al::LiveActor {
public:
    MoviePlayerMapParts(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void kill() override;
    void startClipped() override;
    void endClipped() override;
    void control() override;

    bool tryPlayMovie();

private:
    MoviePlayer* mMoviePlayer = nullptr;
    al::TextureReplacer* mMovieTextureReplacer = nullptr;
    al::TextureReplacer* mWhiteTextureReplacer = nullptr;
    sead::FixedSafeString<128> mMoviePath;
    sead::FixedSafeString<128> mSwitchMoviePath;
    const char* mCurrentMoviePath = nullptr;
    bool mIsNeedTextureUpdate = false;
};

static_assert(sizeof(MoviePlayerMapParts) == 0x260);
