#pragma once

namespace al {
class RankingDirector;

class IUseRanking {
public:
    virtual RankingDirector* getRankingDirector() const;
    virtual bool isAvailableRanking() const;
};
}  // namespace al
