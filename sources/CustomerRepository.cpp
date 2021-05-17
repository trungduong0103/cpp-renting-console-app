#include "../headers/CustomerRepository.h"
#include "../headers/Customer.h"
#include "../headers/CustomerHelpers.h"
#include "../headers/ItemHelpers.h"
#include <algorithm>
#include <fstream>
#include <utility>
#include <vector>

//Modification intents
ModificationIntent::ModificationIntent(Customer *customer) : customer(customer) {}

NameModificationIntent::NameModificationIntent(std::string name) : name(std::move(name)) {}

void NameModificationIntent::modify() {
    customer->set_name(name);
}

PhoneModificationIntent::PhoneModificationIntent(std::string phone) : phone(std::move(phone)) {}

void PhoneModificationIntent::modify() {
    customer->set_phone(phone);
}

AddressModificationIntent::AddressModificationIntent(std::string address) : address(std::move(address)) {}

void AddressModificationIntent::modify() {
    customer->set_address(address);
}

//Repository
InMemoryCustomerRepository::InMemoryCustomerRepository(std::vector<Customer *> customers) : customers(
        std::move(customers)) {}

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

std::vector<std::string> get_customer_as_vector(std::string &line) {
    std::vector<std::string> customer_as_vector;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;

    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        if (token.empty()) {
            return {};
        }
        remove_whitespace(token);
        customer_as_vector.push_back(token);
        line.erase(0, pos + delimiter.length());
    }

    remove_whitespace(line);
    customer_as_vector.push_back(line.substr(0, line.length()));

    return customer_as_vector;
}

//Persistence
std::vector<Customer *> MockCustomerPersistence::load() {
    std::ifstream infile("../textfiles/customers.txt");
    if (!infile) {
        std::cerr << "Cannot read file customers.txt" << std::endl;
        return {};
    }

    unsigned int count = 1;
    std::vector<Customer *> mockCustomers;
    std::string line;
    while(std::getline(infile, line)) {
        std::cout << "LINE " << "[" << count << "]: " << line << std::endl;
        if (line[0] == '#') {
            std::cout << "Ignoring line " << count << " (has # in the beginning)" << std::endl;
        } else if (line.empty()) {
            std::cout << "Ignoring line " << count << " (line is empty)." << std::endl;
        } else if (!correct_customer_info_length(line)) {
            std::cout << "Ignoring line " << count << " (line can only have 5-6 commas)." << std::endl;
        } else {

        }
    }
}

void MockCustomerPersistence::save(std::vector<Customer *>) {
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
IdFilterSpecification::IdFilterSpecification(std::string id) : id(std::move(id)) {}

bool IdFilterSpecification::is_satisfied(Customer const *customer) const {
    return customer->get_id() == id;
}

NameFilterSpecification::NameFilterSpecification(std::string name) : name(std::move(name)) {}

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