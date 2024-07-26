#include <cstdio>
#include <iostream>
#include <ostream>
#include <sodium.h>

#include "cli.hpp"
#include "map.hpp"
#include "serialisation.hpp"

Map g_map;

int main(int argc, char *argv[]) {
  std::cout << "Password Manager\n\n\n";

  auto pass = login_menu();

  Serializer serializer;
  if (serializer.error) {
    return 1;
  }

  auto deserializer_out = serializer.map_deserialize(pass);
  if (!deserializer_out.has_value()) {
    return 1;
  }

  g_map = deserializer_out.value();

  main_menu();

  serializer.map_serialize(g_map, pass);

  return 0;
}
