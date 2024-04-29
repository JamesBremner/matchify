#include "cTask.h"

void cPlayer::addTime(int t)
{
    if (std::find(myTimes.begin(), myTimes.end(), t) == myTimes.end())
        myTimes.push_back(t);
}
void cPlayer::addOpp(const std::string &opp)
{
    if (std::find(myOpps.begin(), myOpps.end(), opp) == myOpps.end())
        myOpps.push_back(opp);
}