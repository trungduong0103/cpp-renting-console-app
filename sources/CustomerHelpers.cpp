#include "../headers/CustomerHelpers.h"

bool correct_customer_info_length(const std::string &line) {
    const unsigned int comma_count = std::count(line.begin(), line.end(), ',');
    if (comma_count != 5) {
        return false;
    }
    return true;
}

bool customer_id_is_valid(const std::string &customer_id) {

}

bool is_not_digit(const std::string &str) {
    const std::string allowed = "0123456789";
    if (str[0] == '-') {
        return str.substr(1, str.length() - 1).find_first_not_of(allowed) != std::string::npos;
    }
    return str.find_first_not_of(allowed) != std::string::npos;
}

bool customer_phone_is_valid(const std::string &phone) {
    const std::string default_error = "Phone is invalid";
    try {
        if (is_not_digit(phone)) {
            std::cerr << default_error << ", received: " << phone << std::endl;
            return false;
        }
        return true;
    } catch (std::invalid_argument &e) {
        std::cerr << default_error << ", received: " << phone << std::endl;
        return false;
    }
}