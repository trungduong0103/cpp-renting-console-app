#include "../headers/CustomerRepository.h"
#include "../headers/Customer.h"
#include "../headers/ItemHelpers.h"
#include "../headers/CustomerHelpers.h"
#include <algorithm>
#include <string>
#include <utility>
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

StateFilterSpecification::StateFilterSpecification(Category state) : state(state) {}

bool StateFilterSpecification::is_satisfied(Customer const *customer) const {
    return customer->get_state() == state;
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

void CustomerService::load(std::vector<Item *> items) {
    repository->set_customers(persistence->load(std::move(items)));
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

std::vector<Customer *> TextFileCustomerPersistence::load(std::vector<Item *> items) {
    std::ifstream infile("../textfiles/customers.txt");
    if (!infile) {
        std::cerr << "Cannot read file customers.txt" << std::endl;
        return {};
    }
    std::cout << "Loading customers from customer.txt..." << std::endl;
    unsigned int count = 1;
    unsigned int num_rentals = 0;
    unsigned int x, y = 0;
    std::vector<Customer *> mockCustomers;
    std::string line;
    std::vector<std::string> lines;
    std::vector<std::string> customer_vector;
    std::vector<std::string> rentals_vector;

    while (getline(infile, line)) {
        lines.push_back(line);
    }

    for (x = 0; x < lines.size(); x++) {
        // search file until C is reached, ignore the rest of data
        if (lines[x][0] == '#') {
            std::cout << "Ignoring line " << count << " (starts with #): " << lines[x] << std::endl;
        } else if (lines[x][0] == 'C') {
            if (lines[x].empty()) {

            }
            if (!correct_customer_info_length(lines[x])) {
                std::cout << "Ignoring line " << count << " (line can only have 5 commas)." << std::endl;
                continue;
            }
            customer_vector = get_customer_as_vector(lines[x], ",");

            num_rentals = std::stoi(customer_vector[4]);
            // 2 cases for invalid data:
            // number of rental in customer info is smaller than the items
            // number of rental in customer info is bigger than the items
            for (y = 0; y < num_rentals; y++) {
                // all of the lines following the Customer info must begin with I, for Item id
                if (lines[x + y + 1][0] != 'I') {
                    std::cout << "Invalid rental data for "
                              << customer_vector[0] << ": "
                              << lines[x + y + 1]
                              << std::endl;
                    x++;
                } else {
                    rentals_vector.push_back(lines[x + y + 1]);
                }
            }

            std::cout << "--- Customer: "
                      << customer_vector[0]
                      << ", rentals size (adjusted if error): "
                      << rentals_vector.size();
            std::cout << ", with items: " << std::endl;

            for (const std::string &rental : rentals_vector) {
                std::cout << "+ Item: " << rental << std::endl;
            }
            // after the loop to get all items that are rented by a Customer, the next line should
            // begin with C, for Customer
            // but it will not be added to the rentals_vector array
            if (x+y+1 < count) {
                if (lines[x + y + 1][0] != 'C' && !lines[x + y + 1].empty()) {
                    std::cout << "More items than num of rentals in customer info " << "(" << customer_vector[0] << "): "
                              << lines[x + y + 1] << std::endl;
                }
            }

            customer_vector.clear();
            rentals_vector.clear();
        }
        count++;
    }

    std::cout << "LINES COUNT: " << count << std::endl;


    return {};
}

void TextFileCustomerPersistence::save(std::vector<Customer *>) {

}
