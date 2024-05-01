#include <GraphTheory.h>

#include "cTask.h"

std::vector<cPlayer> thePlayers;
std::vector<cCourt> theCourts;

cMatcher theClub;



     cGame::cGame()
    : myt( -1 )
    {
    }
     cGame::cGame(
        cPlayer &a,
        cPlayer &b,
        int t,
        cCourt &c)
        : p1(&a),
          p2(&b),
          myt(t),
          myCourt(&c)
    {
        myName = nextName();
    }

void cGame::displayAll()
{
    for (auto &t : theClub.myFeasibleGames)
    {
        t.display();
    }
}

bool cGame::find(const std::string &p1n, const std::string &p2n)
{
    for (auto &t : theClub.myFeasibleGames)
        if (t.p1->myName == p1n && t.p2->myName == p2n)
            return true;
    return false;
}
std::string cGame::nextName()
{
    return "task" + std::to_string(theClub.myFeasibleGames.size());
}
void cGame::display()
{
    std::cout << p1->myName
              << " v " << p2->myName
              << " at " << cMatcher::decodeTime(myt)
              << " on " << myCourt->myName
              << "\n";
}


void cMatcher::timePriority( int t, int p )
{
    myPriorityTimes.emplace_back( std::make_pair(t,p));
}
int cMatcher::priority(int t) const
{
    for ( auto &tp : myPriorityTimes )
    {
        if (tp.first == t)
            return tp.second;
    }
    return 0;
}
void cMatcher::sortTimePriority()
{
    std::sort(
        myFeasibleGames.begin(), myFeasibleGames.end(),
        [this](const cGame &a, cGame &b)
        {
            return this->priority(a.myt) > this->priority(b.myt);
        });
}



void cMatcher::maxflow()
{
    myGames.clear();

    sortTimePriority();

    // setup the flow graph
    raven::graph::sGraphData gd;
    gd.g.directed();
    for (auto &c : theCourts)
        for (auto &t : myFeasibleGames)
            if (t.myt == c.myTime)
            {
                // Game can be played on this court
                gd.g.add(c.myName, t.myName);
            }

    // apply the maxflow algorithm, allocating games to courts
    auto ga = raven::graph::alloc(gd);

    for (int ei = 0; ei < ga.edgeCount(); ei++)
        myGames.push_back(atoi(gd.g.userName(gd.g.dest(ei))
                                   .substr(4)
                                   .c_str()));
}

void cMatcher::checkPlayerGames()
{
    for (auto &p : thePlayers)
        p.myCountGames = 0;
    for (int gi : myGames)
    {
        auto &g = myFeasibleGames[gi];
        if (g.p1->myCountGames == g.p1->myMaxGames ||
            g.p2->myCountGames == g.p2->myMaxGames)
            // one or both players are at their maximum games per week
            // unschedule this game
            g.myt = -1;
        else
        {
            g.p1->myCountGames++;
            g.p2->myCountGames++;
        }
    }
}

void cMatcher::display()
{
    std::cout << "\nGame Schedule\n";
    for (int gi : myGames)
        if( myFeasibleGames[gi].myt>0 )
            myFeasibleGames[gi].display();
}