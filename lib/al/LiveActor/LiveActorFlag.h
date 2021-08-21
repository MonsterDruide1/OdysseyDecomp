#pragma once

namespace al {

    #pragma pack(push, 1)
    class LiveActorFlag {
    public:
        LiveActorFlag();

        long data1;
        int data2;
    };
    static_assert(sizeof(LiveActorFlag) == 0xC);

}
