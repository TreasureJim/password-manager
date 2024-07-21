#include "map.hpp"
#include <fstream>
#include <iostream>
#include <ostream>
#include <utility>

#include "serialisation.hpp"

int main_menu_input() {
	while(true) {
		std::cout <<	
			"1: Search\n"
			"2: Add Login\n\n"
			"Input: " << std::endl;

		char option;
		std::cin >> option;

		switch (option) {
			case '1':
			case '2':
			return option;

			default:
			std::cout << "Invalid input. Try again." << std::endl;
			continue;
		}
	}
}

int main (int argc, char *argv[]) {
	std::cout << "Password Manager\n\n\n";

	// main_menu_input();

	auto [map_file, exists] = find_map_file();
	Map map;
	if (exists) {
		map = map_deserialize(map_file);
	}

	map_print(map);

	map.clear();
	map.insert(std::make_pair("Google", Login("Heehee", "hooHoo")));
	map.insert(std::make_pair("Google", Login("weewa", "yaya")));
	map.insert(std::make_pair("Bumblebee", Login("weewa", "leeloo")));

	map_serialize(map_file, map);

	return 0;
}
