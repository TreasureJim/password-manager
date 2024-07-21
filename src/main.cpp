#include "map.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <utility>

#include "serialisation.hpp"

Map map;

void add_login_menu() {
  std::string name;
  std::string username;
  std::string password;

  std::cout << "Add Login\n"
               "Name: "
            << std::flush;
  std::cin >> name;

  std::cout << "Username: " << std::flush;
  std::cin >> username;
  std::cout << "Password: " << std::flush;
  std::cin >> password;

  map.insert(std::make_pair(name, Login(username, password)));
  std::cout << "Saved!\n" << std::endl;
}

void main_menu_input() {
  while (true) {
    std::cout << "1: Search\n"
                 "2: Show all logins\n"
                 "3: Add Login\n"
                 "4: Save & Exit\n"
                 "\nInput: "
              << std::flush;

    char option;
    std::cin >> option;

    switch (option) {
    case '1':
      break;

    case '2':
      map_print(map);
      break;

    case '3':
      add_login_menu();
      break;

    case '4':
      return;

    default:
      std::cout << "Invalid input. Try again." << std::endl;
      continue;
    }
  }
}

int main(int argc, char *argv[]) {
  std::cout << "Password Manager\n\n\n";

  auto [map_file, output_map] = read_map_file();
	map = output_map;

  main_menu_input();

  map_serialize(map_file, map);

  return 0;
}
