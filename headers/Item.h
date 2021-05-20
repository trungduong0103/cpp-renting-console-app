#pragma once
#include <string>

enum ItemType { GAME = 0, VIDEO, DISC };

struct Item {
	//Attributes
	std::string id;
	std::string title;
	enum class RentalType { TwoDay, OneWeek } rental_type;
	unsigned int number_in_stock;
	float rental_fee;
	enum class RentalStatus { Available, Borrowed } rental_status;

	//Constructor
	Item(std::string id, std::string title, RentalType rental_type, unsigned int stock, float fee, RentalStatus status);

	//Methods
    inline std::string get_id() const { return id; }
	inline std::string get_title() const { return title; }
	inline RentalType get_rental_type() const { return rental_type; }
	inline unsigned int get_number_in_stock() const { return number_in_stock; }
    inline float get_rental_fee() const { return rental_fee; }
    inline RentalStatus get_rental_status() const { return rental_status; }
	inline void set_title(std::string const& new_title) { title = new_title; }
	inline void set_rental_type(RentalType const new_rental_type) { rental_type = new_rental_type; }
	inline void set_num_in_stock(unsigned int const new_num_in_stock) { number_in_stock = new_num_in_stock; }
    inline void set_rental_fee(float fee) { rental_fee = fee ; }
    inline void set_rental_status(RentalStatus new_rental_status) { rental_status = new_rental_status ; }
    inline void increase_num_in_stock(unsigned int value) { number_in_stock += value ; }
    inline void decrease_num_in_stock(unsigned int value) { number_in_stock -=(number_in_stock > value) ? value : number_in_stock; }
	inline bool is_available() const { return rental_status == RentalStatus::Available; }
	inline bool is_in_stock() const { return number_in_stock > 0; }
	friend std::ostream& operator<<(std::ostream& os, Item const& item);
	virtual std::string to_string_console() const = 0;
	virtual std::string to_string_file() const = 0;
	virtual ItemType get_type() const = 0;
};

struct GenredItem : public Item {
	//Attributes
	enum class Genre { Action, Horror, Drama, Comedy } genre;

	//Constructor
	GenredItem(std::string id, std::string title, RentalType rental_type, unsigned int stock, float fee, RentalStatus status, Genre genre);

	//Methods
    inline Genre get_genre() const { return genre; }
    inline void set_genre(Genre const new_genre) { genre = new_genre; }
	friend std::ostream& operator<<(std::ostream& os, GenredItem const& genredItem);
	std::string to_string_console() const override;
	std::string to_string_file() const override = 0;
};

struct Game : public Item {
	//Constructor
	using Item::Item;

	//Methods
	friend std::ostream& operator<<(std::ostream& os, Game const& game);
	std::string to_string_console() const override;
	std::string to_string_file() const override;
    ItemType get_type() const override;
};

struct VideoRecord : public GenredItem {
	//Constructor
	using GenredItem::GenredItem;

	//Methods
	friend std::ostream& operator<<(std::ostream& os, VideoRecord const& videoRecord);
	std::string to_string_console() const override;
    std::string to_string_file() const override;
    ItemType get_type() const override;
};

struct DVD : public GenredItem {
	//Constructor
	using GenredItem::GenredItem;

	//Methods
	friend std::ostream& operator<<(std::ostream& os, DVD const& dvd);
	std::string to_string_console() const override;
    std::string to_string_file() const override;
    ItemType get_type() const override;
};

