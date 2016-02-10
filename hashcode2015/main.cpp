#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>

#include "InputLoader.h"
#include "ServerDistributer.h"
#include "Output.h"

#include <iostream>

int GetScoreWhenSkipOneRow(const std::vector<shared_ptr<Server>>& servers, int rowIndex)
{
    std::array<int, POOL_NUMBER> poolCapacity;
    //std::fill_n(poolCapacity.begin(), poolCapacity.end(), 0);
    for (int i = 0; i < poolCapacity.size(); ++i)
    {
        poolCapacity[i] = 0;
    }
    for (int i = 0; i < servers.size(); ++i)
    {
        if(servers[i]->row == rowIndex)
            continue;

        if(servers[i]->pool != INVALID_VALUE)
        {
            poolCapacity[servers[i]->pool] += servers[i]->capacity;
        }
    }
    return *std::min_element(poolCapacity.begin(), poolCapacity.end());
}

int GetScore(const std::vector<shared_ptr<Server>>& servers)
{
    int score = std::numeric_limits<int>::max();
    for(int i = 0; i < ROW_NUMBER; ++i)
    {
        score = min(score, GetScoreWhenSkipOneRow(servers, i));
    }
    return score;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "We are the champion!\n";

    InputLoader loader;
    loader.LoadServersFromFile("dc.in");

    vector<UnavailableSlot> unSlots = loader.unavailableSlots;

    // sort by rowsIndex, then by slotIndex
    //std::sort(unSlots.begin(), unSlots.end());

    DataCenter dc;
    dc.SetUnavailableSlots(unSlots);

    //shared_ptr<ServerDistributer> distributer(new RandomServerDistributer);
    shared_ptr<ServerDistributer> distributer(new CapacityPerSizeDistributer);


    distributer->Reset(dc, loader.servers);
    distributer->Distribute(dc, loader.servers);

    int score = GetScore(loader.servers);
    cout <<"score is :" << score << endl;
	OutputWriter::WriteOutput(loader.servers, R"(C:\Users\U0137572\test\hashcode2015\Debug\dc.out)");

    //int bestScore = 0;
    //for(int i = 0; i< 10000; ++i)
    //{
    //    distributer->Reset(dc, loader.servers);
    //    distributer->Distribute(dc, loader.servers);
    //    int score = GetScore(loader.servers);
    //    cout <<"score is :" << score << endl;
    //    bestScore = max(bestScore, score);
    //}

    //cout << "best score is :" << bestScore << endl;


    return 0;
}