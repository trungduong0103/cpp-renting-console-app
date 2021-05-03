#pragma once
#include <string>
#include <vector>
#include "Item.h"

enum class Category { guest, regular, vip };
class CustomerState;

class Customer {
	std::string id;
	std::string name;
	std::string address;
	std::string phone;
	int number_of_rentals;
	std::vector<std::string> items;
	CustomerState* state;

public: 
	Customer(std::string const& id, std::string const& name, std::string const& address, std::string const& phone, int total_rentals, std::vector<std::string> const& items, CustomerState* state);
	~Customer();
	inline int get_number_of_rentals() { return number_of_rentals; }
	inline std::vector<std::string> get_items() const { return items; }
	inline std::string get_id() const { return id; }
	inline std::string get_name() const { return name; }
	inline std::string get_address() const { return address; }
	inline std::string get_phone() const { return phone; }
	inline void set_name(std::string const& new_name) { name = new_name; }
	inline void set_address(std::string const& new_address) { address = new_address; }
	inline void set_phone(std::string const& new_phone) { phone = new_phone; }
	Category get_state() const;
	void change_state(CustomerState* new_state);
	void promote();
	void borrow(Item& item);
	void increase_number_of_rentals();
	void add_rental(Item& item);
	friend std::ostream& operator<<(std::ostream& os, Customer const&);
};

class CustomerState {
public:
	virtual void borrow(Item& item) = 0;
	virtual bool can_be_promoted() const = 0;
	virtual void promote() = 0;
	virtual void set_context(Customer* customer) = 0;
	virtual Category get_state() = 0;
};

class ThreeItemPromotableCustomer : public CustomerState {
protected:
	bool promoted;
	Customer* context;
	ThreeItemPromotableCustomer(Customer*, bool);

public:
	static const unsigned int minimum_promotion_rentals = 3;
	virtual void promote() override = 0;
	bool can_be_promoted() const override;
	virtual void set_context(Customer* customer) override;
};

class GuestState : public ThreeItemPromotableCustomer {
	//Maximum number of borrow
	static const unsigned int MAX_RENT = 2;

public:
	//Constructors
	GuestState();
	GuestState(Customer*, bool);

	//Methods
	void borrow(Item& item) override;
	void promote() override;
	Category get_state() override;
};

class RegularState : public ThreeItemPromotableCustomer {
public:
	//Constructors
	RegularState();
	RegularState(Customer*, bool);

	//Methods
	void borrow(Item& item) override;
	void promote() override;
	Category get_state() override;
};

class VIPState : public ThreeItemPromotableCustomer {
private:
	int current_points = 0;
public:
	//Constructors
	VIPState();
	VIPState(Customer*, bool);

	//Methods
	void borrow(Item& item) override;
	void promote() override;
	void set_context(Customer* customer);
	Category get_state() override;
};