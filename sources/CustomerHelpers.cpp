#include "../headers/CustomerHelpers.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> get_customer_as_vector(const std::string &str) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(',', prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + 1;
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

bool correct_customer_info_length(const std::string &line) {
    const unsigned int comma_count = std::count(line.begin(), line.end(), ',');
    if (comma_count != 5) {
        std::cout << "[LOG] Customer info must have 5 commas, received: " << line << std::endl;
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


bool customer_id_number_is_not_numeric(const std::string &id_number) {
    const std::string numerics = "0123456789";
    return id_number.find_first_not_of(numerics) != std::string::npos;
}

bool customer_id_is_valid(const std::string &id, bool from_menu) {
    // format: Cxxx
    std::vector<std::string> id_pool;
    std::string default_error = "Customer ID is incorrect format ";
    std::string custom_text = from_menu ? "" : ", received: " + id;

    // id length of item must be 4
    if (id.length() != 4) {
        std::cerr << default_error << "(wrong length)" << custom_text << std::endl;
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
    if (customer_id_number_is_not_numeric(id_number)) {
        std::cerr << default_error << "(ID number in Item ID must be numerics)!" << std::endl;
        return false;
    }

    return true;
}

bool customer_name_is_valid(const std::string &name) {
//    const std::string
return false;
}

bool valid_customer_data(const std::string &line, const std::string &id) {
    if (!correct_customer_info_length(line)) {
        return false;
    }
    const std::vector<std::string> customer_vector = get_customer_as_vector(line);

    return true;
}