#pragma once
#include <drogon/drogon.h>
#include <string>
#include <functional>

class EmailService {
    public:
        static void sendVerificationCode(const std::string &toEmail,
                                        const std::string &code,
                                        std::function<void(bool success)> &&callback);
};