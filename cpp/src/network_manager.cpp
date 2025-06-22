#include "network_manager.hpp"
#include <iostream>

// Callback function for libcurl to write received data into a std::string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

NetworkManager::NetworkManager() : curl_handle_(nullptr) {
    // Initialize libcurl globally (if not already done)
    // CURL_GLOBAL_DEFAULT is usually sufficient.
    // This should ideally be called once per program, not per NetworkManager instance.
    // For simplicity in this example, we might call it here, but in a larger app,
    // it should be managed carefully.
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_init_res != CURLE_OK) {
        std::cerr << "curl_global_init() failed: " << curl_easy_strerror(global_init_res) << std::endl;
        // In a real application, you might throw an exception or handle this error more robustly.
    }

    curl_handle_ = curl_easy_init();
    if (!curl_handle_) {
        std::cerr << "curl_easy_init() failed." << std::endl;
        // Handle error, perhaps throw an exception
    }
}

NetworkManager::~NetworkManager() {
    if (curl_handle_) {
        curl_easy_cleanup(curl_handle_);
    }
    // Corresponds to curl_global_init. Should be called once when the program is done with libcurl.
    // curl_global_cleanup();
    // For simplicity, we'll call it here. In a larger application, manage this carefully.
    // If multiple NetworkManager instances exist, this could be problematic.
    // A better approach is a static counter or a dedicated global manager for init/cleanup.
    // For now, to keep it simple, we'll comment it out and assume it's handled elsewhere or at program exit.
}

bool NetworkManager::performGetRequest(const std::string& url, std::string& response_data, long& http_code) {
    if (!curl_handle_) {
        return false;
    }

    CURLcode res;
    response_data.clear(); // Clear previous response

    curl_easy_setopt(curl_handle_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(curl_handle_, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects
    // curl_easy_setopt(curl_handle_, CURLOPT_VERBOSE, 1L); // Uncomment for debugging

    // For HTTPS, libcurl usually handles system CAs. If specific CA handling is needed:
    // curl_easy_setopt(curl_handle_, CURLOPT_CAINFO, "/path/to/ca-bundle.crt");

    res = curl_easy_perform(curl_handle_);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        http_code = -1; // Indicate libcurl error
        return false;
    }

    curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &http_code);

    return true;
}

// Placeholder for future POST request implementation
// bool NetworkManager::performPostRequest(const std::string& url, const std::string& post_fields, std::string& response_data, long& http_code) {
//     if (!curl_handle_) {
//         return false;
//     }
//     // ... implementation ...
//     return true;
// }

// Placeholder for setting headers
// bool NetworkManager::setHeaders(const std::vector<std::string>& headers) {
//     // ... implementation ...
//     // struct curl_slist *chunk = NULL;
//     // for (const auto& header : headers) {
//     //     chunk = curl_slist_append(chunk, header.c_str());
//     // }
//     // curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, chunk);
//     // then remember to curl_slist_free_all(chunk) after perform or in destructor
//     return true;
// }

// Placeholder for verbose mode
// void NetworkManager::enableVerbose(bool enable) {
//     if (!curl_handle_) return;
//     curl_easy_setopt(curl_handle_, CURLOPT_VERBOSE, enable ? 1L : 0L);
// }
