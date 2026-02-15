#pragma once

namespace al {
class RankingDirector;

class IUseRanking {
public:
    virtual RankingDirector* getRankingDirector() const = 0;

    virtual bool isAvailableRanking() const { return false; }
};
}  // namespace al
