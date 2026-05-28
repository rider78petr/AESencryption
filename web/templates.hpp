#pragma once

#include "lang.hpp"
#include <string>

namespace WebTemplates {
    std::string renderHeader(const std::string& page, Lang::Language lang);
    std::string renderFooter();

    std::string renderHomePage(Lang::Language lang);
    std::string renderEncryptPage(Lang::Language lang, const std::string& output = "", const std::string& error = "");
    std::string renderDecryptPage(Lang::Language lang, const std::string& output = "", const std::string& error = "");
    std::string renderTestPage(Lang::Language lang, const std::string& output = "");
    std::string renderAboutPage(Lang::Language lang);
    std::string renderDocsPage(Lang::Language lang);

    std::string renderRSAGeneratePage(Lang::Language lang, const std::string& output = "", const std::string& error = "");
    std::string renderRSAEncryptPage(Lang::Language lang, const std::string& output = "", const std::string& error = "");
    std::string renderRSADecryptPage(Lang::Language lang, const std::string& output = "", const std::string& error = "");
}