#pragma once
#include <string>
#include <vector>

namespace WebUtils {

    using byte = unsigned char;
    
    std::string base64_encode(const std::vector<byte>& data);
    std::string base64_encode(const byte* data, size_t len);
    std::vector<byte> base64_decode(const std::string& str);
    std::string bytesToHex(const byte* data, size_t len);
    std::string htmlEscape(const std::string& str);
}
