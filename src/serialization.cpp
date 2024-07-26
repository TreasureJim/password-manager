#include "encryption.hpp"
#include "login.hpp"
#include "map.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <array>

static const char *path = "./map.bin";
std::optional<std::fstream> read_map_file() {
  std::fstream f(path,
                 std::fstream::in | std::fstream::out | std::fstream::binary);
  if (!f.good()) {
    return std::nullopt;
  }

  return f;
}

std::fstream open_map_file() {
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

class Serializer {
private:
  Decryptor decryptor;

public:
  bool error = this->decryptor.error;

  bool map_serialize(const Map &map, const std::string& password) {
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
    if (!encrypt_result.has_value()) return false;
    auto [ciphertext, nonce] = encrypt_result.value();

    auto file = open_map_file();
    file.write((const char*)nonce.data(), NONCE_LEN);
    file.write((const char*)ciphertext.data(), ciphertext.size());

    return true;
  }

  std::optional<Map> map_deserialize(const std::string &password) {
    std::vector<char> ciphertext;
    auto file = read_map_file();
    if (!file.has_value()) return std::nullopt;

    std::array<char, NONCE_LEN> nonce;
    file->read(nonce.data(), nonce.size());
    
    char c;
    while(file.value() >> c)
      ciphertext.push_back(c);

    auto decrypted_result = decryptor.decrypt(ciphertext, (unsigned char*)nonce.data(), password);
    if (!decrypted_result.has_value()) return std::nullopt;
    std::vector<char> v = decrypted_result.value();

    Map map;

    auto iter = v.begin();

    uint16_t size;
    if (!read_from_iterator(iter, v.end(), size)) {
      std::cerr << "ERROR: Could not parse map file\n";
      return map;
    }

    for (uint16_t i = 0; i < size; i++) {
      std::string key = deserialize_string(iter, v.end());
      Login l = Login::deserialize(iter, v.end());

      map.insert(std::make_pair(key, l));
    }

    return map;
  }
};
