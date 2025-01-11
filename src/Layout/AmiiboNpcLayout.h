#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}  // namespace al
class FooterParts;

class AmiiboNpcLayout : public al::LayoutActor {
public:
    AmiiboNpcLayout(const al::LayoutInitInfo& info);

    void startTouch();
    void endTouch();
    void appear();
    void control();
    void decide();
    void end();
    bool isIconEndActionEnd() const;

    void exeAppear();
    void exeWait();
    void exeDecide();
    void exeEnd();

private:
    FooterParts* mFooterParts = nullptr;
    al::LayoutActor* mAmiiboIcon = nullptr;
};
