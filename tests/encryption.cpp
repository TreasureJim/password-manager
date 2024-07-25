#include <iostream>
#include <ostream>
#include <sodium.h>
#include <sodium/crypto_box.h>
#include <sodium/crypto_pwhash_argon2i.h>
#include <string>

#define KEY_LEN crypto_box_SEEDBYTES

#define PASSWORD "thisisapassword"
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + sizeof(PASSWORD))



int main () {
  if (sodium_init() < 0) {
    std::cout << "ERROR: libsodium initialization failed." << std::endl;
    return 1;
  }


	unsigned char ciphertext[CIPHERTEXT_LEN];
	{
		std::string content = "this is content";

		unsigned char salt[crypto_pwhash_SALTBYTES];
		unsigned char key[KEY_LEN];

		randombytes_buf(salt, sizeof salt);

		if (crypto_pwhash_argon2i(key, KEY_LEN, PASSWORD, sizeof(PASSWORD), salt, crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE, crypto_pwhash_ALG_DEFAULT)) {
			std::cerr << "ERROR: could not generate secret key. Possibly not enough memory." << std::endl;
			return 1;
		}

		unsigned char nonce[crypto_secretbox_NONCEBYTES];
		randombytes_buf(nonce, sizeof nonce);

		crypto_secretbox_easy(ciphertext, reinterpret_cast<unsigned char*>(content.data()), content.length(), nonce, key);
	}
	
	return 0;
}
