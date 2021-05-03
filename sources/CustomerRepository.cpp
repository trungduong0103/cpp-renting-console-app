#include "../headers/CustomerRespository.h"
#include "../headers/Customer.h"
#include <algorithm>

//Modification intents
ModificationIntent::ModificationIntent(Customer* customer) : customer(customer) {}

NameModificationIntent::NameModificationIntent(std::string const& name) : name(name) {}
void NameModificationIntent::modify() {
	customer->set_name(name);
}

PhoneModificationIntent::PhoneModificationIntent(std::string const& phone) : phone(phone) {}
void PhoneModificationIntent::modify() {
	customer->set_phone(phone);
}

AddressModificationIntent::AddressModificationIntent(std::string const& address) : address(address) {}
void AddressModificationIntent::modify() {
	customer->set_address(address);
}

//Repository
InMemoryCustomerRepository::InMemoryCustomerRepository(std::vector<Customer*> const& customers) : customers(customers) {}
void InMemoryCustomerRepository::set_customers(std::vector<Customer*> const& new_customers) {
	customers = new_customers;
}
void InMemoryCustomerRepository::add_customer(Customer* customer) {
	customers.push_back(customer);
}
void InMemoryCustomerRepository::remove_customer(std::string const& customer_id) {
	//Find the position of the customer
	int position = -1;
	for (int i = 0; i != customers.size(); ++i) {
		if (customer_id == customers[i]->get_id()) {
			position = i;
			break;
		}
	}

	//Remove if element exists
	if (position != -1) {
		customers.erase(customers.begin() + position);
	}
	else {
		std::cerr << "User does not exist" << std::endl;
	}
}
void InMemoryCustomerRepository::update_customer(std::string const& customer_id, ModificationIntent& intent) {
	//Find the position of the customer
	int position = -1;
	for (int i = 0; i != customers.size(); ++i) {
		if (customer_id == customers[i]->get_id()) {
			position = i;
			break;
		}
	}

	//Update if element exists
	if (position != -1) {
		intent.set_customer(customers[position]);
		intent.modify();
	}
	else {
		std::cerr << "User does not exist" << std::endl;
	}
}

//Persistence
std::vector<Customer*> MockCustomerPersistence::load() {
	return {};
}
void MockCustomerPersistence::save(std::vector<Customer*>) {
	//Do nothing
}

//Displayer
void NameOrder::order(std::vector<Customer*>& customers) const {
	std::sort(customers.begin(), customers.end(), [](Customer const* a, Customer const* b) {
		return a->get_name() < b->get_name();
	});
}

void IdOrder::order(std::vector<Customer*>& customers) const {
	std::sort(customers.begin(), customers.end(), [](Customer const* a, Customer const* b) {
		return a->get_id() < b->get_id();
	});
}

void LevelOrder::order(std::vector<Customer*>& customers) const {
	std::sort(customers.begin(), customers.end(), [](Customer const* a, Customer const* b) {
		return a->get_state() < b->get_state();
	});
}

void NoOrder::order(std::vector<Customer*>& customers) const {
	//Do nothing
}


void ConsoleCustomerDisplayer::display(std::vector<Customer*> customers, Order const* order) {
	//Sort first
	order->order(customers);
	for (auto& customer : customers) {
		std::cout << *customer << std::endl;
	}
}

//Filters
IdFilterSpecification::IdFilterSpecification(std::string const& id) : id(id) {}
bool IdFilterSpecification::is_satisfied(Customer const* customer) const {
	return customer->get_id() == id;
}

NameFilterSpecification::NameFilterSpecification(std::string const& name) : name(name) {}
bool NameFilterSpecification::is_satisfied(Customer const* customer) const {
	return customer->get_name() == name;
}

bool AllFilterSpecification::is_satisfied(Customer const* customer) const {
	return true;
}

std::vector<Customer*> CustomerFilterer::filter(std::vector<Customer*> const& customers, FilterSpecification const* spec) {
	std::vector<Customer*> result;

	for (auto customer : customers) {
		if (spec->is_satisfied(customer)) {
			result.push_back(customer);
		}
	}

	return result;
}

//Customer service
CustomerService::CustomerService(CustomerRepository* repo, CustomerDisplayer* display, CustomerFilterer* filterer, CustomerPersistence* persistence) :
	repository(repo), displayer(display), filterer(filterer), persistence(persistence) {}
CustomerService::~CustomerService() {
	if (repository) {
		delete repository;
	}

	if (displayer) {
		delete displayer;
	}

	if (filterer) {
		delete filterer;
	}

	if (persistence) {
		delete persistence;
	}
}

void CustomerService::load() {
	repository->set_customers(persistence->load());
}

void CustomerService::save() {
	persistence->save(repository->get_customers());
}

void CustomerService::add(Customer* customer) {
	repository->add_customer(customer);
}

void CustomerService::remove(std::string const& id) {
	repository->remove_customer(id);
}

void CustomerService::update(std::string const& id, ModificationIntent& intent) {
	repository->update_customer(id, intent);
}

void CustomerService::display(Order const* order) {
	displayer->display(repository->get_customers(), order);
}

void CustomerService::filter(FilterSpecification const* spec) {
	//Get filtered element
	auto filtered = filterer->filter(repository->get_customers(), spec);
	//Display
	NoOrder order;
	displayer->display(filtered, &order);
}