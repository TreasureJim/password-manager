#include <sodium.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Function to derive a key from a password
std::vector<unsigned char> derive_key(const std::string& password) {
    std::vector<unsigned char> key(crypto_secretbox_KEYBYTES);
    std::vector<unsigned char> salt(crypto_pwhash_SALTBYTES);
    randombytes_buf(salt.data(), salt.size());

    if (crypto_pwhash(key.data(), key.size(), password.c_str(), password.length(), salt.data(),
                      crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        std::cerr << "Error: Key derivation failed" << std::endl;
        return {};
    }

    return key;
}

// Function to encrypt a buffer
std::vector<unsigned char> encrypt_buffer(const std::vector<unsigned char>& input, const std::string& password) {
    std::vector<unsigned char> key = derive_key(password);
    if (key.empty()) {
        return {};
    }

    std::vector<unsigned char> nonce(crypto_secretbox_NONCEBYTES);
    randombytes_buf(nonce.data(), nonce.size());

    std::vector<unsigned char> encrypted(crypto_secretbox_MACBYTES + input.size());

    crypto_secretbox_easy(encrypted.data(), input.data(), input.size(), nonce.data(), key.data());

    // Prepend nonce to encrypted data
    encrypted.insert(encrypted.begin(), nonce.begin(), nonce.end());

    return encrypted;
}

// Function to decrypt a buffer
std::vector<unsigned char> decrypt_buffer(const std::vector<unsigned char>& input, const std::string& password) {
    if (input.size() < crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES) {
        std::cerr << "Error: Input too short" << std::endl;
        return {};
    }

    std::vector<unsigned char> key = derive_key(password);
    if (key.empty()) {
        return {};
    }

    std::vector<unsigned char> nonce(input.begin(), input.begin() + crypto_secretbox_NONCEBYTES);
    
    std::vector<unsigned char> decrypted(input.size() - crypto_secretbox_NONCEBYTES - crypto_secretbox_MACBYTES);

    if (crypto_secretbox_open_easy(decrypted.data(), 
                                   input.data() + crypto_secretbox_NONCEBYTES, 
                                   input.size() - crypto_secretbox_NONCEBYTES, 
                                   nonce.data(), 
                                   key.data()) != 0) {
        std::cerr << "Error: Decryption failed" << std::endl;
        return {};
    }

    return decrypted;
}

// Function to read from a stream into a buffer
std::vector<unsigned char> readStream(std::istream& inStream) {
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(inStream), {});
    if (inStream.fail() && !inStream.eof()) {
        std::cerr << "Error: Failed to read from stream" << std::endl;
        return {};
    }
    return buffer;
}

// Function to write a buffer to a stream
void writeStream(std::ostream& outStream, const std::vector<unsigned char>& data) {
    outStream.write(reinterpret_cast<const char*>(data.data()), data.size());
    if (!outStream) {
        std::cerr << "Error: Failed to write to stream" << std::endl;
    }
}

// Function to encrypt a stream
void encrypt_stream(std::vector<unsigned char> content, std::ostream& outStream, const std::string& password) {
    if (content.empty()) {
        return;
    }
    std::vector<unsigned char> encrypted = encrypt_buffer(content, password);
    if (encrypted.empty()) {
        return;
    }
    writeStream(outStream, encrypted);
}

// Function to decrypt a stream to a buffer
std::vector<unsigned char> decrypt_stream(std::istream& inStream, const std::string& password) {
    std::vector<unsigned char> content = readStream(inStream);
    if (content.empty()) {
        return {};
    }
    return decrypt_buffer(content, password);
}
