#include <iostream> // For input/output operations
#include <iomanip> // For output formatting
#include <sstream> // For string stream processing
#include <chrono> // For high-precision timing
#include <openssl/evp.h> // For SHA256 functions

using namespace std;
using namespace chrono;

// Character set used for brute-force attempts (lowercase, uppercase, digits)
const string CHARSET = "abcdefghijklmnopqrstuvwxyz"; // ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789
// Maximum password length to try
const int MAX_LENGTH = 6;
// Flag to indicate if the password has been found
bool FOUND = false;

string sha256(const string &input)
{
    // Buffer to hold the raw binary hash result
    unsigned char hash[EVP_MAX_MD_SIZE];
    // Length of the hash
    unsigned int hash_len = 0;

     // Create a new message digest context
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    // Initialize the context with SHA-256 algorithm
    EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr);
    // Update the context with the input data
    EVP_DigestUpdate(mdctx, input.data(), input.size());
    // Finalize the digest and get the hash
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    // Free the message digest context
    EVP_MD_CTX_free(mdctx);

    // Convert the hash to a hexadecimal string
    ostringstream oss;
    for (unsigned int i = 0; i < hash_len; ++i)
    {
        oss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return oss.str();
}

void brute_force(const string &target_hash, string &result, char *buffer, int depth, int target_length)
{
    if (FOUND)
        return;

    // If we've reached the current target length, test the password
    if (depth == target_length)
    {
        // Avoid string allocation, use buffer directly
        if (sha256(buffer) == target_hash)
        {
            result.assign(buffer, target_length);
            FOUND = true;
        }

        return;
    }

    // Try each character from the charset at the current position
    for (char ch : CHARSET)
        if (!FOUND)
        {
            buffer[depth] = ch;
            brute_force(target_hash, result, buffer, depth + 1, target_length);
        }
}

void decrypt_sha256(const string &target_hash, string &result)
{
    // Initialize a buffer to hold the current password attempt
    char buffer[MAX_LENGTH + 1] = {0};

    // Iterate over all possible lengths from 1 to MAX_LENGTH
    for (int length = 1; length <= MAX_LENGTH; ++length)
        for (char ch : CHARSET)
            if (!FOUND)
            {
                buffer[0] = ch;
                brute_force(target_hash, result, buffer, 1, length);
            }
}

int main()
{
    // string input = "kgg"; // Example input to hash
    // string target_hash = sha256(input);
    // cout << input << " -> " << target_hash << endl;
    string found_result;
    string target_hash = "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824"; // Example hash
    cout << "Target hash: " << target_hash << endl;

    // Start timing the decryption process
    auto start = high_resolution_clock::now(); 

    // Call the decryption function to find the password
    decrypt_sha256(target_hash, found_result);

    // End timing the decryption process
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    if (FOUND)
        cout << "Found: " << found_result << endl;
    else
        cout << "Not found" << endl;

    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    return 0;
}
