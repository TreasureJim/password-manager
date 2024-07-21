#include "map.hpp"
#include <iostream>

void map_print(const Map& map) {
	for(const auto& pair: map) {
		std::cout << pair.first << "\n" << pair.second << std::endl;
	}
}
