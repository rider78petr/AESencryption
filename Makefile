CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -D_WIN32_WINNT=0x0A00 -DCPPHTTPLIB_NO_EXCEPTIONS -I include -I web -I third_party/httplib
LDFLAGS = -lpthread -lws2_32 -lwsock32 -lcrypt32
BUILD_DIR = build
TARGET = aes_server

.PHONY: all clean dirs

all: dirs $(TARGET)

dirs:
	mkdir -p $(BUILD_DIR)/src/core $(BUILD_DIR)/web

$(TARGET): $(BUILD_DIR)/src/core/gf256.o $(BUILD_DIR)/src/core/sha256.o $(BUILD_DIR)/src/core/hmac.o $(BUILD_DIR)/src/core/pbkdf2.o $(BUILD_DIR)/src/core/BigInt.o $(BUILD_DIR)/src/core/BigIntMath.o $(BUILD_DIR)/src/core/PrimeGen.o $(BUILD_DIR)/src/core/RSAkeys.o $(BUILD_DIR)/src/core/RSAcipher.o $(BUILD_DIR)/web/base64.o $(BUILD_DIR)/web/crypto_helper.o $(BUILD_DIR)/web/handlers.o $(BUILD_DIR)/web/templates.o $(BUILD_DIR)/web/server.o $(BUILD_DIR)/web/rsa_helper.o $(BUILD_DIR)/web/lang.o
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/src/core/gf256.o: src/core/gf256.cpp
	$(CXX) $(CXXFLAGS) -c src/core/gf256.cpp -o $(BUILD_DIR)/src/core/gf256.o

$(BUILD_DIR)/src/core/sha256.o: src/core/sha256.cpp
	$(CXX) $(CXXFLAGS) -c src/core/sha256.cpp -o $(BUILD_DIR)/src/core/sha256.o

$(BUILD_DIR)/src/core/hmac.o: src/core/hmac.cpp
	$(CXX) $(CXXFLAGS) -c src/core/hmac.cpp -o $(BUILD_DIR)/src/core/hmac.o

$(BUILD_DIR)/src/core/pbkdf2.o: src/core/pbkdf2.cpp
	$(CXX) $(CXXFLAGS) -c src/core/pbkdf2.cpp -o $(BUILD_DIR)/src/core/pbkdf2.o

$(BUILD_DIR)/src/core/BigInt.o: src/core/BigInt.cpp include/BigInt.hpp
	$(CXX) $(CXXFLAGS) -c src/core/BigInt.cpp -o $(BUILD_DIR)/src/core/BigInt.o

$(BUILD_DIR)/src/core/BigIntMath.o: src/core/BigIntMath.cpp include/BigIntMath.hpp include/BigInt.hpp
	$(CXX) $(CXXFLAGS) -c src/core/BigIntMath.cpp -o $(BUILD_DIR)/src/core/BigIntMath.o

$(BUILD_DIR)/src/core/PrimeGen.o: src/core/PrimeGen.cpp include/PrimeGen.hpp include/BigIntMath.hpp
	$(CXX) $(CXXFLAGS) -c src/core/PrimeGen.cpp -o $(BUILD_DIR)/src/core/PrimeGen.o

$(BUILD_DIR)/src/core/RSAkeys.o: src/core/RSAkeys.cpp include/RSAkeys.hpp include/PrimeGen.hpp include/BigIntMath.hpp
	$(CXX) $(CXXFLAGS) -c src/core/RSAkeys.cpp -o $(BUILD_DIR)/src/core/RSAkeys.o

$(BUILD_DIR)/src/core/RSAcipher.o: src/core/RSAcipher.cpp include/RSAcipher.hpp include/RSAkeys.hpp include/BigIntMath.hpp
	$(CXX) $(CXXFLAGS) -c src/core/RSAcipher.cpp -o $(BUILD_DIR)/src/core/RSAcipher.o

$(BUILD_DIR)/web/base64.o: web/base64.cpp web/base64.hpp
	$(CXX) $(CXXFLAGS) -c web/base64.cpp -o $(BUILD_DIR)/web/base64.o

$(BUILD_DIR)/web/crypto_helper.o: web/crypto_helper.cpp web/crypto_helper.hpp
	$(CXX) $(CXXFLAGS) -c web/crypto_helper.cpp -o $(BUILD_DIR)/web/crypto_helper.o

$(BUILD_DIR)/web/lang.o: web/lang.cpp web/lang.hpp
	$(CXX) $(CXXFLAGS) -c web/lang.cpp -o $(BUILD_DIR)/web/lang.o

$(BUILD_DIR)/web/handlers.o: web/handlers.cpp web/handlers.hpp web/lang.hpp
	$(CXX) $(CXXFLAGS) -c web/handlers.cpp -o $(BUILD_DIR)/web/handlers.o

$(BUILD_DIR)/web/templates.o: web/templates.cpp web/templates.hpp web/lang.hpp
	$(CXX) $(CXXFLAGS) -c web/templates.cpp -o $(BUILD_DIR)/web/templates.o

$(BUILD_DIR)/web/server.o: web/server.cpp web/handlers.hpp web/config.hpp
	$(CXX) $(CXXFLAGS) -c web/server.cpp -o $(BUILD_DIR)/web/server.o

$(BUILD_DIR)/web/rsa_helper.o: web/rsa_helper.cpp web/rsa_helper.hpp
	$(CXX) $(CXXFLAGS) -c web/rsa_helper.cpp -o $(BUILD_DIR)/web/rsa_helper.o

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
