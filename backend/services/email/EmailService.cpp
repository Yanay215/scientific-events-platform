#include "EmailService.h"

const std::string EMAIL_HOST = "smtp.gmail.com";
const std::string EMAIL_PORT = "587";
const std::string EMAIL_USER = "yanaigiz777@gmail.com";
const std::string EMAIL_PASSWORD = "yanturin.aygiz";

void EmailService::sendVerificationCode(const std::string &toEmail,
                                        const std::string &code,
                                        std::function<void(bool success)> &&callback) {
    std::string subject = "Scientific Events Platform Verification Code";
    std::string body = "Your verification code is: " + code;
    drogon::app().getEmailClient()->sendEmail(EMAIL_USER, toEmail, subject, body, [callback](bool success) {
        callback(success);
    });
}