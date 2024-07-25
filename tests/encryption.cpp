#include <cstring>
#include <iostream>
#include <ostream>
#include <sodium.h>
#include <string>
#include <vector>

#define PASS 0
#define FAIL 1

#define KEY_LEN 128


int main() {
  if (sodium_init() < 0) {
    std::cout << "ERROR: libsodium initialization failed." << std::endl;
    return FAIL;
  }

  unsigned char PASSWORD_s[] = "thisisapassword";
  std::vector<char> password(PASSWORD_s, PASSWORD_s + sizeof(PASSWORD_s));
  unsigned char content_s[] = "this is content";
  std::vector<char> content(content_s, content_s + sizeof(content_s));

  unsigned char key[KEY_LEN];

  unsigned char nonce[crypto_secretbox_NONCEBYTES];
  randombytes_buf(nonce, sizeof(nonce));

  int CIPHERTEXT_LEN = (crypto_secretbox_MACBYTES + content.size());
  unsigned char ciphertext[CIPHERTEXT_LEN];
  {
    unsigned char salt[crypto_pwhash_SALTBYTES];
    randombytes_buf(salt, sizeof(salt));

    if (crypto_pwhash_argon2id(
            key, KEY_LEN, password.data(), password.size(), salt,
            crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MIN,
            crypto_pwhash_ALG_DEFAULT) != 0) {
      std::cerr
          << "ERROR: could not generate secret key. Possibly not enough memory."
          << std::endl;
      return FAIL;
    }

    crypto_secretbox_easy(ciphertext, (unsigned char*)content.data(), content.size(), nonce, key);
  }

  {
    unsigned char decrypted[content.size()];
    if (crypto_secretbox_open_easy(decrypted, ciphertext, CIPHERTEXT_LEN, nonce, key) != 0) {
      std::cerr << "ERROR: data not validated." << std::endl;
      return FAIL;
    }

    std::cout << "OUTPUT: " << decrypted << std::endl;

    if (strcmp((char *)decrypted, (char*)content.data()))
      return FAIL;
  }

  return PASS;
}
