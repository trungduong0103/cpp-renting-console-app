#include "../headers/ItemHelpers.h"

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