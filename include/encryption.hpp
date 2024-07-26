#pragma once

#include <optional>
#include <sodium.h>
#include <string>
#include <vector>

#define KEY_LEN 128
#define SALT_LEN crypto_pwhash_SALTBYTES
#define NONCE_LEN crypto_secretbox_NONCEBYTES

class Decryptor {
private:
  int generate_key(const std::string &password);
  constexpr static unsigned char salt[] = {0xfc, 0x6b, 0x55, 0xb6, 0xcf, 0x31,
                                           0xee, 0xcb, 0xcd, 0x5a, 0xec, 0x64,
                                           0x8f, 0x1c, 0x99, 0x98};

public:
  bool error = false;

  unsigned char key[KEY_LEN];

  Decryptor();
  std::optional<
      std::pair<std::vector<char>, std::array<unsigned char, NONCE_LEN>>>
  encrypt(std::vector<char> &content, const std::string &password);
  std::optional<std::vector<char>> decrypt(std::vector<char> ciphertext,
                                           int ciphertext_len,
                                           unsigned char nonce[NONCE_LEN],
                                           const std::string &password);
};
