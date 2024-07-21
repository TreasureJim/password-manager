#include "map.hpp"
#include <cstring>
#include <iostream>

void map_print(const Map& map) {
	std::string last_key;
	for(const auto& pair: map) {
		if (last_key.compare(pair.first)) {
			std::cout << "\x1B[4m" << pair.first << "\x1B[24m" << "\n";
			last_key = pair.first;
		}
		std::cout << pair.second << std::endl;
	}
}
