#pragma once
#include <ostream>
#include <string>
#include <fstream>

#include <memory>

#include "Objects.h"

class OutputWriter
{
public:
	static void WriteOutput(const std::vector<std::shared_ptr<Server>>& servers, const std::string & fileName)
	{
		std::ofstream file;
		file.open(fileName);

		for (const auto & server: servers)
		{
			if (server->pool == INVALID_VALUE)
			{
				file << "x" << std::endl;
			}
			else
			{
				file << server->row << " " << server->slotIndex << " " << server->pool << std::endl;
			}
		}

		file.close();
	}

};

