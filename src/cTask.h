#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>


class cPlayer
{
    std::vector<int> myTimes;               // available times
    std::vector<std::string> myOpps;        // names of possible opponents

public:
    std::string myName;                     // name

    
    int myMaxGames;                         // maximum games that can be played
    int myCountGames;                       // games scheduled

    cPlayer(const std::string name)
        : myName(name),
        myMaxGames( 0 )
    {
    }

    /** @brief Add an available time
     @param t 

    Player can only be matched at one of these times

    All times are an hour in the same week

    e.g. 210 means 10am on Tuesday
    */

    void addTime(int t);

    /** @brief Add a possible opponent
     @param opp opponent name

    Player can only be matched with opponents of these name
    */

    void addOpp(const std::string &opp);


    bool operator==(std::string &name)
    {
        return myName == name;
    }

    const std::vector<int>& getTimes() const
    {
        return myTimes;
    }
    const std::vector<std::string>& getOpps() const
    {
        return myOpps;
    }
};

class cCourt
{
public:
    int myTime;
    std::string myName;

    cCourt(const std::string &name, int t)
        : myName(name),
          myTime(t)
    {
    }
};
/**
 * @brief A feasible match between two players on an available court at a particular time
 *
 */
class cGame
{
public:
    cPlayer *p1;
    cPlayer *p2;
    int myt;
    cCourt *myCourt;
    std::string myName;

    cGame();
    cGame(
        cPlayer &a,
        cPlayer &b,
        int t,
        cCourt &c);
    void display();
    static void displayAll();
    static bool find(
        const std::string &p1,
        const std::string &p2);
    static std::string nextName();
    static cGame &get(int index);

};

class cMatcher
{
    std::vector<std::pair<int,int>> myPriorityTimes;
    
public:
    std::vector<cGame> myFeasibleGames;
    std::vector<int> myGames;       /// indices of the scheduled games


    void generate1();
    void generate2();

    void timePriority(int t, int p);
    int priority( int t ) const;
    void sortTimePriority();

    void check();
    void maxflow();
    void checkPlayerGames();
    void display();
};

extern std::vector<cPlayer> thePlayers;
extern std::vector<cCourt> theCourts;
extern cMatcher theClub;
