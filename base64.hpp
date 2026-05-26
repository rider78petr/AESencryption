#pragma once
#include <string>
#include <vector>

namespace WebUtils {

    using byte = unsigned char;
    
    /**
     * Encodes binary data to Base64 string
     */
    std::string base64_encode(const std::vector<byte>& data);
    
    /**
     * Encodes binary data to Base64 string
     */
    std::string base64_encode(const byte* data, size_t len);
    
    /**
     * Decodes Base64 string to binary data
     */
    std::vector<byte> base64_decode(const std::string& str);
    
    /**
     * Converts bytes to hexadecimal string
     */
    std::string bytesToHex(const byte* data, size_t len);
    
    /**
     * HTML escapes special characters
     */
    std::string htmlEscape(const std::string& str);
}