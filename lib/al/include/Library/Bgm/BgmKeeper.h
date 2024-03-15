#pragma once

namespace al {
    class AudioSystemInfo;
    class BgmDirector;
    class BgmUserInfo;

    class BgmKeeper {
    public:
        BgmKeeper(const al::AudioSystemInfo*, al::BgmDirector*, const char*);
        
        BgmDirector** create(const al::AudioSystemInfo*, al::BgmDirector*, const char*)
        void update();
        const BgmUserInfo** getUserName();
    private:
        const char* mUserName; //this is BgmUserInfo == char
    }

}