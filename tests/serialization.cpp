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

    auto new_map = des_res.value();
    map_print(new_map);


    // add check for comparing original and deserialized version
    auto value = new_map.find("test service");
    if (value == new_map.end()) return FAIL;
    if (!(value->second.username == "username" && value->second.password == "password")) return FAIL;
  }

  return PASS;
}
