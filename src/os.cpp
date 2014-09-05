#include <iostream>
#include <os.h> 

using namespace std;

OS::OS(Config *cfg) :
    osConfig(cfg->getOsConfig()),
    maxTick(osConfig->getMaxTick()),
    unitTick(osConfig->getUnitTick()),
    contextSwitchTick(osConfig->getContextSwitchTick()),
    nApps(osConfig->getNApps()),
    untilContextSwitch(0),
    curTick(0),
    nAppsFinished(0)
{
    env = new Environment(cfg->getEnvConfig());
    readyQueue = new AppQueue(cfg->getAppQueueConfig());
    //factory = new AppFactory();

    configFileRoot = cfg->getConfigFileRoot();
    osTypeString = cfg->getOsTypeString();

    syscallSpecs = configFileRoot+osTypeString+"/syscalls.json";
    appSpecs = configFileRoot+osTypeString+"/apps.json";

}

void OS::run() {
    init();
    while(curTick <= maxTick) {
        cout << "<<Tick: " << curTick << ">>" << endl;
        checkAndDoSchedule();
        for(int i=0; i<env->getNCores(); i++) {
            cout << "execute core " << i << "..." << endl;
            executeCore(i, unitTick);
        }
        curTick += unitTick;
        afterExecute();
    }
    wrapUp();
}

void OS::init() {
    for(unsigned int i=0; i<nApps; i++) {
        readyQueue->enque(factory->createApp());
    }
    for(unsigned int i=0; i<env->getNCores(); i++) {
        env->getCore(i)->loadApp(readyQueue->deque());
    }
}

void OS::executeCore(unsigned int n, unsigned int unitTick) {
    Core *core = env->getCore(n);
    core->run(unitTick);
}


void OS::wrapUp() {
    cout << "total apps processed: " << nAppsFinished << endl;
    return;
}
