#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <string>
#include "picosha2.h"  // SHA-256 Lib: https://github.com/okdshin/PicoSHA2

// Custom mix function based on a simplified integral formula
double integral_mix_function(double x) {
    if (x == 0.0) x = 1e-10;
    try {
        return pow(x, sin(x)) * pow(63.0, x) + (1.0 / x);
    } catch (...) {
        return pow(x, sin(x)) * 1e10 + (1.0 / x);
    }
}

// Mix the SHA-256 hash using the custom function
std::vector<unsigned char> mix_data(const std::vector<unsigned char>& data, int rounds = 3) {
    std::vector<unsigned char> result = data;

    for (int r = 0; r < rounds; ++r) {
        for (size_t i = 0; i < result.size(); ++i) {
            double x = (static_cast<double>(result[i]) / 255.0) * M_PI;
            double mixed = integral_mix_function(x);
            int byte_val = static_cast<int>(mixed * 1e5) % 256;
            result[i] ^= static_cast<unsigned char>(byte_val);
        }
    }
    return result;
}

// Full HEL-420 hash function
std::string hel_420_hash(const std::string& input) {
    std::vector<unsigned char> hash_bytes(picosha2::k_digest_size);
    picosha2::hash256(input.begin(), input.end(), hash_bytes.begin(), hash_bytes.end());

    std::vector<unsigned char> mixed = mix_data(hash_bytes);

    std::string final_hash;
    picosha2::hash256(mixed.begin(), mixed.end(), final_hash);

    return final_hash;
}


// example main
int main() {
    std::string input = "HEL-420 test message";

    std::string hash_output = hel_420_hash(input);
    std::cout << "HEL-420 hash:\n" << hash_output << std::endl;

    return 0;
}
