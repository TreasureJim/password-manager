#include "login.hpp"
#include "serialisation.hpp"
#include <iostream>
#include <string>

std::ostream &operator<<(std::ostream &os, const Login &login) {
  os << login.username << " : " << login.password << std::endl;
  return os;
}

Login::Login(std::string username, std::string password) {
  this->username = username;
  this->password = password;
}

void Login::serialize(std::vector<char> &vec) const {
  serialize_string(vec, this->username);
  serialize_string(vec, this->password);
}

Login Login::deserialize(std::vector<char>::const_iterator &iter,
                         const std::vector<char>::const_iterator &end) {
  auto username = deserialize_string(iter, end);
  auto password = deserialize_string(iter, end);

  return Login(username, password);
}
