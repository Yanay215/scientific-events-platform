#include "EmailService.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <cstring>

struct UploadStatus {
    size_t bytes_read;
    std::string payload;
};

static size_t payloadSource(char *ptr, size_t size, size_t nmemb, void *userp) {
    UploadStatus *status = static_cast<UploadStatus *>(userp);
    size_t n = size * nmemb;
    if ((size==0) || (nmemb==0) || (n < 0)) return 0;
    if (!status->payload.empty() && status->bytes_read < status->payload.size()) {
        size_t len = status->payload.size() - status->bytes_read;
        if (len > n) len = n;
        memcpy(ptr, status->payload.c_str() + status->bytes_read, len);
        status->bytes_read += len;
        return len;
    }
    return 0;
}

void EmailService::sendVerificationCode(const std::string &toEmail,
                                        const std::string &code,
                                        std::function<void(bool success)> &&callback) {
    std::thread([toEmail, code, callback = std::move(callback)]() {
        std::string subject = "Scientific Events Platform Verification Code";
        std::string body = "Your verification code is: " + code;
        const char* envEmail = std::getenv("EMAIL_USER");
        std::string EMAIL_USER = envEmail ? std::string(envEmail) : "your_default_email@gmail.com";
        const char* envPassword = std::getenv("EMAIL_PASSWORD");
        std::string EMAIL_PASSWORD = envPassword ? std::string(envPassword) : "your_default_password";
        const char* envHost = std::getenv("EMAIL_HOST");
        std::string EMAIL_HOST = envHost ? std::string(envHost) : "smtp.gmail.com";
        const char* envPort = std::getenv("EMAIL_PORT");
        std::string EMAIL_PORT = envPort ? std::string(envPort) : "587";
        std::string smtpUrl = "smtp://" + EMAIL_HOST + ":" + EMAIL_PORT;
        std::stringstream payload;
        payload << "To: " << toEmail << "\r\n";
        payload << "From: " << EMAIL_USER << "\r\n";
        payload << "Subject: " << subject << "\r\n";
        payload << "Content-Type: text/plain; charset=UTF-8\r\n";
        payload << "\r\n";
        payload << body << "\r\n";
        UploadStatus status{0, payload.str()};
        CURL *curl = curl_easy_init();
        bool success = false;
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, smtpUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_USERNAME, EMAIL_USER.c_str());
            curl_easy_setopt(curl, CURLOPT_PASSWORD, EMAIL_PASSWORD.c_str());
            curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
            // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(curl, CURLOPT_MAIL_FROM, EMAIL_USER.c_str());
            struct curl_slist *recipients = nullptr;
            recipients = curl_slist_append(recipients, toEmail.c_str());
            curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, payloadSource);
            curl_easy_setopt(curl, CURLOPT_READDATA, &status);
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            // curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)status.payload.size());
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                success = false;
            } else {
                success = true;
            }
            curl_slist_free_all(recipients);
            curl_easy_cleanup(curl);
        }
        if (callback) callback(success);
    }).detach();
}