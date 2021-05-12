#include "../headers/ItemHelpers.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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

bool id_number_is_not_numeric(const std::string &id_number) {
    const std::string numerics = "0123456789";
    return id_number.find_first_not_of(numerics) != std::string::npos;
}

bool item_id_is_valid(const std::string &id, const std::vector<Item *> &mockItems) {
    // format: Ixxx-yyyy
    std::vector<std::string> id_pool;
    std::string default_error = "Item ID is incorrect format ";
    // id must be unique
    for (Item *item : mockItems) {
        if (item != nullptr && item->get_id() == id) {
            std::cerr << default_error << "(already exists)!" << std::endl;
            return false;
        }
    }
    // id length of item must be 9
    if (id.length() != 9) {
        std::cerr << default_error << "(wrong length)!" << std::endl;
        return false;
    }

    char first_letter = id[0];
    const std::string id_number = id.substr(1, 3);
    char hyphen = id[4];
    const std::string year = id.substr(5, 4);

    // first letter must be "I" (‘I’ is the capital letter I).
    if (first_letter != 'I') {
        std::cerr << default_error << "(must begin with 'I')!" << std::endl;
        return false;
    }

    // ‘xxx’ is a unique code of 3 digits (e.g. 123)
    if (id_number_is_not_numeric(id_number)) {
        std::cerr << default_error << "(ID number in Item ID must be numerics)!" << std::endl;
        return false;
    }

    // ‘-‘ is a single hyphen character
    if (hyphen != '-') {
        std::cerr << default_error << "(hyphen is missing)!" << std::endl;
        return false;
    }

    // `yyyy` is the year the item was published (e.g. 1980)
    // first year ever made was in 1888, current year is 2021
    const unsigned int MAX_YEAR = 2021, MIN_YEAR = 1888;
    try {
        const unsigned int int_year = std::stoi(year, nullptr, 10);
        if (int_year < MIN_YEAR || int_year > MAX_YEAR) {
            std::cerr << default_error << "(year must be between 1888 and 2021)!" << std::endl;
            return false;
        }
    } catch (const std::invalid_argument &e) {
        std::cerr << default_error << "(year is in invalid format)!" << std::endl;
        return false;
    }

    return true;
}

bool item_type_and_genre_is_valid(
        const std::string &type,
        const std::string &genre,
        std::vector<std::string>::size_type item_info_length
) {
    const std::string &default_error = "Item Type/Genre is in invalid ";
    const std::vector<std::string> allowed_games = {"Game", "game"};
    const std::vector<std::string> allowed_videos_dvds = {"Record", "record", "DVD", "dvd"};
    const std::vector<std::string> allowed_genres = {
            "Action",
            "action",
            "Horror",
            "horror",
            "Drama",
            "drama",
            "Comedy",
            "comedy"
    };

    bool is_game = std::count(allowed_games.begin(), allowed_games.end(), type) != 0;
    bool is_video_or_dvd = std::count(allowed_videos_dvds.begin(), allowed_videos_dvds.end(), type) != 0;
    bool correct_genre = std::count(allowed_genres.begin(), allowed_genres.end(), genre);

    if (item_info_length == 6) {
        if (!is_game) {
            std::cout << default_error << "(item type is invalid), received: " << type << std::endl;
            return false;
        }
        return true;
    } else if (item_info_length == 7) {
        // Flow
        // if not dvd or video => false
        // if either dvd or video but genre is not of action, horror, comedy, and drama => false
        // otherwise true
        if (is_game) {
            std::cout << default_error
                      << "(received game but game has genre/or other data at the end of line)!"
                      << std::endl;
            return false;
        }
        if (!is_video_or_dvd) {
            std::cout << default_error << "(item type is invalid), received: " << type << std::endl;
            return false;
        }
        if (!correct_genre) {
            std::cout << default_error << "(item genre is invalid), received: " << genre << std::endl;
            return false;
        }

        return true;
    } else {
        std::cerr << default_error << "(item info length is invalid)!" << std::endl;
        return false;
    }

}

bool item_loan_type_is_valid(const std::string &loan_type) {
    if (loan_type != "1-week" && loan_type != "2-day") {
        std::cerr << "Item loan type is invalid, received: " << loan_type << std::endl;
        return false;
    }
    return true;
}


bool has_digits_or_comma(const std::string &str) {
    for (char c : str) {
        if (c < 48 || c > 57) {
            return true;
        }
    }

    return false;
}

bool item_stock_is_valid(const std::string &stock) {
    const std::string default_error = "Item stock is invalid";
    try {
        const int int_stock = std::stoi(stock, nullptr, 10);
        std::cout << "stock: " << int_stock << std::endl;
        if (has_digits_or_comma(stock)) {
            std::cerr << default_error << ", received: " << stock << std::endl;
            return false;
        }
        if (int_stock < 0) {
            std::cerr << default_error << ", stock must be bigger than 0, received: " << stock << std::endl;
            return false;
        }
        return true;
    } catch (std::invalid_argument &e) {
        std::cerr << default_error << ", received: " << stock << std::endl;
        return false;
    }
}