#include <iostream>
#include "network_manager.hpp"

int main(int argc, char *argv[]) {
    std::cout << "RcloneCpp experimental client" << std::endl;

    // It's better to manage curl_global_init/cleanup outside NetworkManager instances
    // if multiple instances or other libcurl uses are anticipated.
    // For this simple main, we ensure it's called.
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        std::cerr << "Failed to initialize libcurl globally in main." << std::endl;
        return 1;
    }

    NetworkManager nm;
    std::string response_body;
    long http_status_code;

    std::string url_to_test = "http://example.com";
    if (argc > 1) {
        url_to_test = argv[1]; // Allow testing a URL from command line
    }

    std::cout << "Attempting to GET: " << url_to_test << std::endl;

    if (nm.performGetRequest(url_to_test, response_body, http_status_code)) {
        std::cout << "HTTP Status Code: " << http_status_code << std::endl;
        std::cout << "Response Body:" << std::endl;
        // std::cout << response_body << std::endl; // Can be very long
        if (response_body.length() > 500) {
            std::cout << response_body.substr(0, 500) << "..." << std::endl;
            std::cout << "(Response truncated)" << std::endl;
        } else {
            std::cout << response_body << std::endl;
        }
    } else {
        std::cout << "Failed to perform GET request." << std::endl;
        if (http_status_code == -1) {
             std::cout << "A libcurl internal error likely occurred." << std::endl;
        }
    }

    // Cleanup libcurl global state
    curl_global_cleanup();

    return 0;
}
