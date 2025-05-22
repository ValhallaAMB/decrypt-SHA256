#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>   // Use OpenSSL EVP API

using namespace std;   

/*
The EVP functions provide a high level interface to OpenSSL cryptographic functions.
MD stands for `message digest` and uses the specified digest to create the key from the passphrase. The default algorithm is sha-256.
    A message digest or hash value is a numeric string generated using the cryptographic hash function. 
CTX  is a context structure that holds the state of the digest operation. 
*/

string hashToHex(const unsigned char* hash, unsigned int hash_len) {
    ostringstream oss;
    for(unsigned int i = 0; i < hash_len; ++i) {
        oss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return oss.str();
}

string sha256(const string& input) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr); // initializes the digest context with the specified algorithm.
    EVP_DigestUpdate(mdctx, input.data(), input.size()); // feeds data into the digest context.
    EVP_DigestFinal_ex(mdctx, hash, &hash_len); // finalizes the digest operation and returns the hash value.
    EVP_MD_CTX_free(mdctx); // frees the memory allocated for the digest context.

    cout << "input size: " << input.size() << endl;
    cout << "Hash length: " << hash_len << endl;

    // convert to hex string
    return hashToHex(hash, hash_len);
}

int main() {
    string input = "h";
    string hashed_string = sha256(input);
    cout << input << " -> " << hashed_string << endl;

    return 0;
}

