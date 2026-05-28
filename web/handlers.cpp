#include "handlers.hpp"
#include "templates.hpp"
#include "crypto_helper.hpp"
#include "rsa_helper.hpp"
#include "base64.hpp"
#include "lang.hpp"
#include <fstream>
#include <sstream>

namespace WebHandlers {

static Lang::Language getLang(const httplib::Request& req) {
    auto it = req.headers.find("Cookie");
    if (it != req.headers.end()) {
        return Lang::fromCookie(it->second);
    }
    return Lang::Language::RU;
}

static void setLangCookie(httplib::Response& res, Lang::Language lang) {
    std::string cookie = "lang=" + Lang::toCookieValue(lang) + "; Path=/; Max-Age=31536000";
    res.set_header("Set-Cookie", cookie);
}

void handleLangSwitch(const httplib::Request& req, httplib::Response& res) {
    std::string set = req.get_param_value("set");
    Lang::Language lang = (set == "en") ? Lang::Language::EN : Lang::Language::RU;
    setLangCookie(res, lang);
    res.set_redirect("/");
}

void handleStaticFile(const std::string& filename, httplib::Response& res) {
    std::string filepath = "web/static/" + filename;
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        res.status = 404;
        res.set_content("File not found: " + filename, "text/plain");
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content_type = "text/plain";
    if (filename.ends_with(".css"))  content_type = "text/css";
    if (filename.ends_with(".js"))   content_type = "application/javascript";
    if (filename.ends_with(".html")) content_type = "text/html";
    res.set_content(buffer.str(), content_type);
}

void handleHome(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string html = WebTemplates::renderHeader("home", lang);
    html += WebTemplates::renderHomePage(lang);
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleEncryptForm(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string html = WebTemplates::renderHeader("encrypt", lang);
    html += WebTemplates::renderEncryptPage(lang);
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleEncryptSubmit(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string password  = req.get_param_value("password");
    std::string plaintext = req.get_param_value("plaintext");
    std::string html = WebTemplates::renderHeader("encrypt", lang);
    if (password.empty() || plaintext.empty()) {
        html += WebTemplates::renderEncryptPage(lang, "", Lang::get(lang).error_missing_fields);
    } else {
        auto result = WebCrypto::encryptText(plaintext, password);
        if (result.success) {
            std::string output = "Ciphertext (Base64):\n" + result.ciphertextBase64 +
                                 "\n\nSalt (Base64):\n" + result.saltBase64;
            html += WebTemplates::renderEncryptPage(lang, output, "");
        } else {
            html += WebTemplates::renderEncryptPage(lang, "", result.error);
        }
    }
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleDecryptForm(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string html = WebTemplates::renderHeader("decrypt", lang);
    html += WebTemplates::renderDecryptPage(lang);
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleDecryptSubmit(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string password   = req.get_param_value("password");
    std::string ciphertext = req.get_param_value("ciphertext");
    std::string salt       = req.get_param_value("salt");
    std::string html = WebTemplates::renderHeader("decrypt", lang);
    if (password.empty() || ciphertext.empty() || salt.empty()) {
        html += WebTemplates::renderDecryptPage(lang, "", Lang::get(lang).error_missing_fields);
    } else {
        auto result = WebCrypto::decryptText(ciphertext, salt, password);
        if (result.success)
            html += WebTemplates::renderDecryptPage(lang, result.plaintext, "");
        else
            html += WebTemplates::renderDecryptPage(lang, "", result.error);
    }
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleTestPage(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string html = WebTemplates::renderHeader("test", lang);
    html += WebTemplates::renderTestPage(lang);
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleTestRun(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    auto result = WebCrypto::runFIPSTest();
    std::string html = WebTemplates::renderHeader("test", lang);
    html += WebTemplates::renderTestPage(lang, result.details);
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleAbout(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string html = WebTemplates::renderHeader("about", lang);
    html += WebTemplates::renderAboutPage(lang);
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleRSAGenerate(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string html = WebTemplates::renderHeader("rsa_generate", lang);
    html += WebTemplates::renderRSAGeneratePage(lang);
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleRSAGenerateSubmit(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    const auto& s = Lang::get(lang);
    std::string html = WebTemplates::renderHeader("rsa_generate", lang);
    auto result = WebRSA::generateKeys();
    if (result.success) {
        std::string output =
            "<h3>" + s.rsa_gen_public + "</h3>"
            "<p style='color:#0f0;'>n (hex):</p><pre>" + result.publicKeyN + "</pre>"
            "<p style='color:#0f0;'>e (hex):</p><pre>" + result.publicKeyE + "</pre>"
            "<h3 style='margin-top:20px;'>" + s.rsa_gen_private + "</h3>"
            "<p style='color:#f55;'>n (hex):</p><pre>" + result.privateKeyN + "</pre>"
            "<p style='color:#f55;'>d (hex):</p><pre>" + result.privateKeyD + "</pre>"
            "<p style='color:#aaa; font-size:12px; margin-top:15px;'>" + s.rsa_gen_save_hint + "</p>";
        html += WebTemplates::renderRSAGeneratePage(lang, output, "");
    } else {
        html += WebTemplates::renderRSAGeneratePage(lang, "", result.error);
    }
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleRSAEncrypt(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string html = WebTemplates::renderHeader("rsa_encrypt", lang);
    html += WebTemplates::renderRSAEncryptPage(lang);
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleRSAEncryptSubmit(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string plaintext = req.get_param_value("plaintext");
    std::string pubkey_n  = req.get_param_value("pubkey_n");
    std::string pubkey_e  = req.get_param_value("pubkey_e");
    std::string html = WebTemplates::renderHeader("rsa_encrypt", lang);
    if (plaintext.empty() || pubkey_n.empty() || pubkey_e.empty()) {
        html += WebTemplates::renderRSAEncryptPage(lang, "", Lang::get(lang).error_missing_fields);
    } else {
        auto result = WebRSA::encryptWithPublicKey(plaintext, pubkey_n, pubkey_e);
        if (result.success)
            html += WebTemplates::renderRSAEncryptPage(lang, "<pre>" + result.result + "</pre>", "");
        else
            html += WebTemplates::renderRSAEncryptPage(lang, "", result.error);
    }
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleRSADecrypt(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string html = WebTemplates::renderHeader("rsa_decrypt", lang);
    html += WebTemplates::renderRSADecryptPage(lang);
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleRSADecryptSubmit(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string ciphertext = req.get_param_value("ciphertext");
    std::string privkey_n  = req.get_param_value("privkey_n");
    std::string privkey_d  = req.get_param_value("privkey_d");
    std::string html = WebTemplates::renderHeader("rsa_decrypt", lang);
    if (ciphertext.empty() || privkey_n.empty() || privkey_d.empty()) {
        html += WebTemplates::renderRSADecryptPage(lang, "", Lang::get(lang).error_missing_fields);
    } else {
        auto result = WebRSA::decryptWithPrivateKey(ciphertext, privkey_n, privkey_d);
        if (result.success)
            html += WebTemplates::renderRSADecryptPage(lang, "<pre>" + result.result + "</pre>", "");
        else
            html += WebTemplates::renderRSADecryptPage(lang, "", result.error);
    }
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

void handleDocs(const httplib::Request& req, httplib::Response& res) {
    auto lang = getLang(req);
    std::string html = WebTemplates::renderHeader("docs", lang);
    html += WebTemplates::renderDocsPage(lang);
    html += WebTemplates::renderFooter();
    res.set_content(html, "text/html");
}

}
