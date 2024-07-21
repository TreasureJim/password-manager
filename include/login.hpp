#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

class Login {
public:
	std::string username;
	std::string password;

	Login(std::string username, std::string password);

	void serialize(std::fstream& f) const;
	static Login deserialize(std::fstream& f);
	friend std::ostream &operator<<(std::ostream &os, const Login &login);
};
