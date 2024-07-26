#include "serialisation.hpp"
#include "encryption.hpp"
#include "map.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <array>

std::optional<std::fstream> read_map_file(std::string path = DEFAULT_PATH) {
  std::fstream f(path,
                 std::fstream::in | std::fstream::out | std::fstream::binary);
  if (!f.good()) {
    return std::nullopt;
  }

  return f;
}

std::fstream open_map_file(std::string path = DEFAULT_PATH) {
  return std::fstream(path, std::fstream::out | std::fstream::binary |
                                std::fstream::trunc);
}

bool read_from_iterator(std::vector<char>::const_iterator &it,
                        std::vector<char>::const_iterator end, char *output,
                        uint16_t length) {
  if (std::distance(it, end) < length) {
    std::cerr << "ERROR: Could not parse data from vector\n";
    return false;
  }

  // Copy data from iterator to the output variable
  std::memcpy(&output, &(*it), length);
  std::advance(it, length);
  return true;
}

template <typename T>
bool read_from_iterator(std::vector<char>::const_iterator &it,
                        const std::vector<char>::const_iterator &end,
                        T &output) {
  if (std::distance(it, end) < sizeof(T)) {
    std::cerr << "ERROR: Could not parse data from vector\n";
    return false;
  }

  // Copy data from iterator to the output variable
  std::memcpy(&output, &(*it), sizeof(T));
  std::advance(it, sizeof(T));
  return true;
}

void serialize_string(std::vector<char> &vec, const std::string &s) {
  uint16_t size = s.length();
  vec.insert(vec.end(), reinterpret_cast<char *>(&size),
             reinterpret_cast<char *>(&size) + sizeof(size));

  vec.insert(vec.end(), s.data(), s.end().base());
}

std::string deserialize_string(std::vector<char>::const_iterator &iter,
                               const std::vector<char>::const_iterator &end) {
  uint16_t size;
  read_from_iterator(iter, end, size);

  std::string s(size, ' ');
  read_from_iterator(iter, end, s.data(), size);
  return s;
}

/// ------------------ ///

bool Serializer::map_serialize(const Map &map, const std::string &password) {
  std::vector<char> vec;

  uint16_t size = map.size();
  vec.insert(vec.end(), reinterpret_cast<char *>(&size),
             reinterpret_cast<char *>(&size) + sizeof(size));

  for (auto &pair : map) {
    serialize_string(vec, pair.first);
    pair.second.serialize(vec);
  }

  // encryption and saving
  auto encrypt_result = decryptor.encrypt(vec, password);
  if (!encrypt_result.has_value())
    return false;
  auto [ciphertext, nonce] = encrypt_result.value();

  auto file = open_map_file();
  file.write((const char *)nonce.data(), NONCE_LEN);
  file.write((const char *)ciphertext.data(), ciphertext.size());

  return true;
}

std::optional<Map> Serializer::map_deserialize(const std::string &password) {
  Map map;

  auto file = read_map_file();
  if (!file.has_value())
  {
    std::cout << "No map.bin file found. Generating new save." << std::endl;
    return map;
  }

  std::array<char, NONCE_LEN> nonce;
  if (!file->read(nonce.data(), nonce.size())) {
    std::cerr << "ERROR: couldn't read nonce from file." << std::endl;
    return std::nullopt;
  }

  file->seekg(0, std::ios::end);
  int ciphertext_size = (int)file->tellg() - NONCE_LEN;
  file->seekg(NONCE_LEN, std::ios::beg);

  std::vector<char> ciphertext;
  ciphertext.resize(ciphertext_size);
  file->read(ciphertext.data(), ciphertext.size());

  auto decrypted_result =
      decryptor.decrypt(ciphertext, (unsigned char *)nonce.data(), password);
  if (!decrypted_result.has_value())
    return std::nullopt;
  std::vector<char> v = decrypted_result.value();

  std::vector<char>::const_iterator iter = v.begin();

  uint16_t size;
  if (!read_from_iterator(iter, v.end(), size)) {
    std::cerr << "ERROR: Could not parse map file\n";
    return std::nullopt;
  }

  for (uint16_t i = 0; i < size; i++) {
    std::string key = deserialize_string(iter, v.end());
    Login l = Login::deserialize(iter, v.end());

    map.insert(std::make_pair(key, l));
  }

  return map;
}
