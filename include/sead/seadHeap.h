#pragma once

#include "seadDisposer.h"
#include "seadNamable.h"
#include "seadHostIO.h"

namespace sead
{
    class Heap : public IDisposer, INamable, hostio::Reflexible
    {
    public:
        
    };
};