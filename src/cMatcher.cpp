#include "cTask.h"

int findPlayer(const std::string &name)
{
    for (int ip = 0; ip < thePlayers.size(); ip++)
        if (thePlayers[ip].myName == name)
            return ip;
    return -1;
}

void cMatcher::clear()
{
    thePlayers.clear();
    theCourts.clear();
    myFeasibleGames.clear();
}
void cMatcher::generate1()
{
    // generate players
    thePlayers.emplace_back("pA");
    thePlayers.emplace_back("pB");

    // add possible opponents
    thePlayers[0].addOpp("pB");
    thePlayers[1].addOpp("pA");

    // add available times
    thePlayers[0].addTime(1);
    thePlayers[0].addTime(3);
    thePlayers[0].addTime(2);
    thePlayers[1].addTime(2);
    thePlayers[1].addTime(3);

    // generate court times
    theCourts.emplace_back("cA", 1);
    theCourts.emplace_back("cA", 2);
    theCourts.emplace_back("cA", 3);
}
void cMatcher::generate2()
{
    // generate players
    thePlayers.emplace_back("pA");
    thePlayers.emplace_back("pB");

    // add possible opponents
    thePlayers[0].addOpp("pB");
    thePlayers[1].addOpp("pA");

    // add available times
    thePlayers[0].addTime(109);
    thePlayers[0].addTime(110);
    thePlayers[1].addTime(109);
    thePlayers[1].addTime(110);

    // generate court times
    theCourts.emplace_back("cA", 109);
    theCourts.emplace_back("cA", 110);

    // set time priority
    theClub.timePriority(2, 1);
}

bool cMatcher::unitTest()
{
    bool ret = true;
    cMatcher matcher;
    matcher.generate2();
    matcher.check();
    matcher.maxflow();

    if (matcher.myGames.size() != 1)
        ret = false;
    if (matcher.myGames[0] != 0)
        ret = false;

    if (!ret)
        std::cout << "unit test 1 failed\n";

    // test player always available
    matcher.clear();
    matcher.generate2();
    thePlayers[0].clear();
    thePlayers[1].clear();
    thePlayers[1].addTime(110);

    matcher.check();
    matcher.maxflow();
    if (matcher.myGames.size() != 1)
        ret = false;
    else if (matcher.myGames[0] != 0)
        ret = false;
    else if( matcher.myFeasibleGames[0].myt != 110 )
        ret = false;

    if (!ret)
        std::cout << "unit test 2 failed\n";

    matcher.clear();

    return ret;
}

std::string cMatcher::decodeTime(int t)
{
    static std::vector<std::string> weekdays{"???", "Mon", "Tue", "Wed", "Thu", "Fri"};
    std::string ret;
    int day = t / 100;
    ret = weekdays[day] + " ";
    int hour24 = t - 100 * day;
    if (hour24 <= 12)
        ret += std::to_string(hour24) + "am";
    else
        ret += std::to_string(hour24 - 12) + "pm";
    return ret;
}


void cMatcher::check()
{
    // loop over players
    int pi = -1;
    for (auto &p : thePlayers)
    {
        pi++;

        // assume that player has no possible opponent with matching time
        bool ok = false;

        // loop over player available times
        for (int t : p.getTimes())
        {
            // loop over player possible opponents
            for (auto &os : p.getOpps())
            {
                // loop over opponents available times
                int oi = findPlayer(os);
                for (int ot : thePlayers[oi].getTimes())
                {
                    if (isTimeMatch(t,ot))
                    {
                        if( !t )
                            t = ot;

                        // loop over court times
                        for (auto &c : theCourts)
                        {
                            if( isTimeMatch(t,c.myTime))
                            {
                                // found a match with opponent and court

                                ok = true;

                                // create a task for each feasible pair
                                // only record task when player 1 index less than player 2
                                if (pi < oi)
                                {
                                    myFeasibleGames.emplace_back(p, thePlayers[oi], t, c);
                                }
                            }
                        }
                    }
                }
            }
        }
        if (!ok)
        {
            std::cout << "Cannot find opponent for " << p.myName << "\n";
            exit(1);
        }
    }
    std::cout << "found feasible opponents for all players\n";
    cGame::displayAll();
}

bool cMatcher::isTimeMatch( int t1, int t2 )
{
    if( ! t1 )
        return true;
    if( ! t2 )
        return true;
    return t1 == t2;
}
