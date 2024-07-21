#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

class Login {
public:
	std::string username;
	std::string password;

	Login(std::string username, std::string password);

	void serialize(std::vector<char> &vec) const;
	static Login deserialize(std::vector<char>::const_iterator &iter, const std::vector<char>::const_iterator& end);
	friend std::ostream &operator<<(std::ostream &os, const Login &login);
};
