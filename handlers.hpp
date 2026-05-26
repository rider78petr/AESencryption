#pragma once

#define CPPHTTPLIB_NO_EXCEPTIONS
#include "httplib.h"
#include <string>

namespace WebHandlers {
    void handleHome(const httplib::Request& req, httplib::Response& res);
    void handleEncryptForm(const httplib::Request& req, httplib::Response& res);
    void handleEncryptSubmit(const httplib::Request& req, httplib::Response& res);
    void handleDecryptForm(const httplib::Request& req, httplib::Response& res);
    void handleDecryptSubmit(const httplib::Request& req, httplib::Response& res);
    void handleTestPage(const httplib::Request& req, httplib::Response& res);
    void handleTestRun(const httplib::Request& req, httplib::Response& res);
    void handleAbout(const httplib::Request& req, httplib::Response& res);
    void handleStaticFile(const std::string& filename, httplib::Response& res);
    void handleLangSwitch(const httplib::Request& req, httplib::Response& res);

    void handleRSAGenerate(const httplib::Request& req, httplib::Response& res);
    void handleRSAGenerateSubmit(const httplib::Request& req, httplib::Response& res);
    void handleRSAEncrypt(const httplib::Request& req, httplib::Response& res);
    void handleRSAEncryptSubmit(const httplib::Request& req, httplib::Response& res);
    void handleRSADecrypt(const httplib::Request& req, httplib::Response& res);
    void handleRSADecryptSubmit(const httplib::Request& req, httplib::Response& res);
}