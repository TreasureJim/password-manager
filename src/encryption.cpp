#include <array>
#include <iostream>
#include <optional>
#include <ostream>
#include <sodium.h>
#include <string>
#include <utility>
#include <vector>

#include "encryption.hpp"


Decryptor::Decryptor() {
  if (sodium_init() < 0) {
    std::cout << "ERROR: libsodium initialization failed." << std::endl;
    this->error = true;
  }
};

int Decryptor::generate_key(const std::string &password) {
  if (crypto_pwhash_argon2id(
          this->key, KEY_LEN, password.data(), password.size(), this->salt,
          crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MIN,
          crypto_pwhash_ALG_DEFAULT) != 0) {
    std::cerr
        << "ERROR: could not generate secret key. Possibly not enough memory."
        << std::endl;
    return 0;
  }

  return 1;
}

std::optional<std::pair<std::vector<char>, std::array<unsigned char, NONCE_LEN>>> Decryptor::encrypt(std::vector<char> &content, const std::string &password) {
  if (!this->generate_key(password)) {
    return std::nullopt;
  }

  int ciphertext_size = crypto_secretbox_MACBYTES + content.size();
  std::vector<char> ciphertext;
  ciphertext.resize(ciphertext_size);

  std::array<unsigned char, NONCE_LEN> nonce;
  randombytes_buf(nonce.data(), NONCE_LEN);

  crypto_secretbox_easy((unsigned char *)ciphertext.data(),
                        (unsigned char *)content.data(), content.size(), (unsigned char*)nonce.data(),
                        this->key);

  return std::make_pair(ciphertext, nonce);
}

std::optional<std::vector<char>> Decryptor::decrypt(std::vector<char> ciphertext, int ciphertext_len, unsigned char nonce[NONCE_LEN], const std::string& password) {
  if (!this->generate_key(password)) return std::nullopt;

  int decrypted_size = ciphertext_len - crypto_secretbox_MACBYTES;

  std::vector<char> decrypted;
  decrypted.resize(decrypted_size);

  if (crypto_secretbox_open_easy(
          (unsigned char *)decrypted.data(), (unsigned char *)ciphertext.data(),
          ciphertext_len, nonce, (unsigned char *)this->key) != 0) {
    std::cerr << "ERROR: data not validated." << std::endl;
    return std::nullopt;
  }

  return decrypted;
}
