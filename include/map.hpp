#pragma once

#include "login.hpp"

using Map = std::unordered_multimap<std::string, Login>;

void map_print(const Map& map);
