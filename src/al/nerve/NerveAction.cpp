#include "al/nerve/NerveAction.h"

namespace al
{
    NerveAction::NerveAction()
    {
        _8 = nullptr;

        alNerveFunction::NerveActionCollector* collector = alNerveFunction::NerveActionCollector::sCurrentCollector;

        if (collector->_8)
        {
            collector->_10->_8 = this;
        }
        else
        {
            collector->_8 = this;
        }
        
        collector->_10 = this;
        collector->mActionCount++;
    }
};

namespace alNerveFunction
{
    void NerveActionCollector::addNerve(al::NerveAction *pAction)
    {
        if (_8)
        {
            _10->_8 = pAction;
        }
        else
        {
            _8 = pAction;
        }
        
        _10 = pAction;
        mActionCount++;
    }

    NerveActionCollector::NerveActionCollector()
    {
        mActionCount = 0;
        _8 = nullptr, _10 = nullptr;

        alNerveFunction::NerveActionCollector::sCurrentCollector = this;
    }
};