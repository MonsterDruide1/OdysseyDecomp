#pragma once

namespace al
{
    class LiveActor;

    class ExecuteRequestInfo
    {
    public:
        ExecuteRequestInfo();

        unsigned long _0;
    };

    class ExecuteRequestTable
    {
    public:
        ExecuteRequestTable(int);

        int mRequestCount; // _0
        int _4;
        unsigned long* _8;
    };

    class ExecuteRequestKeeper
    {
    public:
        ExecuteRequestKeeper(int);

        enum Request : int
        {
            Request_Movement = 0x0,
            Request_RemoveFromMovement = 0x1,
            Request_Draw = 0x2,
            Request_RemoveFromDraw = 0x3
        }

        void executeRequestActorMovementAllOn();
        void executeRequestActorMovementAllOff();
        void executeRequestActorDrawAllOn();
        void executeRequestActorDrawAllOff();
        void request(al::LiveActor *, ExecuteRequestKeeper::Request);
    };
}
};