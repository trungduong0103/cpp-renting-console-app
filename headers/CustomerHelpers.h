#pragma once

#include <string>
#include <iostream>
#include <vector>

std::vector<std::string> get_customer_as_vector(const std::string &str);

bool correct_customer_info_length(const std::string &line);

bool customer_phone_is_valid(const std::string &stock);

bool customer_id_is_valid(const std::string &id, bool from_menu = true);

bool customer_name_is_valid(const std::string &id);

bool valid_customer_data(const std::string &id);