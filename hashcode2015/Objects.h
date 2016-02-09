#pragma once
#include <vector>
#include <array>
#include <limits>
using namespace std;

const int EMPTY_SLOT = -1;
const int UNAVAILABLE_SLOT = -2;
const int ROW_NUMBER = 16;
const int SLOT_NUMBER_PER_ROW = 100;
const int POOL_NUMBER = 45;

const int INVALID_SLOT_INDEX = -1;

const int INVALID_VALUE = -1;



struct Server
{
    Server() : id(INVALID_VALUE), 
        size(INVALID_VALUE),
        capacity(INVALID_VALUE),
        pool(INVALID_VALUE),
        row(INVALID_VALUE),
        slotIndex(INVALID_VALUE)
    {

    }

    int id;  // index from the input file
    int size;
    int capacity;
    int pool;   //starts from 0
    int row;
    int slotIndex;
};

ostream& operator<< (ostream& stream, const Server& server)
{
    stream << "size: " << server.size << ", "
        << "capacity: " << server.capacity;
    return stream;
}

struct UnavailableSlot
{
    int rowIndex;
    int slotIndex;

    //bool operator < (const UnavailableSlot& otherSlot)
    //{
    //    if (rowIndex < otherSlot.rowIndex)
    //        return true;
    //    if (rowIndex == otherSlot.rowIndex && slotIndex < otherSlot.slotIndex)
    //        return true;
    //    else
    //        return false;
    //}
};

bool operator < (const UnavailableSlot& slot1, const UnavailableSlot& slot2)
{
    if (slot1.rowIndex < slot2.rowIndex)
        return true;
    if (slot1.rowIndex == slot2.rowIndex && slot1.slotIndex < slot2.slotIndex)
        return true;
    else
        return false;
}

ostream& operator<< (ostream& stream, const UnavailableSlot& unavailableSlot)
{
    stream << "rowIndex: " << unavailableSlot.rowIndex << ", "
        << "slotIndex: " << unavailableSlot.slotIndex;
    return stream;
}

struct InputParam
{
    int Rows;
    int Slots;
    int UnavailableSlots;
    int Pools;
    int NbServes;
};

ostream& operator<< (ostream& stream, const InputParam& param)
{
    stream << "Rows: " << param.Rows << ", "
        << "Slots: " << param.Slots << ", "
        << "Unavaiable slots: " << param.UnavailableSlots << ", "
        << "Pools: " << param.Pools << ", "
        << "NbServers: " << param.NbServes;
    return stream;
}

struct DataCenterRow
{
    DataCenterRow()
    {
        std::fill_n(slots.begin(), slots.size(), EMPTY_SLOT);
    }

    //vector<int> slots;   // index of the server
    array<int, SLOT_NUMBER_PER_ROW> slots;
    int rowIndex;
};

struct DataCenter
{
    DataCenter()
    {
        for(size_t i=0; i< rows.size(); ++i)
        {
            rows[i].rowIndex = i;
        }
    }

    void SetUnavailableSlots(const std::vector<UnavailableSlot> & unavailableSlots)
    {
        for(size_t i = 0; i < unavailableSlots.size(); ++i)
        {
            const UnavailableSlot& slot = unavailableSlots[i];
            rows[slot.rowIndex].slots[slot.slotIndex] = UNAVAILABLE_SLOT;
        }
    }

    bool AreSlotsAllOccupied()
    {
        for(int i = 0; i < rows.size(); ++i)
        {
            for(int j = 0; j < rows[i].slots.size(); ++j)
            {
                if (rows[i].slots[j] == EMPTY_SLOT)
                    return false;
            }
        }
        return true;
    }

    

    //vector<DataCenterRow> rows;
    std::array<DataCenterRow, ROW_NUMBER> rows;
    
};
