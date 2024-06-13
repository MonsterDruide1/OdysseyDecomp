#pragma once

namespace al {
class Resource;

class BgmDataBase {
public:
    static BgmDataBase* create(const char*, const char*);

    BgmDataBase(const Resource*, const Resource*);
};
}  // namespace al
