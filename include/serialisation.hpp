#pragma once


#include "map.hpp"
#include <fstream>
#include <utility>

std::pair<std::fstream, bool> find_map_file();
void map_serialize(std::fstream& file, const Map& map);
Map map_deserialize(std::fstream& f);
