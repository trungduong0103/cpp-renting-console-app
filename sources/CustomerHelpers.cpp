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
        if (token[token.length() - 1] == 32) token.erase(token.length() - 1, 1);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + 1;
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

bool correct_customer_info_length(const std::string &line) {
    const unsigned int comma_count = std::count(line.begin(), line.end(), ',');
    if (comma_count != 5) {
        std::cout << "[ERROR] Customer info must have 5 commas, received: " << line << std::endl;
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
    std::string default_error = "[ERROR] Customer ID is incorrect format ";
    std::string custom_text = from_menu ? "" : ", received: " + id;

    // id length of item must be 4
    if (id.length() != 4) {
        std::cout << default_error << "(wrong length)" << custom_text << std::endl;
        return false;
    }

    char first_letter = id[0];
    const std::string id_number = id.substr(1, 3);

    // first letter must be "C".
    if (first_letter != 'C') {
        std::cout << default_error << "(must begin with 'C')!" << std::endl;
        return false;
    }

    // ‘xxx’ is a unique code of 3 digits (e.g. 123)
    if (customer_id_number_is_not_numeric(id_number)) {
        std::cout << default_error << "(ID number in Item ID must be numerics)!" << std::endl;
        return false;
    }

    return true;
}

bool customer_name_is_valid(const std::string &name) {
    if (name.length() < 4) {
        std::cout << "[ERROR] Customer name must have at least 4 characters." << std::endl;
        return false;
    }
    for (char c : name) {
        if (!std::isalnum(c) && c != 32) {
            std::cout << "[ERROR] Customer name must not have special characters/digits, received " << name
                      << std::endl;
            return false;
        }
    }
    return true;
}

bool customer_address_is_valid(const std::string &address) {
    if (address.length() < 6) {
        std::cout << "[ERROR] Customer address must have at least 6 characters." << std::endl;
        return false;
    }
    for (char c : address) {
        if (!std::isalnum(c) && c != 32) {
            std::cout << "[ERROR] Customer address must not have special characters/digits, received: " << address
                      << std::endl;
            return false;
        }
    }
    return true;
}

bool customer_rental_number_is_valid(const std::string &rental_num) {
    const std::string default_error = "[ERROR] Customer number of rentals is invalid ";
    try {
        const int int_rental_num = std::stoi(rental_num);
        if (int_rental_num >= 0) {
            return true;
        }
        std::cout << default_error
                  << ", rental number must be bigger or equals to 0, received: "
                  << rental_num
                  << std::endl;
        return false;
    } catch (std::invalid_argument &e) {
        std::cout << default_error << ", received: " << rental_num << std::endl;
        return false;
    }
}

bool customer_type_is_valid(const std::string &type) {
    if (type != "Guest" && type != "Regular" && type != "VIP") {
        std::cout << "Customer must be either Guest, Regular, or VIP, received: " << type << std::endl;
        return false;
    }

    return true;
}

bool valid_customer_data(const std::string &line) {
    if (!correct_customer_info_length(line)) {
        return false;
    }
    const std::vector<std::string> customer_vector = get_customer_as_vector(line);
    return customer_id_is_valid(customer_vector[0], false) &&
           customer_name_is_valid(customer_vector[1]) &&
           customer_address_is_valid(customer_vector[2]) &&
           customer_rental_number_is_valid(customer_vector[4]) &&
           customer_type_is_valid(customer_vector[5]);
}

int get_number_of_videos(Customer const *customer) {
    int video_count = 0;

    for (auto const &item : customer->get_items()) {
        if (item->get_type() == ItemType::VIDEO) video_count += 1;
    }

    return video_count;
}
