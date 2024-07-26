#pragma once

#include <optional>
#include <sodium.h>
#include <string>
#include <vector>

#define KEY_LEN 128
#define NONCE_LEN crypto_secretbox_NONCEBYTES

class Decryptor {
private:
  int generate_key(const std::string &password);

public:
  bool error = false;

  unsigned char key[KEY_LEN];

  Decryptor();
  std::optional<std::pair<std::vector<char>, std::array<unsigned char, NONCE_LEN>>> encrypt(std::vector<char> &content, const std::string &password);
  std::optional<std::vector<char>> decrypt(std::vector<char> ciphertext, int ciphertext_len, unsigned char nonce[NONCE_LEN], const std::string& password);
};
