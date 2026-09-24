#pragma once

namespace al {
class DataStoreDirector;

class IUseDataStore {
public:
    virtual DataStoreDirector* getDataStoreDirector() const = 0;
};
}  // namespace al
