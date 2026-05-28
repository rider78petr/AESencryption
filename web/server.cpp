#include "handlers.hpp"
#include "config.hpp"
#include <iostream>

int main() {
    httplib::Server app;

    app.Get("/lang", WebHandlers::handleLangSwitch);

    app.Get("/", WebHandlers::handleHome);
    app.Get("/encrypt", WebHandlers::handleEncryptForm);
    app.Post("/encrypt", WebHandlers::handleEncryptSubmit);
    app.Get("/decrypt", WebHandlers::handleDecryptForm);
    app.Post("/decrypt", WebHandlers::handleDecryptSubmit);
    app.Get("/test", WebHandlers::handleTestPage);
    app.Post("/test", WebHandlers::handleTestRun);
    app.Get("/about", WebHandlers::handleAbout);
    app.Get("/docs", WebHandlers::handleDocs);

    app.Get("/rsa/generate", WebHandlers::handleRSAGenerate);
    app.Post("/rsa/generate", WebHandlers::handleRSAGenerateSubmit);
    app.Get("/rsa/encrypt", WebHandlers::handleRSAEncrypt);
    app.Post("/rsa/encrypt", WebHandlers::handleRSAEncryptSubmit);
    app.Get("/rsa/decrypt", WebHandlers::handleRSADecrypt);
    app.Post("/rsa/decrypt", WebHandlers::handleRSADecryptSubmit);

    app.Get("/static/(.*)", [](const httplib::Request& req, httplib::Response& res) {
        std::string filename = req.matches[1];
        WebHandlers::handleStaticFile(filename, res);
    });

    std::cout << "\n========================================\n";
    std::cout << "  Cipher Tools (AES-128 & RSA-2048)\n";
    std::cout << "========================================\n";
    std::cout << "  URL: http://localhost:" << WebConfig::SERVER_PORT << "\n";
    std::cout << "========================================\n\n";
    std::cout << "Starting server..." << std::endl;

    if (!app.listen("0.0.0.0", WebConfig::SERVER_PORT)) {
        std::cerr << "Failed to start server on port " << WebConfig::SERVER_PORT << std::endl;
        return 1;
    }

    return 0;
}
