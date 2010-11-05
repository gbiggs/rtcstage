#include <iostream>
#include <rtm/Manager.h>
#include <string>
#include <stdlib.h>

#include "rtcstage.h"

void StageModuleInit(RTC::Manager* manager)
{
    rtcstage_init(manager);
    RTC::RtcBase* comp;
    comp = manager->createComponent("RTC_Stage");

    if (comp == NULL)
    {
        std::cerr << "Stage component creation failed." << std::endl;
        abort();
    }
}


int main(int argc, char** argv)
{
    RTC::Manager *manager;
    manager = RTC::Manager::init(argc, argv);
    manager->init(argc, argv);
    manager->setModuleInitProc(StageModuleInit);
    manager->activateManager();
    manager->runManager();

    return 0;
}

