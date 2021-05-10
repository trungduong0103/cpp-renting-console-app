#pragma once
#include "Item.h"
#include <iostream>
#include <string>

Item::RentalType string_to_rental_type(const std::string &string_rental_type);
Item::RentalStatus string_to_rental_status(const std::string &string_rental_status);