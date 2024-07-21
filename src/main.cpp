#include <cstdio>
#include <ios>
#include <iostream>
#include <ostream>
#include <sodium.h>
#include <vector>

#include "cli.hpp"
#include "encryption.hpp"
#include "map.hpp"
#include "serialisation.hpp"

Map g_map;

int main(int argc, char *argv[]) {
  if (sodium_init() < 0) {
    std::cout << "ERROR: libsodium initialization failed." << std::endl;
    return 1;
  }

  std::cout << "Password Manager\n\n\n";

  auto pass = login_menu();

  {
    auto map_file = read_map_file();
    if (map_file.has_value()) {
      auto decrypted_stream = decrypt_stream(map_file.value(), pass);
      if (decrypted_stream.empty()) {
        std::cerr << "ERROR: couldn't decrypt file." << std::endl;
        return 1;
      }

      g_map = map_deserialize(
          reinterpret_cast<const std::vector<char> &>(decrypted_stream));
    }
  }

  main_menu();

  auto map_file = open_map_file();

  auto vector_saving = map_serialize(g_map);
  encrypt_stream(reinterpret_cast<std::vector<unsigned char> &>(vector_saving),
                 map_file, pass);

  return 0;
}
