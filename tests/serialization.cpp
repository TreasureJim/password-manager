#include "map.hpp"
#include "serialisation.hpp"

#define PASS 0
#define FAIL 1

#define PASSWORD "password"

int main() {
  Map map_before_save;
  map_before_save.insert(std::make_pair("test service", Login("username", "password")));

  map_print(map_before_save);

  {
    Serializer serializer;
    if (serializer.error) return FAIL;
    
    if (!serializer.map_serialize(map_before_save, PASSWORD)) return FAIL;
  }
  
  {
    Serializer serializer;
    if (serializer.error) return FAIL;
    
    auto des_res = serializer.map_deserialize(PASSWORD);
    if (!des_res.has_value()) return FAIL;

    map_print(des_res.value());

    // add check for comparing original and deserialized version
  }

  return PASS;
}
