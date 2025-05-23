#include <iostream>
#include <iomanip> // Input/output manipulators
#include <sstream> // String stream library
#include <chrono> // Time library
#include <openssl/evp.h> // Use OpenSSL EVP API
// #include <thread>

using namespace std;
using namespace chrono;

string sha256(const string &input)
{
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(mdctx, input.data(), input.size());
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    ostringstream oss;
    for (unsigned int i = 0; i < hash_len; ++i)
    {
        oss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return oss.str();
}

// Recursive brute-force function
bool brute_force(const string &target_hash, string &result, const string &charset, string current, int max_len)
{
    if (current.length() > max_len)
        return false;

    string current_hash = sha256(current);
    if (current_hash == target_hash)
    {
        cout << "Match Found: " << current << " -> " << current_hash << endl;
        result = current;
        return true;
    }

    for (char ch : charset)
    {
        if (brute_force(target_hash, result, charset, current + ch, max_len))
            return true;
    }
    return false;
}

int main()
{
    string input = "hello";
    string target_hash = sha256(input);
    cout << input << " -> " << target_hash << endl;

    const string charset = "abcdefghijklmnopqrstuvwxyz"; // ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789
    const int max_length = 5; // Limit to keep runtime reasonable

    string found_result;
    auto start = high_resolution_clock::now();

    bool found = brute_force(target_hash, found_result, charset, "", max_length);

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    if (!found)
        cout << "No match found within the given constraints." << endl;

    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    return 0;
}