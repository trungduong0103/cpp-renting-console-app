#pragma once

#include <string>
#include <iostream>

std::vector<std::string> get_customer_as_vector(const std::string &str, const std::string &delimiter);

bool correct_customer_info_length(std::string &line);

bool customer_phone_is_valid(const std::string &stock);