#pragma once

namespace al {
class Resource;

class BgmDataBase {
public:
    BgmDataBase(const Resource*, const Resource*);
    static BgmDataBase* create(const char*, const char*);
};
}
