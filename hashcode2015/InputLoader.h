#pragma once

#include "Objects.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>


class InputLoader
{
public:
    void LoadServersFromFile(const string & fileName)
    {
        //vector<std::shared_ptr<Server>> servers;
        ifstream file("dc.in");
        if (file.is_open())
        {
            string line;
            getline(file, line);
            stringstream ss;
            ss << line;
            ss >> param.Rows >> param.Slots >> param.UnavailableSlots >> param.Pools >> param.NbServes;
            ss.clear();
            //cout << param << endl;

            int unavailableSlotNb = param.UnavailableSlots;
            while(unavailableSlotNb)
            {
                getline(file, line);
                UnavailableSlot slot;
                ss << line;
                ss >> slot.rowIndex >> slot.slotIndex;
                //cout << slot << endl;
                unavailableSlots.push_back(slot);
                unavailableSlotNb--;
                ss.clear();
            }

            int serverNb = param.NbServes;
            int serverIndex = 0;
            while(serverNb)
            {
                getline(file, line);
                auto server = std::make_shared<Server>();
                ss << line;
                ss >> server->size >> server->capacity;
                ss.clear();
                //cout << *server << endl;
                server->id = serverIndex;
                serverIndex++;
                servers.push_back(server);
                serverNb--;
            }
        }
        else
        {
            cout << " file is not open" << endl;
        }
    }

public:
    InputParam param;
    vector<shared_ptr<Server>> servers;
    vector<UnavailableSlot> unavailableSlots;

};