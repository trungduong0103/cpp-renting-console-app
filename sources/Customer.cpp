#include <iostream>
#include "../headers/Customer.h"

//Promotable
bool ThreeItemPromotableCustomer::can_be_promoted() const {
    //User can only be promoted if the number of rentals >= 3 and has not been promoted yet
    return context->get_number_of_rentals() - ThreeItemPromotableCustomer::minimum_promotion_rentals * promoted >= 3;
}

//Constructor for ThreeItemPromotableCustomer
ThreeItemPromotableCustomer::ThreeItemPromotableCustomer(Customer* customer, bool promoted) : context(customer), promoted(promoted) {}

//Set the Customer context (State design pattern)
void ThreeItemPromotableCustomer::set_context(Customer* customer) {
    context = customer;
}

//Guest state
//Constructor with no arguments
GuestState::GuestState() : ThreeItemPromotableCustomer(nullptr, false) {};

//Constructor with context and promoted
GuestState::GuestState(Customer* customer, bool promoted) : ThreeItemPromotableCustomer(customer, promoted) {}

//Get State enum
Category GuestState::get_state() { return Category::guest; }

//Method to promote customer
void GuestState::promote() {
    //Can not be promoted -> Throw error
    if (!can_be_promoted()) {
        //Throw exception
        std::cerr << "Illegible to promotion to regular" << std::endl;
        return;
    }

    //Promote to Regular customer by changing the state of
    //Customer context (State design pattern)
    context->change_state(new RegularState{ nullptr, true });
}

//Method to borrow an item
void GuestState::borrow(Item& item) {
    //Check if item is in stock
    if (!item.is_in_stock()) {
        std::cerr << "Item is currently out of stock, can not be borrowed" << std::endl;
        return;
    }

    //Check the number of item in the list
    //Guest can not borrow more than 2 items
    if (context->get_items().size() >= 2) {
        std::cerr << "Can not rent more items (number of currently renting items has exceeded 2)" << std::endl;
        return;
    }

    //Check the type of item
    //Guest can not borrow TwoDay items
    if (item.get_rental_type() == Item::RentalType::TwoDay) {
        std::cerr << "Can not borrow two-day item (for guest account)" << std::endl;
        return;
    }

    //Borrow items
    context->add_rental(item);
    context->increase_number_of_rentals();

    //Decrease number of stock
    item.decrease_num_in_stock(1);
    item.set_rental_status(Item::RentalStatus::Borrowed);

    //Display successfuly message
    std::cout << "Item rented successfully" << std::endl;
}

//Regular state
//Constructor with no argument
RegularState::RegularState() : ThreeItemPromotableCustomer(nullptr, false) {}

//Constructor with Customer state and promoted boolean
RegularState::RegularState(Customer* customer, bool promoted) : ThreeItemPromotableCustomer(customer, promoted) {}

//Get Regular state enum
Category RegularState::get_state() { return Category::regular; }

//Promote the State
void RegularState::promote() {
    if (!can_be_promoted()) {
        //Throw exception
        std::cerr << "Illegible to promotion to vip" << std::endl;
        return;
    }

    //Set the State attribute of Customer context to VIP (State design pattern)
    context->change_state(new VIPState{ nullptr, true });
}

//Method to borrow an item
void RegularState::borrow(Item& item) {
    //Check if the item is already borrowed
    if (!item.is_in_stock()) {
        std::cerr << "Item is currently out of stock, can not be borrowed" << std::endl;
        return;
    }

    //There are no restriction on a regular account
    //Just borrow the item
    context->add_rental(item);
    context->increase_number_of_rentals();

    //Decrease number of stock
    item.decrease_num_in_stock(1);
    item.set_rental_status(Item::RentalStatus::Borrowed);

    //Display successful message
    std::cout << "Item rented successfully" << std::endl;
}

//VIP state
VIPState::VIPState() : ThreeItemPromotableCustomer(nullptr, false) {}
VIPState::VIPState(Customer* customer, bool promoted) : ThreeItemPromotableCustomer(customer, promoted) {
    //Initially set up customer points
    current_points = customer->get_number_of_rentals() * 10;
}
Category VIPState::get_state() { return Category::vip; }

void VIPState::promote() {
    //Display error message
    std::cerr << "VIP is the highest-ranked account" << std::endl;
    return;
}

void VIPState::borrow(Item& item) {
    //Check if the item is borrowed or not
    if (!item.is_in_stock()) {
        std::cerr << "Item is currently out of stock, can not be borrowed" << std::endl;
        return;
    }

    //Then borrow the item
    //VIP account has no restriction
    context->increase_number_of_rentals();
    context->add_rental(item);

    //Decrease number of stock
    item.decrease_num_in_stock(1);
    item.set_rental_status(Item::RentalStatus::Borrowed);

    //If current point is over 100
    //Item will be rented for free
    if (current_points >= 100) {
        current_points -= 100;
        std::cout << "Item rented for free (vip account only)" << std::endl;
    }
    else {
        current_points += 10;
        std::cout << "Item rented successfully (add 10 points to vip account)" << std::endl;
    }
}

//Set context of VIPState account
void VIPState::set_context(Customer* customer) {
    ThreeItemPromotableCustomer::set_context(customer);
    //We need to get the current points by 10 times the number of items he/she has borrowed
    current_points = customer->get_number_of_rentals() * 10;
}

//Customer constructor
Customer::Customer(std::string const& id, std::string const& name, std::string const& address, std::string const& phone, int total_rentals, std::vector<std::string> const& items, CustomerState* state)
        : id(id), name(name), address(address), phone(phone), number_of_rentals(total_rentals), items(items) , state(state) {
    state->set_context(this);
}

//Destrcutor
Customer::~Customer() {
    //Delete the state
    if (state != nullptr) {
        delete state;
    }
}

//Change the customer state
//Guest -> Regular or Regular -> VIP
void Customer::change_state(CustomerState* new_state) {
    //Delete the current state
    if (state != nullptr) {
        delete state;
    }

    //Set the new state
    state = new_state;

    //Also set the context of the state to this (customer)
    state->set_context(this);
}

//Promote the customer by calling
//The promote method on its state
void Customer::promote() {
    if (state == nullptr) {
        std::cerr << "Invalid state" << std::endl;
        return;
    }
    state->promote();
}

//Get State enum of the customer
Category Customer::get_state() const {
    return state->get_state();
}

//Customer borrowing an item
//By calling the method on item
void Customer::borrow(Item& item) {
    state->borrow(item);
}

//Increase the number of rental items
void Customer::increase_number_of_rentals() {
    number_of_rentals += 1;
}

//Add item id to rental list
void Customer::add_rental(Item& item) {
    items.push_back(item.id);
}

//Display the customer
std::ostream& operator<<(std::ostream& os, Customer const& customer) {
    std::string state;
    switch ((customer.state)->get_state()) {
        case Category::guest:
            state = "guest";
            break;
        case Category::regular:
            state = "regular";
            break;
        case Category::vip:
            state = "vip";
            break;
    }
    os << "Id: " << customer.id << ", Name: " << customer.name << ", State: " << state;
    return os;
}