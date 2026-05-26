#include "templates.hpp"
#include "lang.hpp"

namespace WebTemplates {

std::string renderHeader(const std::string& page, Lang::Language lang) {
    const auto& s = Lang::get(lang);
    std::string langParam = (lang == Lang::Language::EN) ? "ru" : "en";

    auto active = [&](const std::string& p) -> std::string {
        return page == p ? "class='active'" : "";
    };

    return "<!DOCTYPE html>\n"
           "<html lang=\"" + std::string(lang == Lang::Language::RU ? "ru" : "en") + "\">\n"
           "<head>\n"
           "    <meta charset=\"UTF-8\">\n"
           "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
           "    <title>Cipher Tools</title>\n"
           "    <link rel=\"stylesheet\" href=\"/static/style.css\">\n"
           "</head>\n"
           "<body>\n"
           "    <div class=\"container\">\n"
           "        <div style=\"display:flex; justify-content:space-between; align-items:center;\">\n"
           "            <h1>Cipher Tools</h1>\n"
           "            <a href=\"/lang?set=" + langParam + "\" class=\"button\" "
           "style=\"background:#222; color:#0f0; padding:5px 12px; font-size:12px;\">"
           + s.lang_switch + "</a>\n"
           "        </div>\n"
           "        <div class=\"nav\">\n"
           "            <a href=\"/\" " + active("home") + ">" + s.nav_home + "</a>\n"
           "            <a href=\"/encrypt\" " + active("encrypt") + ">" + s.nav_aes_encrypt + "</a>\n"
           "            <a href=\"/decrypt\" " + active("decrypt") + ">" + s.nav_aes_decrypt + "</a>\n"
           "            <a href=\"/rsa/generate\" " + active("rsa_generate") + ">" + s.nav_rsa_keys + "</a>\n"
           "            <a href=\"/rsa/encrypt\" " + active("rsa_encrypt") + ">" + s.nav_rsa_encrypt + "</a>\n"
           "            <a href=\"/rsa/decrypt\" " + active("rsa_decrypt") + ">" + s.nav_rsa_decrypt + "</a>\n"
           "            <a href=\"/test\" " + active("test") + ">" + s.nav_fips_test + "</a>\n"
           "            <a href=\"/about\" " + active("about") + ">" + s.nav_about + "</a>\n"
           "        </div>\n"
           "        <div class=\"content\">\n";
}

std::string renderFooter() {
    return "        </div>\n"
           "    </div>\n"
           "</body>\n"
           "</html>\n";
}

std::string renderHomePage(Lang::Language lang) {
    const auto& s = Lang::get(lang);
    return "<div class=\"card\">\n"
           "    <h2>" + s.home_title + "</h2>\n"
           "    <p style=\"color:#aaa; line-height:1.7;\">" + s.home_desc + "</p>\n"
           "    <div class=\"button-group\">\n"
           "        <a href=\"/encrypt\" class=\"button\">" + s.home_btn_encrypt + "</a>\n"
           "        <a href=\"/rsa/generate\" class=\"button\" style=\"background:#333; color:#0f0;\">"
           + s.home_btn_rsa + "</a>\n"
           "    </div>\n"
           "</div>\n"
           "<div class=\"card\">\n"
           "    <h3>" + s.home_algorithms + "</h3>\n"
           "    <div class=\"stats-grid\">\n"
           "        <div class=\"stat-card\"><div class=\"stat-value\">AES-128</div>"
           "<div class=\"stat-label\">" + s.home_symmetric + "</div></div>\n"
           "        <div class=\"stat-card\"><div class=\"stat-value\">RSA-2048</div>"
           "<div class=\"stat-label\">" + s.home_asymmetric + "</div></div>\n"
           "        <div class=\"stat-card\"><div class=\"stat-value\">SHA-256</div>"
           "<div class=\"stat-label\">" + s.home_hash + "</div></div>\n"
           "        <div class=\"stat-card\"><div class=\"stat-value\">PBKDF2</div>"
           "<div class=\"stat-label\">" + s.home_key_derivation + "</div></div>\n"
           "    </div>\n"
           "</div>\n";
}

std::string renderEncryptPage(Lang::Language lang, const std::string& output, const std::string& error) {
    const auto& s = Lang::get(lang);
    std::string result_html;
    if (!output.empty()) {
        result_html = "<div class=\"card result\">\n"
                      "    <h3 style=\"color:#0f0;\">" + s.aes_encrypted_title + "</h3>\n"
                      "    <pre>" + output + "</pre>\n"
                      "</div>\n";
    } else if (!error.empty()) {
        result_html = "<div class=\"card result error\">\n"
                      "    <h3 style=\"color:#f55;\">" + s.error_title + "</h3>\n"
                      "    <p>" + error + "</p>\n"
                      "</div>\n";
    }
    return "<div class=\"card\">\n"
           "    <h2>" + s.aes_encrypt_title + "</h2>\n"
           "    <form method=\"POST\" action=\"/encrypt\">\n"
           "        <label>" + s.aes_encrypt_label_password + "</label>\n"
           "        <input type=\"password\" name=\"password\" required placeholder=\""
           + s.aes_encrypt_placeholder_password + "\">\n"
           "        <label>" + s.aes_encrypt_label_plaintext + "</label>\n"
           "        <textarea name=\"plaintext\" rows=\"6\" required placeholder=\""
           + s.aes_encrypt_placeholder_plaintext + "\"></textarea>\n"
           "        <button type=\"submit\">" + s.aes_encrypt_btn + "</button>\n"
           "    </form>\n"
           "</div>\n" + result_html;
}

std::string renderDecryptPage(Lang::Language lang, const std::string& output, const std::string& error) {
    const auto& s = Lang::get(lang);
    std::string result_html;
    if (!output.empty()) {
        result_html = "<div class=\"card result\">\n"
                      "    <h3 style=\"color:#0f0;\">" + s.aes_decrypted_title + "</h3>\n"
                      "    <pre>" + output + "</pre>\n"
                      "</div>\n";
    } else if (!error.empty()) {
        result_html = "<div class=\"card result error\">\n"
                      "    <h3 style=\"color:#f55;\">" + s.error_title + "</h3>\n"
                      "    <p>" + error + "</p>\n"
                      "</div>\n";
    }
    return "<div class=\"card\">\n"
           "    <h2>" + s.aes_decrypt_title + "</h2>\n"
           "    <form method=\"POST\" action=\"/decrypt\">\n"
           "        <label>" + s.aes_decrypt_label_password + "</label>\n"
           "        <input type=\"password\" name=\"password\" required placeholder=\""
           + s.aes_decrypt_placeholder_password + "\">\n"
           "        <label>" + s.aes_decrypt_label_ciphertext + "</label>\n"
           "        <textarea name=\"ciphertext\" rows=\"3\" required placeholder=\""
           + s.aes_decrypt_placeholder_ciphertext + "\"></textarea>\n"
           "        <label>" + s.aes_decrypt_label_salt + "</label>\n"
           "        <textarea name=\"salt\" rows=\"2\" required placeholder=\""
           + s.aes_decrypt_placeholder_salt + "\"></textarea>\n"
           "        <button type=\"submit\">" + s.aes_decrypt_btn + "</button>\n"
           "    </form>\n"
           "</div>\n" + result_html;
}

std::string renderTestPage(Lang::Language lang, const std::string& output) {
    const auto& s = Lang::get(lang);
    std::string result_html;
    if (!output.empty()) {
        result_html = "<div class=\"card result\">\n"
                      "    <h3 style=\"color:#0f0;\">" + s.fips_result + "</h3>\n"
                      "    <pre>" + output + "</pre>\n"
                      "</div>\n";
    }
    return "<div class=\"card\">\n"
           "    <h2>" + s.fips_title + "</h2>\n"
           "    <p style=\"color:#aaa; margin-bottom:15px;\">" + s.fips_desc + "</p>\n"
           "    <form method=\"POST\" action=\"/test\">\n"
           "        <button type=\"submit\">" + s.fips_btn + "</button>\n"
           "    </form>\n"
           "</div>\n" + result_html;
}

std::string renderAboutPage(Lang::Language lang) {
    const auto& s = Lang::get(lang);
    return "<div class=\"card\">\n"
           "    <h2>" + s.about_title + "</h2>\n"
           "    <p style=\"color:#aaa; line-height:1.7; margin-bottom:15px;\">" + s.about_desc + "</p>\n"
           "    <h3>" + s.about_implemented + "</h3>\n"
           "    <ul>\n"
           "        <li>AES-128 encryption/decryption</li>\n"
           "        <li>RSA-2048 key generation and encryption</li>\n"
           "        <li>BigInt (arbitrary precision arithmetic)</li>\n"
           "        <li>Miller-Rabin primality test</li>\n"
           "        <li>SHA-256</li>\n"
           "        <li>HMAC-SHA256 &amp; PBKDF2</li>\n"
           "    </ul>\n"
           "</div>\n";
}

std::string renderRSAGeneratePage(Lang::Language lang, const std::string& output, const std::string& error) {
    const auto& s = Lang::get(lang);
    std::string result_html;
    if (!output.empty()) {
        result_html = "<div class=\"card result\">\n"
                      "    <h3 style=\"color:#0f0;\">" + s.rsa_gen_success + "</h3>\n"
                      + output +
                      "</div>\n";
    } else if (!error.empty()) {
        result_html = "<div class=\"card result error\">\n"
                      "    <h3 style=\"color:#f55;\">" + s.error_title + "</h3>\n"
                      "    <p>" + error + "</p>\n"
                      "</div>\n";
    }
    return "<div class=\"card\">\n"
           "    <h2>" + s.rsa_gen_title + "</h2>\n"
           "    <p style=\"color:#aaa; margin-bottom:15px;\">" + s.rsa_gen_warning + "</p>\n"
           "    <form method=\"POST\" action=\"/rsa/generate\">\n"
           "        <button type=\"submit\">" + s.rsa_gen_btn + "</button>\n"
           "    </form>\n"
           "</div>\n" + result_html;
}

std::string renderRSAEncryptPage(Lang::Language lang, const std::string& output, const std::string& error) {
    const auto& s = Lang::get(lang);
    std::string result_html;
    if (!output.empty()) {
        result_html = "<div class=\"card result\">\n"
                      "    <h3 style=\"color:#0f0;\">" + s.rsa_enc_success + "</h3>\n"
                      + output +
                      "</div>\n";
    } else if (!error.empty()) {
        result_html = "<div class=\"card result error\">\n"
                      "    <h3 style=\"color:#f55;\">" + s.error_title + "</h3>\n"
                      "    <p>" + error + "</p>\n"
                      "</div>\n";
    }
    return "<div class=\"card\">\n"
           "    <h2>" + s.rsa_enc_title + "</h2>\n"
           "    <form method=\"POST\" action=\"/rsa/encrypt\">\n"
           "        <label>" + s.rsa_enc_label_n + "</label>\n"
           "        <textarea name=\"pubkey_n\" rows=\"6\" required placeholder=\""
           + s.rsa_enc_placeholder_n + "\"></textarea>\n"
           "        <label>" + s.rsa_enc_label_e + "</label>\n"
           "        <input type=\"text\" name=\"pubkey_e\" required placeholder=\""
           + s.rsa_enc_placeholder_e + "\">\n"
           "        <label>" + s.rsa_enc_label_plaintext + "</label>\n"
           "        <textarea name=\"plaintext\" rows=\"4\" required placeholder=\""
           + s.rsa_enc_placeholder_plaintext + "\"></textarea>\n"
           "        <button type=\"submit\">" + s.rsa_enc_btn + "</button>\n"
           "    </form>\n"
           "</div>\n" + result_html;
}

std::string renderRSADecryptPage(Lang::Language lang, const std::string& output, const std::string& error) {
    const auto& s = Lang::get(lang);
    std::string result_html;
    if (!output.empty()) {
        result_html = "<div class=\"card result\">\n"
                      "    <h3 style=\"color:#0f0;\">" + s.rsa_dec_success + "</h3>\n"
                      + output +
                      "</div>\n";
    } else if (!error.empty()) {
        result_html = "<div class=\"card result error\">\n"
                      "    <h3 style=\"color:#f55;\">" + s.error_title + "</h3>\n"
                      "    <p>" + error + "</p>\n"
                      "</div>\n";
    }
    return "<div class=\"card\">\n"
           "    <h2>" + s.rsa_dec_title + "</h2>\n"
           "    <form method=\"POST\" action=\"/rsa/decrypt\">\n"
           "        <label>" + s.rsa_dec_label_n + "</label>\n"
           "        <textarea name=\"privkey_n\" rows=\"6\" required placeholder=\""
           + s.rsa_dec_placeholder_n + "\"></textarea>\n"
           "        <label>" + s.rsa_dec_label_d + "</label>\n"
           "        <textarea name=\"privkey_d\" rows=\"6\" required placeholder=\""
           + s.rsa_dec_placeholder_d + "\"></textarea>\n"
           "        <label>" + s.rsa_dec_label_cipher + "</label>\n"
           "        <textarea name=\"ciphertext\" rows=\"4\" required placeholder=\""
           + s.rsa_dec_placeholder_cipher + "\"></textarea>\n"
           "        <button type=\"submit\">" + s.rsa_dec_btn + "</button>\n"
           "    </form>\n"
           "</div>\n" + result_html;
}

} // namespace WebTemplates