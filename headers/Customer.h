#pragma once
#include <string>
#include <vector>
#include "Item.h"

/*
	This components contains the logic for a customer and its state: Guest, Regular and VIP
	Here, the State design pattern is used to represent a customer's state using
	GuestState, RegularState, VIPState and all the returning and renting of item will be
	carried out using these state. After a customer gets promoted, the state of the account will also be
	changed (GuestState -> RegularState -> VIPState)
*/

//Enum for customer categories
enum class Category { guest, regular, vip };
class CustomerState;

//Customer class containing id, name, address, phone, item and state
class Customer {
    //Hold the id, name, address and phone
    std::string id;
    std::string name;
    std::string address;
    std::string phone;

    //Hold the number of renting items
    int number_of_rentals = 0;
    int number_of_videos = 0;

    //Hold the id of items
    std::vector<Item*> items;

    //Apply the State design pattern for Customer state
    //Each customer will have a state representing his/her privelegde
    CustomerState* state;

public:
    //Constructor and destructor
    Customer() = default;
    Customer(std::string  id, std::string  name, std::string  address, std::string  phone, int total_rentals, std::vector<Item*>  items, CustomerState* state);
    ~Customer();

    //Get methods
    inline int get_number_of_rentals() const { return number_of_rentals; }
    inline void increase_number_of_videos() { number_of_videos += 1; }
    inline int get_number_of_videos() const { return number_of_videos; }
    inline std::vector<Item*> get_items() const { return items; }
    inline std::string get_id() const { return id; }
    inline std::string get_name() const { return name; }
    inline std::string get_address() const { return address; }
    inline std::string get_phone() const { return phone; }
    Category get_state() const;

    //Set methods
    inline void set_name(std::string const& new_name) { name = new_name; }
    inline void set_address(std::string const& new_address) { address = new_address; }
    inline void set_phone(std::string const& new_phone) { phone = new_phone; }
    void change_state(CustomerState* new_state);

    //Method to promote a customer
    void promote();

    //Methods to borrow an item
    bool borrow(Item* item);
    bool return_item(Item* item);
    void increase_number_of_rentals();
    void decrease_number_of_rentals();
    void add_rental(Item* item);

    //Methods for printing the items
    friend std::ostream& operator<<(std::ostream& os, Customer const&);
    std::string to_string_file() const;
};

//State design pattern
//Each customer will have a State attribute
//Which can be change to reflects his/her status
class CustomerState {
public:
    //Methods to borrow an items
    virtual void borrow(Item* item) = 0;

    //Method to promote to next state
    //Guest -> Regular or Regula -> VIP
    virtual bool can_be_promoted() const = 0;
    virtual void promote() = 0;
    virtual std::string to_string() const = 0;

    //Set the Context of the state - which is the customer itself
    virtual void set_context(Customer* customer) = 0;

    //Get the current state (guest, regular or VIP)
    virtual Category get_state() = 0;
};

//Child state
//This CustomerState can be promote if user borrows and successfully returns 3 items
class ThreeItemPromotableCustomer : public CustomerState {
protected:
    //Holds the customer context and boolean checking if the customer has been promoted
    bool promoted;
    int number_of_video_rented{};
    Customer* context;
    //Protected constructor
    ThreeItemPromotableCustomer(Customer*, bool);

public:
    //Can only be promoted if 3 items are borrowed and returned successfully
    static const unsigned int minimum_promotion_rentals = 3;

    //Overidden methods to promote the state
    void promote() override = 0;
    bool can_be_promoted() const override;

    //Overriden method to set Customer context
    void set_context(Customer* customer) override;
};

//GuestState: Child of ThreeItemPromotableState
class GuestState : public ThreeItemPromotableCustomer {
    //Maximum number of borrow
    static const unsigned int MAX_RENT = 2;

public:
    //Constructors
    GuestState();
    GuestState(Customer*, bool);

    //Methods to borrow and promote customer
    void borrow(Item* item) override;
    void promote() override;

    //Method to get the State (Guest, VIP, Regular)
    Category get_state() override;

    std::string to_string() const override;
};

//RegularState: Child of ThreeItemPromotableState
class RegularState : public ThreeItemPromotableCustomer {
public:
    //Constructors
    RegularState();
    RegularState(Customer*, bool);

    //Methods to borrow and promote customer
    void borrow(Item* item) override;
    void promote() override;

    //Get the State enum (guest, regular, VIP)
    Category get_state() override;
    std::string to_string() const override;
};

//VIPState: Child of ThreeItemPromotableState
class VIPState : public ThreeItemPromotableCustomer {
private:
    int current_points = 0;
public:
    //Constructors
    VIPState();
    VIPState(Customer*, bool);

    //Methods to borrow and promote customer
    void borrow(Item* item) override;
    void promote() override;

    //Get the State enum (guest, regular, VIP) and set context
    void set_context(Customer* customer) override;
    Category get_state() override;
    std::string to_string() const override;
};