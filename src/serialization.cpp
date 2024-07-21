#include "login.hpp"
#include "map.hpp"

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

void serialize_string(std::fstream &f, const std::string &s) {
  uint16_t size = s.length();
  f.write(reinterpret_cast<const char *>(&size), sizeof(size));

  f.write(s.c_str(), size);
}

std::string deserialize_string(std::fstream &f) {
  uint16_t size;
  f.read(reinterpret_cast<char *>(&size), sizeof(size));

  std::string s(size, ' ');
  f.read(s.data(), size);
  return s;
}

void Login::serialize(std::fstream &f) const {
  serialize_string(f, this->username);
  serialize_string(f, this->password);
}

Login Login::deserialize(std::fstream &f) {
  auto username = deserialize_string(f);
  auto password = deserialize_string(f);

  return Login(username, password);
}

void map_serialize(std::fstream &file, const Map &map) {
  uint16_t size = map.size();
  file.write(reinterpret_cast<const char *>(&size), sizeof(size));

  for (auto &pair : map) {
    serialize_string(file, pair.first);
    pair.second.serialize(file);
  }
}

Map map_deserialize(std::fstream &f) {
  Map map;

  uint16_t size;
  if (!f.read(reinterpret_cast<char *>(&size), sizeof(size))) {
    std::cerr << "ERROR: Could not parse map file\n";
    return map;
  }

  for (uint16_t i = 0; i < size; i++) {
    std::string key = deserialize_string(f);
    Login l = Login::deserialize(f);

    map.insert(std::make_pair(key, l));
  }

  return map;
}

std::pair<std::fstream, Map> read_map_file() {
  static const char *path = "./map.bin";
  bool exists = false;

  {
    FILE *f = fopen(path, "r");
    exists = !(f == NULL);
    if (exists)
      fclose(f);
  }

  Map map;
  if (exists) {
    std::fstream f(path, std::fstream::in | std::fstream::binary);
    map = map_deserialize(f);
  }

  return std::make_pair( std::fstream(path, std::fstream::out | std::fstream::binary | std::fstream::trunc), map);
}
