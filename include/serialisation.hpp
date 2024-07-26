#pragma once

#include "encryption.hpp"
#include "map.hpp"
#include <fstream>
#include <optional>

#define DEFAULT_PATH "./map.bin"

std::optional<std::fstream> read_map_file(std::string path);
std::fstream open_map_file(std::string path);

void serialize_string(std::vector<char> &vec, const std::string &s);
std::string deserialize_string(std::vector<char>::const_iterator &iter, const std::vector<char>::const_iterator &end);

class Serializer {
private:
  Decryptor decryptor;

public:
  bool error = this->decryptor.error;

  bool map_serialize(const Map &map, const std::string& password);
  std::optional<Map> map_deserialize(const std::string &password);
};
