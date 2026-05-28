#include "base64.hpp"
#include <sstream>
#include <iomanip>

namespace WebUtils {

    static const std::string BASE64_CHARS = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::string base64_encode(const std::vector<byte>& data) {
        return base64_encode(data.data(), data.size());
    }

    std::string base64_encode(const byte* data, size_t len) {
        std::string result;
        unsigned char buf[3];
        int bytes = 0;
        
        for (size_t idx = 0; idx < len; ++idx) {
            buf[bytes++] = data[idx];
            if (bytes == 3) {
                result += BASE64_CHARS[(buf[0] & 0xFC) >> 2];
                result += BASE64_CHARS[((buf[0] & 0x03) << 4) | ((buf[1] & 0xF0) >> 4)];
                result += BASE64_CHARS[((buf[1] & 0x0F) << 2) | ((buf[2] & 0xC0) >> 6)];
                result += BASE64_CHARS[buf[2] & 0x3F];
                bytes = 0;
            }
        }
        
        if (bytes == 1) {
            result += BASE64_CHARS[(buf[0] & 0xFC) >> 2];
            result += BASE64_CHARS[(buf[0] & 0x03) << 4];
            result += "==";
        } else if (bytes == 2) {
            result += BASE64_CHARS[(buf[0] & 0xFC) >> 2];
            result += BASE64_CHARS[((buf[0] & 0x03) << 4) | ((buf[1] & 0xF0) >> 4)];
            result += BASE64_CHARS[(buf[1] & 0x0F) << 2];
            result += "=";
        }
        return result;
    }

    std::vector<byte> base64_decode(const std::string& str) {
        std::vector<byte> result;
        unsigned char buf[4];
        int bytes = 0;
        
        for (char c : str) {
            if (c == '=') break;
            size_t pos = BASE64_CHARS.find(c);
            if (pos == std::string::npos) continue;
            buf[bytes++] = static_cast<unsigned char>(pos);
            if (bytes == 4) {
                result.push_back((buf[0] << 2) | (buf[1] >> 4));
                result.push_back((buf[1] << 4) | (buf[2] >> 2));
                result.push_back((buf[2] << 6) | buf[3]);
                bytes = 0;
            }
        }
        
        if (bytes == 3) {
            result.push_back((buf[0] << 2) | (buf[1] >> 4));
            result.push_back((buf[1] << 4) | (buf[2] >> 2));
        } else if (bytes == 2) {
            result.push_back((buf[0] << 2) | (buf[1] >> 4));
        }
        return result;
    }

    std::string bytesToHex(const byte* data, size_t len) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (size_t i = 0; i < len; ++i) {
            ss << std::setw(2) << static_cast<int>(data[i]);
        }
        return ss.str();
    }

    std::string htmlEscape(const std::string& str) {
        std::string result;
        for (char c : str) {
            switch (c) {
                case '&': result += "&amp;"; break;
                case '<': result += "&lt;"; break;
                case '>': result += "&gt;"; break;
                case '"': result += "&quot;"; break;
                case '\'': result += "&#39;"; break;
                default: result += c; break;
            }
        }
        return result;
    }
}