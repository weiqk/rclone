#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <string>
#include <vector>
#include <curl/curl.h>

// Callback function for libcurl to write received data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    // Performs a simple HTTP GET request
    bool performGetRequest(const std::string& url, std::string& response_data, long& http_code);

    // Future methods:
    // bool performPostRequest(const std::string& url, const std::string& post_fields, std::string& response_data, long& http_code);
    // bool setHeaders(const std::vector<std::string>& headers);
    // void enableVerbose(bool enable);

private:
    CURL* curl_handle_;
    // std::vector<std::string> current_headers_;
};

#endif // NETWORK_MANAGER_HPP
