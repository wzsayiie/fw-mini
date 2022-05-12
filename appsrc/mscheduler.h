#pragma once

#include "mobject.h"

m_class(MScheduler, MObject) {
public:
    static MScheduler *sharedObject();

public:
    double GetSecondsTick();
};
