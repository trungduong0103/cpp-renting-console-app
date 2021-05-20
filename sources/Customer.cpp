#include <iostream>
#include <utility>
#include "../headers/Customer.h"
#include "../headers/CustomerHelpers.h"

//Promotable
bool ThreeItemPromotableCustomer::can_be_promoted() const {
    //User can only be promoted if the number of rentals >= 3 and has not been promoted yet
    std::cout << context->get_number_of_videos() << std::endl;
    return context->get_number_of_videos() - ThreeItemPromotableCustomer::minimum_promotion_rentals * promoted >= 3;
}

//Constructor for ThreeItemPromotableCustomer
ThreeItemPromotableCustomer::ThreeItemPromotableCustomer(Customer *customer, bool promoted) : context(customer),
                                                                                              promoted(promoted) {}

//Set the Customer context (State design pattern)
void ThreeItemPromotableCustomer::set_context(Customer *customer) {
    context = customer;
}

//Get number of videos rented
int ThreeItemPromotableCustomer::get_number_of_videos_rented() const { return number_of_video_rented; }

void ThreeItemPromotableCustomer::increase_number_of_videos_rented() { number_of_video_rented += 1; }

//Guest state
//Constructor with no arguments
GuestState::GuestState() : ThreeItemPromotableCustomer(nullptr, false) {};

//Constructor with context and promoted
GuestState::GuestState(Customer *customer, bool promoted) : ThreeItemPromotableCustomer(customer, promoted) {}

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
    context->change_state(new RegularState{nullptr, true});
    std::cout << "Customer promoted to Regular customer" << std::endl;
}

//Method to borrow an item
void GuestState::borrow(Item *item) {
    //Check if item is in stock
    if (!item->is_in_stock()) {
        std::cerr << "Item is currently out of stock, can not be borrowed" << std::endl;
        return;
    }

    //Check the number of item in the list
    //Guest can not borrow more than 2 items
    if (get_number_of_videos(context) >= 2) {
        std::cerr << "Can not rent more items (number of currently renting items has exceeded 2)" << std::endl;
        return;
    }

    //Check the type of item
    //Guest can not borrow TwoDay items
    if (item->get_rental_type() == Item::RentalType::TwoDay) {
        std::cerr << "Can not borrow two-day item (for guest account)" << std::endl;
        return;
    }

    //Borrow items
    context->add_rental(item);
    context->increase_number_of_rentals();

    //Decrease number of stock
    item->decrease_num_in_stock(1);
    item->set_rental_status(Item::RentalStatus::Borrowed);

    //Display successfuly message
    std::cout << "Item rented successfully" << std::endl;
}

void GuestState::return_item(Item *item) {
    if (item->get_type() == VIDEO) {
        increase_number_of_videos_rented();
    }
}

//Regular state
//Constructor with no argument
RegularState::RegularState() : ThreeItemPromotableCustomer(nullptr, false) {}

//Constructor with Customer state and promoted boolean
RegularState::RegularState(Customer *customer, bool promoted) : ThreeItemPromotableCustomer(customer, promoted) {}

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
    context->change_state(new VIPState{context, true});
    std::cout << "Customer promoted to VIP customer" << std::endl;
}

//Method to borrow an item
void RegularState::borrow(Item *item) {
    //Check if the item is already borrowed
    if (!item->is_in_stock()) {
        std::cerr << "Item is currently out of stock, can not be borrowed" << std::endl;
        return;
    }

    //There are no restriction on a regular account
    //Just borrow the item
    context->add_rental(item);
    context->increase_number_of_rentals();

    //Decrease number of stock
    item->decrease_num_in_stock(1);
    item->set_rental_status(Item::RentalStatus::Borrowed);

    //Display successful message
    std::cout << "Item rented successfully" << std::endl;
}

void RegularState::return_item(Item *item) {

}

//VIP state
VIPState::VIPState() : ThreeItemPromotableCustomer(nullptr, false) {}

VIPState::VIPState(Customer *customer, bool promoted) : ThreeItemPromotableCustomer(customer, promoted) {
    //Initially set up customer points
    current_points = customer->get_number_of_rentals() * 10;
}

Category VIPState::get_state() { return Category::vip; }

void VIPState::promote() {
    //Display error message
    std::cerr << "VIP is the highest-ranked account" << std::endl;
    return;
}

void VIPState::borrow(Item *item) {
    //Check if the item is borrowed or not
    if (!item->is_in_stock()) {
        std::cerr << "Item is currently out of stock, can not be borrowed" << std::endl;
        return;
    }

    //Then borrow the item
    //VIP account has no restriction
    context->increase_number_of_rentals();
    context->add_rental(item);

    //Decrease number of stock
    item->decrease_num_in_stock(1);
    item->set_rental_status(Item::RentalStatus::Borrowed);


    //If current point is over 100
    //Item will be rented for free
    if (current_points >= 100) {
        current_points -= 100;
        std::cout << "Item rented for free (vip account only)" << std::endl;
    } else {
        current_points += 10;
        std::cout << "Item rented successfully (add 10 points to vip account)" << std::endl;
    }
}

void VIPState::return_item(Item *item) {

}

//Set context of VIPState account
void VIPState::set_context(Customer *customer) {
    ThreeItemPromotableCustomer::set_context(customer);
    //We need to get the current points by 10 times the number of items he/she has borrowed
    current_points = customer->get_number_of_rentals() * 10;
}

//Customer constructor
Customer::Customer(std::string id, std::string name, std::string address, std::string phone, int total_rentals,
                   std::vector<Item *> items, CustomerState *state)
        : id(std::move(id)), name(std::move(name)), address(std::move(address)), phone(std::move(phone)),
          number_of_rentals(total_rentals), items(std::move(items)), state(state) {
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
void Customer::change_state(CustomerState *new_state) {
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
bool Customer::borrow(Item *item) {
    //Check if item is already borrowed
    for (int i = 0; i != items.size(); ++i) {
        if (items[i]->get_id() == item->get_id()) {
            std::cerr << "Item is already borrowed by customer" << std::endl;
            return false;
        }
    }

    //Then borrowed item
    state->borrow(item);
    return true;
}

//Customer returning an item
bool Customer::return_item(Item *item) {
    //Check if item id in rental
    int position = -1;
    for (int i = 0; i != items.size(); ++i) {
        if (items[i]->get_id() == item->get_id()) {
            position = i;
            break;
        }
    }

    //Display message if not exist
    if (position == -1) {
        std::cerr << "Item is not borrowed by user" << std::endl;
        return false;
    }

    //Increase item count
    item->increase_num_in_stock(1);

    //Remove from the borrow list
    items.erase(items.begin() + position);

    //Reduce user's item count
    decrease_number_of_rentals();

    //State return item
    if (item->get_type() == VIDEO) {
        increase_number_of_videos();
    }

    return true;
}

//Increase and decrease the number of rental items
void Customer::increase_number_of_rentals() {
    number_of_rentals += 1;
}

void Customer::decrease_number_of_rentals() {
    number_of_rentals -= 1;
}

//Add item id to rental list
void Customer::add_rental(Item *item) {
    items.push_back(item);
}

//Display the customer
std::ostream &operator<<(std::ostream &os, Customer const &customer) {
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