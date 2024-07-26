#pragma once


#include "map.hpp"
#include <fstream>
#include <optional>

std::optional<std::fstream> read_map_file();
std::fstream open_map_file();

void serialize_string(std::vector<char> &vec, const std::string &s);
std::string deserialize_string(std::vector<char>::const_iterator &iter, const std::vector<char>::const_iterator &end);

std::vector<char> map_serialize(const Map &map);
Map map_deserialize(const std::vector<char> &v);
