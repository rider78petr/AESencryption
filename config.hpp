#pragma once
#include <string>

namespace WebConfig {

    constexpr int SERVER_PORT = 9090;
    constexpr const char* SERVER_HOST = "localhost";
    constexpr int PBKDF2_ITERATIONS = 10000;
    constexpr int KEY_SIZE_BYTES = 16;  // AES-128
    constexpr int SALT_SIZE_BYTES = 16;
    
    inline std::string getServerUrl() {
        return "http://localhost:" + std::to_string(SERVER_PORT);
    }
    
    inline std::string getServerName() {
        return "AES-128 Encryption Server";
    }
}