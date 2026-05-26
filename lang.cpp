#include "lang.hpp"

namespace Lang {

static const Strings RU_STRINGS = {
    // Навигация
    .nav_home         = "главная",
    .nav_aes_encrypt  = "aes шифр",
    .nav_aes_decrypt  = "aes дешифр",
    .nav_rsa_keys     = "rsa ключи",
    .nav_rsa_encrypt  = "rsa шифр",
    .nav_rsa_decrypt  = "rsa дешифр",
    .nav_fips_test    = "fips тест",
    .nav_about        = "о проекте",

    // Главная
    .home_title         = "Добро пожаловать",
    .home_desc          = "Реализация AES-128 и RSA на C++ без внешних криптобиблиотек.",
    .home_btn_encrypt   = "aes шифрование",
    .home_btn_rsa       = "сгенерировать rsa ключи",
    .home_algorithms    = "Алгоритмы",
    .home_symmetric     = "симметричный",
    .home_asymmetric    = "асимметричный",
    .home_hash          = "хэш",
    .home_key_derivation = "деривация ключа",

    // AES encrypt
    .aes_encrypt_title              = "AES Шифрование",
    .aes_encrypt_label_password     = "пароль:",
    .aes_encrypt_label_plaintext    = "открытый текст:",
    .aes_encrypt_placeholder_password  = "введите пароль...",
    .aes_encrypt_placeholder_plaintext = "введите текст для шифрования...",
    .aes_encrypt_btn                = "зашифровать",
    .aes_encrypted_title            = "зашифровано",

    // AES decrypt
    .aes_decrypt_title              = "AES Дешифрование",
    .aes_decrypt_label_password     = "пароль:",
    .aes_decrypt_label_ciphertext   = "шифротекст (base64):",
    .aes_decrypt_label_salt         = "соль (base64):",
    .aes_decrypt_placeholder_password   = "введите пароль...",
    .aes_decrypt_placeholder_ciphertext = "вставьте шифротекст...",
    .aes_decrypt_placeholder_salt       = "вставьте соль...",
    .aes_decrypt_btn                = "дешифровать",
    .aes_decrypted_title            = "дешифровано",

    // RSA generate
    .rsa_gen_title    = "Генерация RSA-2048 ключей",
    .rsa_gen_warning  = "Внимание: генерация ключей занимает 10-60 секунд. Пожалуйста подождите.",
    .rsa_gen_btn      = "сгенерировать ключи",
    .rsa_gen_success  = "ключи сгенерированы",
    .rsa_gen_public   = "Открытый ключ",
    .rsa_gen_private  = "Закрытый ключ",
    .rsa_gen_save_hint = "Сохраните закрытый ключ в надёжном месте!",

    // RSA encrypt
    .rsa_enc_title              = "RSA Шифрование",
    .rsa_enc_label_n            = "открытый ключ n (hex):",
    .rsa_enc_label_e            = "открытый ключ e (hex):",
    .rsa_enc_label_plaintext    = "открытый текст:",
    .rsa_enc_placeholder_n      = "вставьте открытый ключ n...",
    .rsa_enc_placeholder_e      = "обычно: 10001",
    .rsa_enc_placeholder_plaintext = "текст для шифрования...",
    .rsa_enc_btn                = "зашифровать",
    .rsa_enc_success            = "зашифровано",

    // RSA decrypt
    .rsa_dec_title              = "RSA Дешифрование",
    .rsa_dec_label_n            = "закрытый ключ n (hex):",
    .rsa_dec_label_d            = "закрытый ключ d (hex):",
    .rsa_dec_label_cipher       = "шифротекст (base64):",
    .rsa_dec_placeholder_n      = "вставьте закрытый ключ n...",
    .rsa_dec_placeholder_d      = "вставьте закрытый ключ d...",
    .rsa_dec_placeholder_cipher = "вставьте шифротекст...",
    .rsa_dec_btn                = "дешифровать",
    .rsa_dec_success            = "дешифровано",

    // FIPS test
    .fips_title  = "FIPS Тест",
    .fips_desc   = "Проверка AES-128 по официальным тест-векторам FIPS 197.",
    .fips_btn    = "запустить тест",
    .fips_result = "результат теста",

    // About
    .about_title       = "О проекте",
    .about_desc        = "Учебный проект. AES-128 и RSA реализованы с нуля на C++.",
    .about_implemented = "Реализовано:",

    // Общие
    .error_title         = "ошибка",
    .error_missing_fields = "Заполните все поля",
    .lang_switch         = "English",
};

static const Strings EN_STRINGS = {
    // Navigation
    .nav_home         = "home",
    .nav_aes_encrypt  = "aes encrypt",
    .nav_aes_decrypt  = "aes decrypt",
    .nav_rsa_keys     = "rsa keys",
    .nav_rsa_encrypt  = "rsa encrypt",
    .nav_rsa_decrypt  = "rsa decrypt",
    .nav_fips_test    = "fips test",
    .nav_about        = "about",

    // Home
    .home_title          = "Welcome",
    .home_desc           = "AES-128 and RSA implementation in C++ without external crypto libraries.",
    .home_btn_encrypt    = "aes encrypt",
    .home_btn_rsa        = "generate rsa keys",
    .home_algorithms     = "Algorithms",
    .home_symmetric      = "symmetric",
    .home_asymmetric     = "asymmetric",
    .home_hash           = "hash",
    .home_key_derivation = "key derivation",

    // AES encrypt
    .aes_encrypt_title              = "AES Encryption",
    .aes_encrypt_label_password     = "password:",
    .aes_encrypt_label_plaintext    = "plaintext:",
    .aes_encrypt_placeholder_password  = "enter password...",
    .aes_encrypt_placeholder_plaintext = "enter text to encrypt...",
    .aes_encrypt_btn                = "encrypt",
    .aes_encrypted_title            = "encrypted",

    // AES decrypt
    .aes_decrypt_title              = "AES Decryption",
    .aes_decrypt_label_password     = "password:",
    .aes_decrypt_label_ciphertext   = "ciphertext (base64):",
    .aes_decrypt_label_salt         = "salt (base64):",
    .aes_decrypt_placeholder_password   = "enter password...",
    .aes_decrypt_placeholder_ciphertext = "paste ciphertext...",
    .aes_decrypt_placeholder_salt       = "paste salt...",
    .aes_decrypt_btn                = "decrypt",
    .aes_decrypted_title            = "decrypted",

    // RSA generate
    .rsa_gen_title    = "Generate RSA-2048 Keys",
    .rsa_gen_warning  = "Warning: key generation takes 10-60 seconds. Please wait.",
    .rsa_gen_btn      = "generate keys",
    .rsa_gen_success  = "keys generated",
    .rsa_gen_public   = "Public Key",
    .rsa_gen_private  = "Private Key",
    .rsa_gen_save_hint = "Keep the private key in a safe place!",

    // RSA encrypt
    .rsa_enc_title              = "RSA Encryption",
    .rsa_enc_label_n            = "public key n (hex):",
    .rsa_enc_label_e            = "public key e (hex):",
    .rsa_enc_label_plaintext    = "plaintext:",
    .rsa_enc_placeholder_n      = "paste public key n...",
    .rsa_enc_placeholder_e      = "usually: 10001",
    .rsa_enc_placeholder_plaintext = "text to encrypt...",
    .rsa_enc_btn                = "encrypt",
    .rsa_enc_success            = "encrypted",

    // RSA decrypt
    .rsa_dec_title              = "RSA Decryption",
    .rsa_dec_label_n            = "private key n (hex):",
    .rsa_dec_label_d            = "private key d (hex):",
    .rsa_dec_label_cipher       = "ciphertext (base64):",
    .rsa_dec_placeholder_n      = "paste private key n...",
    .rsa_dec_placeholder_d      = "paste private key d...",
    .rsa_dec_placeholder_cipher = "paste ciphertext...",
    .rsa_dec_btn                = "decrypt",
    .rsa_dec_success            = "decrypted",

    // FIPS test
    .fips_title  = "FIPS Test",
    .fips_desc   = "Verifies AES-128 against official FIPS 197 test vectors.",
    .fips_btn    = "run test",
    .fips_result = "test result",

    // About
    .about_title       = "About",
    .about_desc        = "University project. AES-128 and RSA implemented from scratch in C++.",
    .about_implemented = "Implemented:",

    // Common
    .error_title          = "error",
    .error_missing_fields = "Please fill in all fields",
    .lang_switch          = "Русский",
};

const Strings& get(Language lang) {
    return lang == Language::RU ? RU_STRINGS : EN_STRINGS;
}

Language fromCookie(const std::string& cookie) {
    // ищем lang=en или lang=ru в строке cookie
    if (cookie.find("lang=en") != std::string::npos) return Language::EN;
    return Language::RU; // по умолчанию русский
}

std::string toCookieValue(Language lang) {
    return lang == Language::EN ? "en" : "ru";
}

} // namespace Lang