#include "cTask.h"
void cMatcher::clear()
{
    thePlayers.clear();
    theCourts.clear();
    // myPriorityTimes.clear();
    // myFeasibleGames.clear();
    // myGames.clear();
}
bool cMatcher::unitTest()
{
    bool ret = true;
    cMatcher matcher;
    matcher.generate2();
    matcher.check();
    matcher.maxflow();

    if ( matcher.myGames.size() != 1 ) {
        std::cout << "unit test 1 failed\n";
        ret = false;
    }

    clear();

    return ret;
}