#pragma once

#include <basis/seadTypes.h>

namespace al {
class CameraPoserAnim;
class CameraPoserCart;
class CameraPoserEntrance;
class CameraPoserFixActor;
class CameraPoserFixPoint;
class CameraPoserParallelSimple;
class CameraPoserQuickTurn;
class CameraPoserRace;
class CameraPoserSubjective;
class CameraPoserTower;
class CameraPoser;
class CameraTicketId;

class CameraTicket {
public:
    enum Priority {
        Priority_Default = 0,
        Priority_Entrance = 2,
        Priority_BossField = 3,
        Priority_Capture = 4,
        Priority_Object = 5,
        Priority_ForceArea = 6,
        Priority_Unknown = 7,  // Mention in alCameraPoserFunction::isPrePriorityEntranceAll
        Priority_SafetyPointRecovery = 8,
        Priority_Player = 9,
        Priority_DemoTalk = 10,
        Priority_Demo = 11,
        Priority_Demo2 = 12,
    };

    CameraTicket(CameraPoser* poser, const CameraTicketId* ticketId, s32 priority);
    void setPriority(s32 priority);

    CameraPoser* getPoser() const { return mPoser; }

    CameraPoserAnim* getAnimPoser() const { return mAnimPoser; }

    CameraPoserCart* getCartPoser() const { return mCartPoser; }

    CameraPoserEntrance* getEntrancePoser() const { return mEntrancePoser; }

    CameraPoserFixActor* getFixActorPoser() const { return mFixActorPoser; }

    CameraPoserFixPoint* getFixPointPoser() const { return mFixPointPoser; }

    CameraPoserParallelSimple* getParallelSimplePoser() const { return mParallelSimplePoser; }

    CameraPoserQuickTurn* getQuickTurnPoser() const { return mQuickTurnPoser; }

    CameraPoserRace* getRacePoser() const { return mRacePoser; }

    CameraPoserSubjective* getSubjectivePoser() const { return mSubjectivePoser; }

    CameraPoserTower* getTowerPoser() const { return mTowerPoser; }

    const CameraTicketId* getTicketId() const { return mTicketId; }

    s32 getPriority() const { return mPriority; }

    bool isActiveCamera() const { return mIsActiveCamera; }

    void setActiveCamera(bool isActiveCamera) { mIsActiveCamera = isActiveCamera; }

private:
    union {
        CameraPoser* mPoser;
        CameraPoserAnim* mAnimPoser;
        CameraPoserCart* mCartPoser;
        CameraPoserEntrance* mEntrancePoser;
        CameraPoserFixActor* mFixActorPoser;
        CameraPoserFixPoint* mFixPointPoser;
        CameraPoserParallelSimple* mParallelSimplePoser;
        CameraPoserQuickTurn* mQuickTurnPoser;
        CameraPoserRace* mRacePoser;
        CameraPoserSubjective* mSubjectivePoser;
        CameraPoserTower* mTowerPoser;
    };

    const CameraTicketId* mTicketId;
    s32 mPriority;
    bool mIsActiveCamera = false;
};

static_assert(sizeof(CameraTicket) == 0x18);

}  // namespace al
