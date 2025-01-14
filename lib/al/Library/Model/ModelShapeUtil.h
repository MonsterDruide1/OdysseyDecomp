#pragma once

namespace al {
class ModelKeeper;

bool isGreaterEqualMaxLodLevelNoClamp(const ModelKeeper* modelKeeper);
bool isLessMaxLodLevelNoClamp(const ModelKeeper* modelKeeper);
}  // namespace al
