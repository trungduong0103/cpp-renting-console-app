#include "../headers/ItemHelpers.h"
#include<iostream>
#include <string>

Item::RentalType string_to_rental_type(const std::string &string_rental_type) {
    if (string_rental_type == "2-day") {
        return Item::RentalType::TwoDay;
    } else if (string_rental_type == "1-week") {
        return Item::RentalType::OneWeek;
    }

    std::cerr << "Must only be 2-day or 1-week" << std::endl;
    return Item::RentalType::TwoDay;
};

Item::RentalStatus string_to_rental_status(const std::string &string_rental_status) {
    if (string_rental_status == "available") {
        return Item::RentalStatus::Available;
    } else if (string_rental_status == "borrowed") {
        return Item::RentalStatus::Borrowed;
    }

    std::cerr << "Must only be available or borrowed!" << std::endl;
    return Item::RentalStatus::Borrowed;
}

GenredItem::Genre string_to_genre(const std::string &string_genre) {
//    bool result = string_genre == "Comedy";
//    std::cout << result << std::endl;
    if (string_genre == "Action") {
        return GenredItem::Genre::Action;
    } else if (string_genre == "Comedy") {
        return GenredItem::Genre::Comedy;
    } else if (string_genre == "Drama") {
        return GenredItem::Genre::Drama;
    } else if (string_genre == "Horror") {
        return GenredItem::Genre::Horror;
    } else {
        std::cerr << "Genre must either be 'Action', 'Comedy', 'Drama', or 'Horror'! Received: " << string_genre
                  << std::endl;
        return GenredItem::Genre::Action;
    }
}

void remove_whitespace(std::string &string) {
    int index = 0;
    for (char i : string) {
        if (i == 13) {
            string.erase(index, 1);
        }
        index++;
    }
}