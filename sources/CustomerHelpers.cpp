#include "../headers/CustomerHelpers.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> get_customer_as_vector(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delimiter, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delimiter.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

bool correct_customer_info_length(std::string &line) {
    const unsigned int comma_count = std::count(line.begin(), line.end(), ',');
    if (comma_count != 5) {
        return false;
    }
    return true;
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

bool id_number_is_not_numeric(const std::string &id_number) {
    const std::string numerics = "0123456789";
    return id_number.find_first_not_of(numerics) != std::string::npos;
}

bool customer_id_is_valid_menu(const std::string &id) {
    // format: Cxxx
    std::vector<std::string> id_pool;
    std::string default_error = "Item ID is incorrect format ";

    // id length of item must be 4
    if (id.length() != 4) {
        std::cerr << default_error << "(wrong length)!" << std::endl;
        return false;
    }

    char first_letter = id[0];
    const std::string id_number = id.substr(1, 3);

    // first letter must be "C".
    if (first_letter != 'C') {
        std::cerr << default_error << "(must begin with 'C')!" << std::endl;
        return false;
    }

    // ‘xxx’ is a unique code of 3 digits (e.g. 123)
    if (id_number_is_not_numeric(id_number)) {
        std::cerr << default_error << "(ID number in Item ID must be numerics)!" << std::endl;
        return false;
    }

    return true;
}