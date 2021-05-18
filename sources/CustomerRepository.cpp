#include "../headers/CustomerRepository.h"
#include "../headers/Customer.h"
#include "../headers/ItemHelpers.h"
#include "../headers/CustomerHelpers.h"
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

//Modification intents
ModificationIntent::ModificationIntent(Customer *customer) : customer(customer) {}

NameModificationIntent::NameModificationIntent(std::string const &name) : name(name) {}

void NameModificationIntent::modify() {
    customer->set_name(name);
}

PhoneModificationIntent::PhoneModificationIntent(std::string const &phone) : phone(phone) {}

void PhoneModificationIntent::modify() {
    customer->set_phone(phone);
}

AddressModificationIntent::AddressModificationIntent(std::string const &address) : address(address) {}

void AddressModificationIntent::modify() {
    customer->set_address(address);
}

//Repository
InMemoryCustomerRepository::InMemoryCustomerRepository(std::vector<Customer *> const &customers) : customers(
        customers) {}

void InMemoryCustomerRepository::set_customers(std::vector<Customer *> const &new_customers) {
    customers = new_customers;
}

Customer *InMemoryCustomerRepository::get_customer(std::string const &id) {
    for (auto customer_ptr : customers) {
        if (customer_ptr != nullptr && customer_ptr->get_id() == id) {
            return customer_ptr;
        }
    }
    //No customer found
    return nullptr;
}

void InMemoryCustomerRepository::add_customer(Customer *customer) {
    customers.push_back(customer);
}

void InMemoryCustomerRepository::remove_customer(std::string const &customer_id) {
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
    } else {
        std::cerr << "User does not exist" << std::endl;
    }
}

void InMemoryCustomerRepository::update_customer(std::string const &customer_id, ModificationIntent &intent) {
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
    } else {
        std::cerr << "User does not exist" << std::endl;
    }
}

//Displayer
void CustomerNameOrder::order(std::vector<Customer *> &customers) const {
    std::sort(customers.begin(), customers.end(), [](Customer const *a, Customer const *b) {
        return a->get_name() < b->get_name();
    });
}

void CustomerIdOrder::order(std::vector<Customer *> &customers) const {
    std::sort(customers.begin(), customers.end(), [](Customer const *a, Customer const *b) {
        return a->get_id() < b->get_id();
    });
}

void CustomerLevelOrder::order(std::vector<Customer *> &customers) const {
    std::sort(customers.begin(), customers.end(), [](Customer const *a, Customer const *b) {
        return a->get_state() < b->get_state();
    });
}

void CustomerNoOrder::order(std::vector<Customer *> &customers) const {
    //Do nothing
}


void ConsoleCustomerDisplayer::display(std::vector<Customer *> customers, CustomerOrder const *order) {
    //Sort first
    order->order(customers);
    for (auto &customer : customers) {
        std::cout << *customer << std::endl;
    }
}

//Filters
IdFilterSpecification::IdFilterSpecification(std::string const &id) : id(id) {}

bool IdFilterSpecification::is_satisfied(Customer const *customer) const {
    return customer->get_id() == id;
}

NameFilterSpecification::NameFilterSpecification(std::string const &name) : name(name) {}

bool NameFilterSpecification::is_satisfied(Customer const *customer) const {
    return customer->get_name() == name;
}

bool AllFilterSpecification::is_satisfied(Customer const *customer) const {
    return true;
}

std::vector<Customer *>
CustomerFilterer::filter(std::vector<Customer *> const &customers, FilterSpecification const *spec) {
    std::vector<Customer *> result;

    for (auto customer : customers) {
        if (spec->is_satisfied(customer)) {
            result.push_back(customer);
        }
    }

    return result;
}

//Customer service
CustomerService::CustomerService(CustomerRepository *repo, CustomerDisplayer *display, CustomerFilterer *filterer,
                                 CustomerPersistence *persistence) :
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

Customer *CustomerService::get(std::string const &id) {
    return repository->get_customer(id);
}

void CustomerService::add(Customer *customer) {
    repository->add_customer(customer);
}

void CustomerService::remove(std::string const &id) {
    repository->remove_customer(id);
}

void CustomerService::update(std::string const &id, ModificationIntent &intent) {
    repository->update_customer(id, intent);
}

void CustomerService::display(CustomerOrder const *order) {
    displayer->display(repository->get_customers(), order);
}

void CustomerService::filter(FilterSpecification const *spec) {
    //Get filtered element
    auto filtered = filterer->filter(repository->get_customers(), spec);

    //Display
    CustomerNoOrder order;
    displayer->display(filtered, &order);
}

std::vector<std::string> split(const std::string &str, const std::string &delim) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

std::vector<Customer *> TextFileCustomerPersistence::load() {
    std::ifstream infile("../textfiles/customers.txt");
    if (!infile) {
        std::cerr << "Cannot read file customers.txt" << std::endl;
        return {};
    }

    unsigned int count = 1;
    unsigned int num_rentals = 0;
    unsigned int i = 0;
    std::vector<Customer *> mockCustomers;
    std::vector<std::string> rentals;
    std::string line;
    const std::string default_ignore = "Ignoring line ";
    std::string prev;
    std::vector<std::string> lines;
    std::vector<std::string> customer_vector;
    std::vector<std::string> rental_vectors;

    while (getline(infile, line)) {
        lines.push_back(line);
    }

    for (int x = 0; x < lines.size(); x++) {
        if (lines[x][0] == 'C') {
            std::cout << "Customer: " << lines[x] << std::endl;
            customer_vector = split(lines[x], ",");
            num_rentals = std::stoi(customer_vector[4]);
            for (int y = 0; y < num_rentals; y++) {
                std::cout << "Item: " << lines[x + y + 1] << std::endl;
                if (lines[x + y + 1][0] != 'I') {
                    std::cout << "Invalid rental data: " << lines[x + y + 1] << std::endl;
                } else {
                    rental_vectors.push_back(lines[x + y + 1]);
                }
            }
            std::cout << "Cust v: " << customer_vector[0] << std::endl;
            std::cout << "Rent v size: " << rental_vectors.size() << std::endl;
            customer_vector.clear();
            rental_vectors.clear();
        }
    }


    return {};
}

void TextFileCustomerPersistence::save(std::vector<Customer *>) {

}
