#pragma once

#include <sodium.h>
#include <iostream>
#include <vector>
#include <string>

std::vector<unsigned char> derive_key(const std::string& password);
std::vector<unsigned char> encrypt_buffer(const std::vector<unsigned char>& input, const std::string& password);
std::vector<unsigned char> decrypt_buffer(const std::vector<unsigned char>& input, const std::string& password);

void encrypt_stream(std::vector<unsigned char> content, std::ostream& outStream, const std::string& password);
std::vector<unsigned char> decrypt_stream(std::istream& inStream, const std::string& password);
