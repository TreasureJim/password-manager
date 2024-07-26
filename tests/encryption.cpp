#include <array>
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include "encryption.hpp"
#include <iomanip>

#define PASS 0
#define FAIL 1

void print_vector_as_chars(const std::vector<char>& vec) {
    for (char ch : vec) {
        std::cout << ch;
    }
    std::cout << std::endl;
}

int main() {
  std::string password = "thisisapassword";
  char content_s[] = "this is content";
  std::vector<char> content(content_s, content_s + sizeof(content_s));

  auto decryptor = Decryptor();
  if (decryptor.error) return FAIL;

  auto encrypted_pair = decryptor.encrypt(content, password);
  if (!encrypted_pair.has_value()) return FAIL;
  auto ciphertext = encrypted_pair->first;
  auto nonce = encrypted_pair->second;

  std::optional<std::vector<char>> decrypted = decryptor.decrypt(ciphertext, ciphertext.size(), nonce.data(), password);
  if (!decrypted.has_value()) return FAIL;

  if (strcmp(content_s, decrypted->data())) {
    return FAIL;
  }

  std::cout << "CONTENT: " << content_s << "\n";
  std::cout << "DECRYPTED CONTENT: ";
  print_vector_as_chars(decrypted.value());

  return PASS;
}
