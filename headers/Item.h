#pragma once
#include <string>

/*
	This component contains the logic for items and
	its children: namely GenredItem, Game, Video Record
	and DVD
*/

//Item type to differentiate between types of items
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

	//Getter methods for attributes
    inline std::string get_id() const { return id; }
	inline std::string get_title() const { return title; }
	inline RentalType get_rental_type() const { return rental_type; }
	inline unsigned int get_number_in_stock() const { return number_in_stock; }
    inline float get_rental_fee() const { return rental_fee; }
    inline RentalStatus get_rental_status() const { return rental_status; }
    virtual ItemType get_type() const = 0;

    //Setter methods for attributes
	inline void set_title(std::string const& new_title) { title = new_title; }
	inline void set_rental_type(RentalType const new_rental_type) { rental_type = new_rental_type; }
	inline void set_num_in_stock(unsigned int const new_num_in_stock) { number_in_stock = new_num_in_stock; }
    inline void set_rental_fee(float fee) { rental_fee = fee ; }
    inline void set_rental_status(RentalStatus new_rental_status) { rental_status = new_rental_status ; }

    //Methods to increase or decrease number of stocks
    inline void increase_num_in_stock(unsigned int value) { number_in_stock += value ; }
    inline void decrease_num_in_stock(unsigned int value) { number_in_stock -=(number_in_stock > value) ? value : number_in_stock; }

    //Check if item is available and in stock
	inline bool is_available() const { return rental_status == RentalStatus::Available; }
	inline bool is_in_stock() const { return number_in_stock > 0; }

	//Print item to sstream
	friend std::ostream& operator<<(std::ostream& os, Item const& item);

	//To string for printing to console
	virtual std::string to_string_console() const = 0;

	//To string for print to file
	virtual std::string to_string_file() const = 0;
};

//Genre Items are items which genre
//This is the parent class of Video Record and DVD
struct GenredItem : public Item {
	//Attributes
	enum class Genre { Action, Horror, Drama, Comedy } genre;

	//Constructor
	GenredItem(std::string id, std::string title, RentalType rental_type, unsigned int stock, float fee, RentalStatus status, Genre genre);

	//Setter and getter for genre
    inline Genre get_genre() const { return genre; }
    inline void set_genre(Genre const new_genre) { genre = new_genre; }

    //User for printing genreditem
	friend std::ostream& operator<<(std::ostream& os, GenredItem const& genredItem);

    //Genred Item to string (for console display)
	std::string to_string_console() const override;

    //Genred Item to string (for writing to file)
	std::string to_string_file() const override = 0;
};

//Game is a child of item
struct Game : public Item {
	//Constructor
	using Item::Item;

    //Use for printing game
	friend std::ostream& operator<<(std::ostream& os, Game const& game);

    //Genred Item to string (for console display)
	std::string to_string_console() const override;

    //Genred Item to string (for writing to file)
	std::string to_string_file() const override;

	//Get type of item as enum
    ItemType get_type() const override;
};

//Video record is a child of GenredItem
struct VideoRecord : public GenredItem {
	//Constructor
	using GenredItem::GenredItem;

    //Use for printing Video Record
	friend std::ostream& operator<<(std::ostream& os, VideoRecord const& videoRecord);

    //Video to string (for console display)
	std::string to_string_console() const override;

    //Video to string (for writing to file)
    std::string to_string_file() const override;

    //Get type as enum
    ItemType get_type() const override;
};

//DVD is a child of GenredItem
struct DVD : public GenredItem {
	//Constructor
	using GenredItem::GenredItem;

    //Use for printing DVD
	friend std::ostream& operator<<(std::ostream& os, DVD const& dvd);

    //DVD to string (for console display)
	std::string to_string_console() const override;

    //DVD to string (for writing to file)
    std::string to_string_file() const override;

    //Get type as enum
    ItemType get_type() const override;
};

