#pragma once
#include <string>
#include <map>

namespace Lang {

enum class Language { RU, EN };

// Все строки интерфейса
struct Strings {
    // Навигация
    std::string nav_home;
    std::string nav_aes_encrypt;
    std::string nav_aes_decrypt;
    std::string nav_rsa_keys;
    std::string nav_rsa_encrypt;
    std::string nav_rsa_decrypt;
    std::string nav_fips_test;
    std::string nav_about;
    std::string nav_docs;

    // Главная
    std::string home_title;
    std::string home_desc;
    std::string home_btn_encrypt;
    std::string home_btn_rsa;
    std::string home_algorithms;
    std::string home_symmetric;
    std::string home_asymmetric;
    std::string home_hash;
    std::string home_key_derivation;

    // AES encrypt
    std::string aes_encrypt_title;
    std::string aes_encrypt_label_password;
    std::string aes_encrypt_label_plaintext;
    std::string aes_encrypt_placeholder_password;
    std::string aes_encrypt_placeholder_plaintext;
    std::string aes_encrypt_btn;
    std::string aes_encrypted_title;

    // AES decrypt
    std::string aes_decrypt_title;
    std::string aes_decrypt_label_password;
    std::string aes_decrypt_label_ciphertext;
    std::string aes_decrypt_label_salt;
    std::string aes_decrypt_placeholder_password;
    std::string aes_decrypt_placeholder_ciphertext;
    std::string aes_decrypt_placeholder_salt;
    std::string aes_decrypt_btn;
    std::string aes_decrypted_title;

    // RSA generate
    std::string rsa_gen_title;
    std::string rsa_gen_warning;
    std::string rsa_gen_btn;
    std::string rsa_gen_success;
    std::string rsa_gen_public;
    std::string rsa_gen_private;
    std::string rsa_gen_save_hint;

    // RSA encrypt
    std::string rsa_enc_title;
    std::string rsa_enc_label_n;
    std::string rsa_enc_label_e;
    std::string rsa_enc_label_plaintext;
    std::string rsa_enc_placeholder_n;
    std::string rsa_enc_placeholder_e;
    std::string rsa_enc_placeholder_plaintext;
    std::string rsa_enc_btn;
    std::string rsa_enc_success;

    // RSA decrypt
    std::string rsa_dec_title;
    std::string rsa_dec_label_n;
    std::string rsa_dec_label_d;
    std::string rsa_dec_label_cipher;
    std::string rsa_dec_placeholder_n;
    std::string rsa_dec_placeholder_d;
    std::string rsa_dec_placeholder_cipher;
    std::string rsa_dec_btn;
    std::string rsa_dec_success;

    // FIPS test
    std::string fips_title;
    std::string fips_desc;
    std::string fips_btn;
    std::string fips_result;

    // About
    std::string about_title;
    std::string about_desc;
    std::string about_implemented;

    // Общие
    std::string error_title;
    std::string error_missing_fields;
    std::string lang_switch; // текст кнопки переключения
};

const Strings& get(Language lang);
Language fromCookie(const std::string& cookie);
std::string toCookieValue(Language lang);

} // namespace Lang