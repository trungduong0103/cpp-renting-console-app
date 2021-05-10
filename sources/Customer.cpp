#include <iostream>
#include <utility>
#include "../headers/Customer.h"
#include "../headers/Item.h"

//Promotable 
bool ThreeItemPromotableCustomer::can_be_promoted() const {
	return context->get_number_of_rentals() - ThreeItemPromotableCustomer::minimum_promotion_rentals * promoted >= 3;
}

ThreeItemPromotableCustomer::ThreeItemPromotableCustomer(Customer* customer, bool promoted) : context(customer), promoted(promoted) {}

void ThreeItemPromotableCustomer::set_context(Customer* customer) {
	context = customer;
}

//Guest state
GuestState::GuestState() : ThreeItemPromotableCustomer(nullptr, false) {};
GuestState::GuestState(Customer* customer, bool promoted) : ThreeItemPromotableCustomer(customer, promoted) {}
Category GuestState::get_state() { return Category::guest; }

void GuestState::promote() {
	//Can not be promoted -> Throw error
	if (!can_be_promoted()) {
		//Throw exception
		std::cerr << "Illegible to promotion to regular" << std::endl;
		return;
	}

	//Promote to Regular customer
	context->change_state(new RegularState{ nullptr, true });
}

void GuestState::borrow(Item& item) {
	//Check the number of item in the list
	if (context->get_items().size() >= 2) {
		std::cerr << "Can not rent more items (number of currently renting items has exceeded 2)" << std::endl;
		return;
	}

	//Check the type of item
	if (item.rental_type == Item::RentalType::TwoDay) {
		std::cerr << "Can not borrow two-day item (for guest account)" << std::endl;
		return;
	}

	//Borrow items
	context->add_rental(item);
	context->increase_number_of_rentals();
	std::cout << "Item rented successfully" << std::endl;
}

//Regular state
RegularState::RegularState() : ThreeItemPromotableCustomer(nullptr, false) {}
RegularState::RegularState(Customer* customer, bool promoted) : ThreeItemPromotableCustomer(customer, promoted) {}
Category RegularState::get_state() { return Category::regular; }

void RegularState::promote() {
	if (!can_be_promoted()) {
		//Throw exception
		std::cerr << "Illegible to promotion to vip" << std::endl;
		return;
	}

	context->change_state(new VIPState{ nullptr, true });
}

void RegularState::borrow(Item& item) {
	context->add_rental(item);
	context->increase_number_of_rentals();
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
	//Do nothing
	std::cerr << "VIP is the highest-ranked account" << std::endl;
	return;
}

void VIPState::borrow(Item& item) {
	context->increase_number_of_rentals();
	context->add_rental(item);

	std::cout << current_points << std::endl;

	if (current_points >= 100) {
		current_points -= 100;
		std::cout << "Item rented for free (vip account only)" << std::endl;
	}
	else {
		current_points += 10;
		std::cout << "Item rented successfully (add 10 points to vip account)" << std::endl;
	}
}

void VIPState::set_context(Customer* customer) {
	ThreeItemPromotableCustomer::set_context(customer);
	current_points = customer->get_number_of_rentals() * 10;
}

Customer::Customer(std::string id, std::string name, std::string address, std::string phone,
         int total_rentals, std::vector<std::string> items, CustomerState *state)
	: id(std::move(id)), name(std::move(name)), address(std::move(address)), phone(std::move(phone)), number_of_rentals(total_rentals), items(std::move(items)) , state(state) {
	state->set_context(this);
}

Customer::~Customer() {
	if (state != nullptr) {
		delete state;
	}
}

void Customer::change_state(CustomerState* new_state) {
	if (state != nullptr) {
		delete state;
	}

	state = new_state;
	state->set_context(this);
}

void Customer::promote() {
	if (state == nullptr) {
		std::cerr << "Invalid state" << std::endl;
		return;
	}
	state->promote();
}

Category Customer::get_state() const {
	return state->get_state();
}

void Customer::borrow(Item& item) {
	state->borrow(item);
}

void Customer::increase_number_of_rentals() {
	number_of_rentals += 1;
}

void Customer::add_rental(Item& item) {
	items.push_back(item.id);
}

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