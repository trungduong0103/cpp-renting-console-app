#pragma once

#include <string>
#include <iostream>
#include <vector>

std::vector<std::string> get_customer_as_vector(const std::string &str, const std::string &delimiter);

bool correct_customer_info_length(std::string &line);

bool customer_phone_is_valid(const std::string &stock);

bool customer_id_is_valid_menu(const std::string &id);