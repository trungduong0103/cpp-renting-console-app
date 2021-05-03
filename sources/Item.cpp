#include "../headers/Item.h"
#include <sstream>

//For items
Item::Item(std::string id, std::string title, RentalType rental_type, unsigned int stock, float fee, RentalStatus status) :
	id(id), title(title), rental_type(rental_type), number_in_stock(stock), rental_fee(fee), rental_status(status) {}

std::string Item::to_string() const {
	std::ostringstream oss;
	oss << "ID: " << id << ", ";
	oss << "Title: " << title << ", ";
	oss << "Rental type: " << (rental_type == Item::RentalType::TwoDay ? "Two day" : "One week") << ", ";
	oss << "Stock: " << number_in_stock << ", ";
	oss << "Fee: " << rental_fee << ", ";
	oss << "Rental status: " << (rental_status == Item::RentalStatus::Available ? "Available" : "Borrowed");

	return oss.str();
}

std::ostream& operator<<(std::ostream& os, Item const& item) {
	return os << item.to_string();
}

//For Genre Item
GenredItem::GenredItem(std::string id, std::string title, RentalType rental_type, unsigned int stock, float fee, RentalStatus status, Genre genre) :
	Item(id, title, rental_type, stock, fee, status), genre(genre) {}

std::string convert_genre(GenredItem::Genre const& genre) {
	switch (genre) {
	case GenredItem::Genre::Action:
		return "Action";
	case GenredItem::Genre::Comedy:
		return "Comedy";
	case GenredItem::Genre::Drama:
		return "Drama";
	case GenredItem::Genre::Horror:
		return "Horror";
	default:
		return "Unknown";
	}
}

std::string GenredItem::to_string() const {
	return { Item::to_string() + ", Genre: " + convert_genre(genre) };
}

std::ostream& operator<<(std::ostream& os, GenredItem const& genredItem) {
	return os << genredItem.to_string();
}

//For game
std::string Game::to_string() const {
	return { "Game: [" + Item::to_string() + "]" };
}

std::ostream& operator<<(std::ostream& os, Game const& game) {
	return os << game.to_string();
}

//For record
std::string VideoRecord::to_string() const {
	return { "Video record: [" + GenredItem::to_string() + "]" };
}

std::ostream& operator<<(std::ostream& os, VideoRecord const& videoRecord) {
	return os << videoRecord.to_string();
}

//For dvd
std::string DVD::to_string() const {
	return { "DVD: [" + GenredItem::to_string() + "]" };
}

std::ostream& operator<<(std::ostream& os, DVD const& dvd) {
	return os << dvd.to_string();
}