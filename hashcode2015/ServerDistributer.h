#pragma once

#include "Objects.h"
#include <memory>
#include <queue>

class ServerDistributer
{
public:
    virtual void Distribute(DataCenter& dataCenter, vector<std::shared_ptr<Server>>& servers) = 0;
};


#include <random>


class RandomServerDistributer : public ServerDistributer
{
public:

    void Reset(DataCenter& dataCenter, vector<std::shared_ptr<Server>>& servers)
    {
        for(int i = 0; i < dataCenter.rows.size(); ++i)
        {
            for(int j = 0; j < dataCenter.rows[i].slots.size(); ++j)
            {
                if (dataCenter.rows[i].slots[j] != UNAVAILABLE_SLOT)
                    dataCenter.rows[i].slots[j] = EMPTY_SLOT;
            }
        }

        for(int i = 0; i < servers.size(); ++i)
        {
            servers[i]->pool = INVALID_VALUE;
            servers[i]->row = INVALID_VALUE;
            servers[i]->slotIndex = INVALID_VALUE;
        }
    }

    virtual void Distribute(DataCenter& dataCenter, vector<std::shared_ptr<Server>>& servers) override
    {
        for (size_t i = 0; i < servers.size(); ++i)
        {
            shared_ptr<Server>& server = servers[i];
            
            //server->pool = poolGenerator();
            if (PutServerIntoRandomRowSlot(dataCenter, server) == false)
            {
                serverQueue.push(server);
                continue;
            }
            //server->pool = GetRandomNumber(POOL_NUMBER)-1;
            server->pool = GetRandomPoolNumber()-1;
            //cout << "server pool: " << server->pool << endl;
        }

        while(!serverQueue.empty())
        {
            if(dataCenter.AreSlotsAllOccupied())
                break;

            auto server = serverQueue.front();
            serverQueue.pop();
            PutServerIntoSlotRowByRow(dataCenter, server);
        }
        //cout << "Distribute finished!" << endl;
    }

    bool PutServerIntoRandomRowSlot(DataCenter& dataCenter, std::shared_ptr<Server>& server)
    {
        //int rowIndex = GetRandomNumber(ROW_NUMBER)-1;  // convert to 0-based index
        int rowIndex = GetRandomRowNumber()-1;  // convert to 0-based index
        //cout << "random row index: " << rowIndex << endl;
        //int rowIndex = GetRandomNumber()-1;  // convert to 0-based index
        DataCenterRow& row = dataCenter.rows[rowIndex];
        int slotIndex = GetAvaiableSlot(row, server->size);
        if (slotIndex == INVALID_SLOT_INDEX)
        {
            return false;
        }
        SetServerToRow(row, slotIndex, server);
        return true;
    }

    bool PutServerIntoSlotRowByRow(DataCenter& dataCenter, std::shared_ptr<Server>& server)
    {
        //int rowIndex = GetRandomNumber(ROW_NUMBER)-1;  // convert to 0-based index
        for(int rowIndex = 0; rowIndex < ROW_NUMBER; ++rowIndex)
        {
            DataCenterRow& row = dataCenter.rows[rowIndex];
            int slotIndex = GetAvaiableSlot(row, server->size);
            if (slotIndex != INVALID_SLOT_INDEX)
            {
                SetServerToRow(row, slotIndex, server);
                return true;
            }
        }
        return false;
    }

    int GetAvaiableSlot(const DataCenterRow& row, int size)
    {
        for(int i = 0; i < SLOT_NUMBER_PER_ROW-size; ++i)
        {
            bool allSlotsOK = true;  // here, all slots means all the slots begins with 'i', with a length 'size'
            for(int j=0; j<size; ++j)
            {
                if (row.slots[i+j] != EMPTY_SLOT)
                {
                    allSlotsOK = false;
                    break;
                }
            }
            if (allSlotsOK == true)
            {
                return i;
            }
        }
        return INVALID_SLOT_INDEX;
    }

    void SetServerToRow(DataCenterRow& row, int slotIndex, std::shared_ptr<Server>& server)
    {
        for(int i=0; i < server->size; ++i)
        {
            row.slots[slotIndex+i] = server->id;
        }
        server->row = row.rowIndex;
        server->slotIndex = slotIndex;
    }

    int GetRandomRowNumber()
    {
        static std::random_device rdev;
        static std::default_random_engine engine(rdev());
        static std::uniform_int_distribution<int> dist(1, ROW_NUMBER);
        return dist(engine);
    }

    int GetRandomPoolNumber()
    {
        static std::random_device rdev2;
        static std::default_random_engine engine2(rdev2());
        static std::uniform_int_distribution<int> dist2(1, POOL_NUMBER);
        return dist2(engine2);
    }



    std::queue<std::shared_ptr<Server>> serverQueue;

};
