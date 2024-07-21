#include "globals.hpp"
#include "map.hpp"
#include <cstdio>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <utility>

void update_entry(Map::iterator iter, int length) {
  int entry_selected;
  while (true) {
    printf("Select entry (1-%d): ", length);
    std::cin >> entry_selected;
    if (std::cin.fail()) {
      std::cout << "Not a valid entry. Try again!" << std::endl;
      continue;
    } else if (entry_selected <= 0 || entry_selected > length) {
      std::cout << "Not in valid entry range. Try again!" << std::endl;
      continue;
    } else
      break;
  }

  std::advance(iter, entry_selected - 1);
  auto &entry = iter->second;

  std::cout << "Current username: " << entry.username << "\n"
            << "Current password: " << entry.password << std::endl;

  std::cout << "Enter username (leave blank for unchanged): " << std::flush;
  std::cin >> entry.username;
  std::cout << "Enter password (leave blank for unchanged): " << std::flush;
  std::cin >> entry.password;

  std::cout << "Entry updated!\n" << std::endl;
}

void remove_entry(Map::iterator iter, int length) {
  int entry_selected;
  while (true) {
    printf("Select entry (1-%d): ", length);
    std::cin >> entry_selected;
    if (std::cin.fail()) {
      std::cout << "Not a valid entry. Try again!" << std::endl;
      continue;
    } else if (entry_selected <= 0 || entry_selected > length) {
      std::cout << "Not in valid entry range. Try again!" << std::endl;
      continue;
    } else
      break;
  }

  std::advance(iter, entry_selected - 1);
  g_map.erase(iter);

  std::cout << "Entry removed!\n" << std::endl;
}

void search_menu() {
  std::cout << "Search (exact match): " << std::flush;
  std::string search;
  std::cin >> search;

  while (true) {
    auto results = g_map.equal_range(search);

    if (results.first == results.second) {
      std::cout << "No results found for \"" << search << "\"\n" << std::endl;
      return;
    }

    std::cout << "Search results for \"" << search << "\"\n" << std::endl;

    int i = 0;
    for (auto entry = results.first; entry != results.second; entry++) {
      i++;
      std::cout << "Entry " << i << "\n" << entry->second << std::endl;
    }

    std::cout << "\n\n"
                 "1. Update entry\n"
                 "2. Remove entry\n"
                 "q: Back\n"
                 "\nInput: "
              << std::flush;
    char option;
    std::cin >> option;

    switch (option) {
    case '1': {
      auto entry = g_map.find(search);
      update_entry(entry, i);
    } break;

    case '2': {
      auto entry = g_map.find(search);
      remove_entry(entry, i);
    } break;

    case 'q':
      return;
    }
  }
}

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

  g_map.insert(std::make_pair(name, Login(username, password)));
  std::cout << "Saved!\n" << std::endl;
}

void main_menu() {
  while (true) {
    std::cout << "1: Search\n"
                 "2: Show all logins\n"
                 "3: Add Login\n"
                 "q: Save & Exit\n"
                 "\nInput: "
              << std::flush;

    char option;
    std::cin >> option;

    switch (option) {
    case '1':
      search_menu();
      break;

    case '2':
      std::cout << "\n" << std::endl;
      map_print(g_map);
      break;

    case '3':
      add_login_menu();
      break;

    case 'q':
      return;

    default:
      std::cout << "Invalid input. Try again." << std::endl;
      continue;
    }
  }
}

std::string login_menu() {
  std::cout << "Input password: " << std::flush;
  std::string pass;
  std::cin >> pass;
  return pass;
}
