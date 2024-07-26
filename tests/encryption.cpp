#include <array>
#include <cstring>
#include <string>
#include <vector>
#include "encryption.hpp"

#define PASS 0
#define FAIL 1

int main() {
  std::string password = "thisisapassword";
  char content_s[] = "this is content";
  std::vector<char> content(content_s, content_s + sizeof(content_s));

  auto decryptor = Decryptor();
  if (decryptor.error) return FAIL;

  auto encrypted_pair = decryptor.encrypt(content, password);
  auto ciphertext = encrypted_pair->first;
  auto nonce = encrypted_pair->second;

  std::optional<std::vector<char>> decrypted = decryptor.decrypt(ciphertext, ciphertext.size(), nonce.data(), password);
  if (!decrypted.has_value()) return FAIL;

  if (strcmp(content_s, decrypted->data())) {
    return FAIL;
  }

  return PASS;
}
