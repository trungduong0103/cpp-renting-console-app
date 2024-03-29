#include "../headers/Item.h"
#include "../headers/ItemHelpers.h"
#include <iostream>
#include <sstream>

//For items
Item::Item(std::string id, std::string title, RentalType rental_type, unsigned int stock, float fee,
           RentalStatus status) :
        id(std::move(id)), title(std::move(title)), rental_type(rental_type), number_in_stock(stock), rental_fee(fee),
        rental_status(status) {}

std::string Item::to_string_console() const {
    std::ostringstream oss;
    oss << "ID: " << id << ", ";
    oss << "Title: " << title << ", ";
    oss << "Rental type: " << (rental_type == Item::RentalType::TwoDay ? "Two day" : "One week") << ", ";
    oss << "Stock: " << number_in_stock << ", ";
    oss << "Fee: " << rental_fee << ", ";
    oss << "Rental status: " << (rental_status == Item::RentalStatus::Available ? "Available" : "Borrowed");

    return oss.str();
}

std::ostream &operator<<(std::ostream &os, Item const &item) {
    return os << item.to_string_console();
}

//For Genred Item
GenredItem::GenredItem(std::string id, std::string title, RentalType rental_type, unsigned int stock, float fee,
                       RentalStatus status, Genre genre) :
        Item(std::move(id), std::move(title), rental_type, stock, fee, status), genre(genre) {}

std::string GenredItem::to_string_console() const {
    return {Item::to_string_console() + ", Genre: " + genre_to_string(genre)};
}

std::ostream &operator<<(std::ostream &os, GenredItem const &genredItem) {
    return os << genredItem.to_string_console();
}

//For game
std::string Game::to_string_console() const {
    return {"Game: [" + Item::to_string_console() + "]"};
}

std::string Game::to_string_file() const {
    return {
            this->get_id() + "," +
            this->get_title() + "," +
            "Game" + "," +
            rental_type_to_string(this->rental_type) + "," +
            std::to_string(this->get_number_in_stock()) + "," +
            std::to_string(this->get_rental_fee())
    };
}

ItemType Game::get_type() const { return GAME; }

std::ostream &operator<<(std::ostream &os, Game const &game) {
    return os << game.to_string_console();
}

//For record
std::string VideoRecord::to_string_console() const {
    return {"Video record: [" + GenredItem::to_string_console() + "]"};
}

std::string VideoRecord::to_string_file() const {
    return {
            this->get_id() + "," +
            this->get_title() + "," +
            "Record" + "," +
            rental_type_to_string(this->rental_type) + "," +
            std::to_string(this->get_number_in_stock()) + "," +
            std::to_string(this->get_rental_fee()) + "," +
            genre_to_string(this->get_genre())
    };
}

ItemType VideoRecord::get_type() const { return VIDEO; }

std::ostream &operator<<(std::ostream &os, VideoRecord const &videoRecord) {
    return os << videoRecord.to_string_console();
}

//For dvd
std::string DVD::to_string_console() const {
    return {"DVD: [" + GenredItem::to_string_console() + "]"};
}

std::string DVD::to_string_file() const {
    return {
            this->get_id() + "," +
            this->get_title() + "," +
            "DVD" + "," +
            rental_type_to_string(this->rental_type) + "," +
            std::to_string(this->get_number_in_stock()) + "," +
            std::to_string(this->get_rental_fee()) + "," +
            genre_to_string(this->get_genre())
    };
}

ItemType DVD::get_type() const { return DISC; }

std::ostream &operator<<(std::ostream &os, DVD const &dvd) {
    return os << dvd.to_string_console();
}