#include "login.hpp"
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
