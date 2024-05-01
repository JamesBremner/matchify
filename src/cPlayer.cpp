#include "cTask.h"

void cPlayer::clear()
{
    myTimes.clear();
}

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

const std::vector<int> &cPlayer::getTimes() const
{
    if (myTimes.size() > 0)
        return myTimes;
    static std::vector<int> alltimes(1, 0);
    return alltimes;
}