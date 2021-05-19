#pragma once
#pragma once

#include "Item.h"
#include <iostream>
#include <string>
#include <vector>

Item::RentalType string_to_rental_type(const std::string &string_rental_type);

std::string rental_type_to_string(Item::RentalType rental_type);

Item::RentalStatus string_to_rental_status(const std::string &string_rental_status);

std::string rental_status_to_string(Item::RentalStatus rental_status);

GenredItem::Genre string_to_genre(const std::string &string_genre);

std::string genre_to_string(GenredItem::Genre genre);

void remove_whitespace(std::string &string);

bool correct_info_length(const std::string &line);

bool item_id_is_valid(const std::string &id, const std::vector<Item *> &mockItems = {}, bool format_only = true);

bool item_type_and_genre_is_valid(
        const std::string &type,
        const std::string &genre,
        std::vector<std::string>::size_type item_info_length
);

bool item_loan_type_is_valid(const std::string &loan_type);

bool item_stock_is_valid(const std::string &stock);

bool item_price_is_valid(const std::string &price);

bool valid_item_data(
        const std::string &id,
        const std::vector<Item *> &mockItems,
        const std::string &type,
        const std::string &genre,
        std::vector<std::string>::size_type item_info_length,
        const std::string &loan_type,
        const std::string &stock,
        const std::string &price
);

//bool item_exists_with_id(const std::string &id, const std::vector<Item *> &items);