#include "templates.hpp"
#include "templates_generated.hpp"
#include "lang.hpp"
#include <algorithm>

// CSS встроен в код
static const char* EMBEDDED_CSS = R"CSS(
* { margin: 0; padding: 0; box-sizing: border-box; }
body { background-color: #111; color: #ddd; font-family: monospace; padding: 20px; }
.container { max-width: 800px; margin: 0 auto; }
h1 { color: #0f0; font-size: 24px; padding: 20px 0; border-bottom: 1px solid #333; margin-bottom: 10px; }
.nav { display: flex; gap: 5px; padding: 10px 0; border-bottom: 1px solid #333; margin-bottom: 20px; flex-wrap: wrap; }
.nav a { color: #aaa; text-decoration: none; padding: 5px 10px; border: 1px solid #333; }
.nav a:hover { color: #0f0; border-color: #0f0; }
.nav a.active { color: #0f0; border-color: #0f0; }
.card { background: #1a1a1a; border: 1px solid #333; padding: 20px; margin-bottom: 20px; }
h2, h3 { color: #eee; margin-bottom: 15px; }
label { display: block; color: #aaa; margin: 12px 0 5px; font-size: 13px; }
input, textarea { width: 100%; background: #111; border: 1px solid #444; color: #eee; padding: 8px; font-family: monospace; font-size: 14px; }
input:focus, textarea:focus { outline: none; border-color: #0f0; }
textarea { resize: vertical; }
button, .button { background: #0f0; color: #000; border: none; padding: 8px 20px; font-family: monospace; font-size: 14px; font-weight: bold; cursor: pointer; margin-top: 12px; display: inline-block; text-decoration: none; }
button:hover, .button:hover { background: #0c0; }
.result { margin-top: 15px; padding: 15px; border-left: 3px solid #0f0; background: #111; }
.result.error { border-color: #f55; }
pre { background: #111; color: #0f0; padding: 12px; overflow-x: auto; font-size: 13px; border: 1px solid #333; white-space: pre-wrap; word-break: break-all; }
hr { border: none; border-top: 1px solid #333; margin: 15px 0; }
ul { margin-left: 20px; line-height: 1.8; color: #bbb; }
li { margin-bottom: 4px; }
.stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); gap: 10px; margin-top: 15px; }
.stat-card { background: #111; border: 1px solid #333; padding: 12px; text-align: center; }
.stat-value { font-size: 18px; font-weight: bold; color: #0f0; }
.stat-label { color: #777; font-size: 12px; margin-top: 4px; }
.button-group { display: flex; gap: 10px; margin-top: 15px; flex-wrap: wrap; }
a { color: #0f0; }
@media (max-width: 600px) { .nav a { font-size: 12px; padding: 4px 8px; } }
)CSS";

namespace WebTemplates {

// Заменяем плейсхолдер {{KEY}} на value в строке tmpl
static std::string replace(std::string tmpl, const std::string& key, const std::string& value) {
    std::string placeholder = "{{" + key + "}}";
    size_t pos = 0;
    while ((pos = tmpl.find(placeholder, pos)) != std::string::npos) {
        tmpl.replace(pos, placeholder.size(), value);
        pos += value.size();
    }
    return tmpl;
}

// Блок результата (успех или ошибка)
static std::string resultBlock(const std::string& title, const std::string& content, bool isError) {
    if (content.empty()) return "";
    std::string cls = isError ? "card result error" : "card result";
    std::string color = isError ? "#f55" : "#0f0";
    return "<div class=\"" + cls + "\">\n"
           "    <h3 style=\"color:" + color + ";\">" + title + "</h3>\n"
           "    <pre>" + content + "</pre>\n"
           "</div>\n";
}

std::string renderHeader(const std::string& page, Lang::Language lang) {
    const auto& s = Lang::get(lang);
    std::string langParam = (lang == Lang::Language::EN) ? "ru" : "en";

    auto active = [&](const std::string& p) -> std::string {
        return page == p ? "class='active'" : "";
    };

    return std::string("<!DOCTYPE html>\n<html lang=\"")
        + (lang == Lang::Language::RU ? "ru" : "en") + "\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>Cipher Tools</title>\n"
        "    <style>" + EMBEDDED_CSS + "</style>\n"
        "</head>\n"
        "<body>\n"
        "    <div class=\"container\">\n"
        "        <div style=\"display:flex; justify-content:space-between; align-items:center;\">\n"
        "            <h1>Cipher Tools</h1>\n"
        "            <a href=\"/lang?set=" + langParam + "\" class=\"button\" "
        "style=\"background:#222; color:#0f0; padding:5px 12px; font-size:12px;\">" + s.lang_switch + "</a>\n"
        "        </div>\n"
        "        <div class=\"nav\">\n"
        "            <a href=\"/\" " + active("home") + ">" + s.nav_home + "</a>\n"
        "            <a href=\"/encrypt\" " + active("encrypt") + ">" + s.nav_aes_encrypt + "</a>\n"
        "            <a href=\"/decrypt\" " + active("decrypt") + ">" + s.nav_aes_decrypt + "</a>\n"
        "            <a href=\"/rsa/generate\" " + active("rsa_generate") + ">" + s.nav_rsa_keys + "</a>\n"
        "            <a href=\"/rsa/encrypt\" " + active("rsa_encrypt") + ">" + s.nav_rsa_encrypt + "</a>\n"
        "            <a href=\"/rsa/decrypt\" " + active("rsa_decrypt") + ">" + s.nav_rsa_decrypt + "</a>\n"
        "            <a href=\"/test\" " + active("test") + ">" + s.nav_fips_test + "</a>\n"
        "            <a href=\"/docs\" " + active("docs") + ">" + s.nav_docs + "</a>\n"
        "            <a href=\"/about\" " + active("about") + ">" + s.nav_about + "</a>\n"
        "        </div>\n"
        "        <div class=\"content\">\n";
}

std::string renderFooter() {
    return "        </div>\n    </div>\n</body>\n</html>\n";
}

std::string renderHomePage(Lang::Language lang) {
    const auto& s = Lang::get(lang);
    std::string t = Templates::HOME;
    t = replace(t, "HOME_TITLE",  s.home_title);
    t = replace(t, "HOME_DESC",   s.home_desc);
    t = replace(t, "BTN_ENCRYPT", s.home_btn_encrypt);
    t = replace(t, "BTN_RSA",     s.home_btn_rsa);
    t = replace(t, "ALGORITHMS",  s.home_algorithms);
    t = replace(t, "SYMMETRIC",   s.home_symmetric);
    t = replace(t, "ASYMMETRIC",  s.home_asymmetric);
    t = replace(t, "HASH",        s.home_hash);
    t = replace(t, "KEY_DERIV",   s.home_key_derivation);
    return t;
}

std::string renderEncryptPage(Lang::Language lang, const std::string& output, const std::string& error) {
    const auto& s = Lang::get(lang);
    std::string result = output.empty()
        ? resultBlock(s.error_title, error, true)
        : resultBlock(s.aes_encrypted_title, output, false);
    std::string t = Templates::ENCRYPT;
    t = replace(t, "TITLE",               s.aes_encrypt_title);
    t = replace(t, "LABEL_PASSWORD",      s.aes_encrypt_label_password);
    t = replace(t, "PLACEHOLDER_PASSWORD",s.aes_encrypt_placeholder_password);
    t = replace(t, "LABEL_PLAINTEXT",     s.aes_encrypt_label_plaintext);
    t = replace(t, "PLACEHOLDER_PLAINTEXT",s.aes_encrypt_placeholder_plaintext);
    t = replace(t, "BTN",                 s.aes_encrypt_btn);
    t = replace(t, "RESULT",              result);
    return t;
}

std::string renderDecryptPage(Lang::Language lang, const std::string& output, const std::string& error) {
    const auto& s = Lang::get(lang);
    std::string result = output.empty()
        ? resultBlock(s.error_title, error, true)
        : resultBlock(s.aes_decrypted_title, output, false);
    std::string t = Templates::DECRYPT;
    t = replace(t, "TITLE",                s.aes_decrypt_title);
    t = replace(t, "LABEL_PASSWORD",       s.aes_decrypt_label_password);
    t = replace(t, "PLACEHOLDER_PASSWORD", s.aes_decrypt_placeholder_password);
    t = replace(t, "LABEL_CIPHERTEXT",     s.aes_decrypt_label_ciphertext);
    t = replace(t, "PLACEHOLDER_CIPHERTEXT",s.aes_decrypt_placeholder_ciphertext);
    t = replace(t, "LABEL_SALT",           s.aes_decrypt_label_salt);
    t = replace(t, "PLACEHOLDER_SALT",     s.aes_decrypt_placeholder_salt);
    t = replace(t, "BTN",                  s.aes_decrypt_btn);
    t = replace(t, "RESULT",               result);
    return t;
}

std::string renderTestPage(Lang::Language lang, const std::string& output) {
    const auto& s = Lang::get(lang);
    std::string result = output.empty() ? "" : resultBlock(s.fips_result, output, false);
    std::string t = Templates::TEST;
    t = replace(t, "TITLE",  s.fips_title);
    t = replace(t, "DESC",   s.fips_desc);
    t = replace(t, "BTN",    s.fips_btn);
    t = replace(t, "RESULT", result);
    return t;
}

std::string renderAboutPage(Lang::Language lang) {
    const auto& s = Lang::get(lang);
    std::string t = Templates::ABOUT;
    t = replace(t, "TITLE",       s.about_title);
    t = replace(t, "DESC",        s.about_desc);
    t = replace(t, "IMPLEMENTED", s.about_implemented);
    return t;
}

std::string renderRSAGeneratePage(Lang::Language lang, const std::string& output, const std::string& error) {
    const auto& s = Lang::get(lang);
    std::string result = error.empty()
        ? (output.empty() ? "" : "<div class=\"card result\">\n<h3 style=\"color:#0f0;\">" + s.rsa_gen_success + "</h3>\n" + output + "</div>\n")
        : resultBlock(s.error_title, error, true);
    std::string t = Templates::RSA_GENERATE;
    t = replace(t, "TITLE",   s.rsa_gen_title);
    t = replace(t, "WARNING", s.rsa_gen_warning);
    t = replace(t, "BTN",     s.rsa_gen_btn);
    t = replace(t, "RESULT",  result);
    return t;
}

std::string renderRSAEncryptPage(Lang::Language lang, const std::string& output, const std::string& error) {
    const auto& s = Lang::get(lang);
    std::string result = error.empty()
        ? (output.empty() ? "" : "<div class=\"card result\">\n<h3 style=\"color:#0f0;\">" + s.rsa_enc_success + "</h3>\n" + output + "</div>\n")
        : resultBlock(s.error_title, error, true);
    std::string t = Templates::RSA_ENCRYPT;
    t = replace(t, "TITLE",               s.rsa_enc_title);
    t = replace(t, "LABEL_N",             s.rsa_enc_label_n);
    t = replace(t, "PLACEHOLDER_N",       s.rsa_enc_placeholder_n);
    t = replace(t, "LABEL_E",             s.rsa_enc_label_e);
    t = replace(t, "PLACEHOLDER_E",       s.rsa_enc_placeholder_e);
    t = replace(t, "LABEL_PLAINTEXT",     s.rsa_enc_label_plaintext);
    t = replace(t, "PLACEHOLDER_PLAINTEXT",s.rsa_enc_placeholder_plaintext);
    t = replace(t, "BTN",                 s.rsa_enc_btn);
    t = replace(t, "RESULT",              result);
    return t;
}

std::string renderRSADecryptPage(Lang::Language lang, const std::string& output, const std::string& error) {
    const auto& s = Lang::get(lang);
    std::string result = error.empty()
        ? (output.empty() ? "" : "<div class=\"card result\">\n<h3 style=\"color:#0f0;\">" + s.rsa_dec_success + "</h3>\n" + output + "</div>\n")
        : resultBlock(s.error_title, error, true);
    std::string t = Templates::RSA_DECRYPT;
    t = replace(t, "TITLE",            s.rsa_dec_title);
    t = replace(t, "LABEL_N",          s.rsa_dec_label_n);
    t = replace(t, "PLACEHOLDER_N",    s.rsa_dec_placeholder_n);
    t = replace(t, "LABEL_D",          s.rsa_dec_label_d);
    t = replace(t, "PLACEHOLDER_D",    s.rsa_dec_placeholder_d);
    t = replace(t, "LABEL_CIPHER",     s.rsa_dec_label_cipher);
    t = replace(t, "PLACEHOLDER_CIPHER",s.rsa_dec_placeholder_cipher);
    t = replace(t, "BTN",              s.rsa_dec_btn);
    t = replace(t, "RESULT",           result);
    return t;
}

std::string renderDocsPage(Lang::Language lang) {
    // Docs страница без плейсхолдеров — разные версии для RU/EN
    if (lang == Lang::Language::RU) {
        return Templates::DOCS_RU;
    }
    return Templates::DOCS_EN;
}

} // namespace WebTemplates
