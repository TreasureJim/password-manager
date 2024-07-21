#pragma once


#include "map.hpp"
#include <fstream>
#include <optional>

std::optional<std::fstream> read_map_file();
std::fstream open_map_file();

std::vector<char> map_serialize(const Map &map);
Map map_deserialize(const std::vector<char> &v);
