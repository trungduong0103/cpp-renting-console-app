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

bool already_have_item(const std::vector<std::string> &vector, const std::string &item) {
    return std::count(vector.begin(), vector.end(), item) != 0;
}

Customer * load_customer(
        const std::vector<std::string> &customer_vector,
        std::vector<std::string> rentals_vector,
        const std::vector<Item *> &items,
        std::string items_quantity_msg
) {
    std::vector<Item *> rental_items;
    items_quantity_msg = rentals_vector.empty() ? " with no items." : " with item(s):";
    if (customer_vector[5] == "Guest") {
        unsigned int video_count = 0;
        for (const std::string &item_id : rentals_vector) {
            Item *item = get_item_with_id(items, item_id);
            if (item->get_type() == ItemType::VIDEO) {
                video_count++;
            }
        }

        if (video_count > 2) {
            std::cout << "[ERROR] " << customer_vector[0]
                      << " (Guest) can only borrow 2 Video Items at a time" << std::endl;
            unsigned int difference = video_count - 2;
            unsigned int delete_count = 0;
            for (unsigned int i = rentals_vector.size() - 1; i > 0; i--) {
                Item *item = get_item_with_id(items, rentals_vector[i]);
                if (delete_count == difference) {
                    std::cout << "[LOG] Excess videos deleted." << std::endl;
                    break;
                }
                if (item->get_type() == ItemType::VIDEO) {
                    std::cout << "[LOG] " << "Removing video: " << rentals_vector[i] << std::endl;
                    rentals_vector.erase(rentals_vector.begin() + i);
                    delete_count++;
                }
            }
        }
    }

    if (std::stoi(customer_vector[4]) < rentals_vector.size()) {
        std::cout
                << "[ERROR] "
                << customer_vector[0]
                << "'s number of rentals is smaller than actual items after customer info!"
                << std::endl;
        std::cout << "[LOG] Number of rentals: " << customer_vector[4] << std::endl;
        std::cout << "[LOG] Actual items: " << rentals_vector.size() << std::endl;
        unsigned int difference = rentals_vector.size() - std::stoi(customer_vector[4]), i = 0;
        for (; i < difference; i++) {
            rentals_vector.pop_back();
        }
        std::cout << "[LOG] Will only store " << rentals_vector.size() << " item(s): " << std::endl;
        for (const std::string &item : rentals_vector) {
            std::cout << "+ " << item << ::std::endl;
        }
    } else if (std::stoi(customer_vector[4]) > rentals_vector.size()) {
        std::cout
                << "[ERROR] " << customer_vector[0]
                << "'s number of rentals is bigger than actual items after customer info! "
                << std::endl;
        std::cout << "[LOG] Number of rentals: " << customer_vector[4] << std::endl;
        std::cout << "[LOG] Actual items: " << rentals_vector.size() << std::endl;
    }

    std::cout << "[INFO] Loaded Customer: " << customer_vector[0] << items_quantity_msg << std::endl;
    for (const std::string &item : rentals_vector) {
        std::cout << "+ " << item << ::std::endl;
        Item *new_item = get_item_with_id(items, item);
        rental_items.push_back(new_item);
    }


    if (customer_vector[customer_vector.size() - 1] == "Guest") {
        std::cout << "[SUCCESS] Guest customer with ID: " << customer_vector[0] << " successfully created!" << std::endl;
        CustomerState *guestState = new GuestState;
        Customer *guest_customer = new Customer(
                customer_vector[0],
                customer_vector[1],
                customer_vector[2],
                customer_vector[3],
                std::stoi(customer_vector[4]),
                rental_items,
                guestState);
        return guest_customer;
    }
    else if (customer_vector[customer_vector.size() - 1] == "Regular") {
        std::cout << "[SUCCESS] Regular customer with ID: " << customer_vector[0] << " successfully created!" << std::endl;
        CustomerState *regularState = new RegularState;
        Customer *regular_customer = new Customer(
                customer_vector[0],
                customer_vector[1],
                customer_vector[2],
                customer_vector[3],
                std::stoi(customer_vector[4]),
                rental_items,
                regularState);
        return regular_customer;
    }
    else if (customer_vector[customer_vector.size() - 1] == "VIP") {
        std::cout << "[SUCCESS] VIP customer with ID: " << customer_vector[0] << " successfully created!" << std::endl;
        CustomerState *vipState = new VIPState;
        Customer *vip_customer = new Customer(
                customer_vector[0],
                customer_vector[1],
                customer_vector[2],
                customer_vector[3],
                std::stoi(customer_vector[4]),
                rental_items,
                vipState);
        return vip_customer;
    }

    return nullptr;
}

std::vector<Customer *> TextFileCustomerPersistence::load(std::vector<Item *> items) {
    std::ifstream infile("../textfiles/customers.txt");
    if (!infile) {
        std::cerr << "Cannot read file customers.txt" << std::endl;
        return {};
    }
    std::cout << "[INFO] Loading customers from customer.txt..." << std::endl;
    unsigned int count = 1;
    unsigned int x = 0;
    std::vector<Customer *> mockCustomers;
    std::string line;
    std::vector<std::string> lines;
    std::vector<std::string> customer_vector;
    std::vector<std::string> rentals_vector;
    std::string items_quantity_msg;

    while (getline(infile, line)) {
        remove_carriage_return(line);
        lines.push_back(line);
    }

    for (x = 0; x < lines.size(); x++) {
        // search vector until C is reached, ignoring the rest of data prior to first C
        if (lines[x][0] == '#') {
            std::cout << "[LOG] Ignoring line " << count << " (starts with #): " << lines[x] << std::endl;
        } else if (lines[x][0] == 'C') {
            // customer and items have been loaded
            if (!customer_vector.empty()) {
                Customer *customer = load_customer(customer_vector, rentals_vector, items, items_quantity_msg);
                if (customer != nullptr) {
                    mockCustomers.push_back(customer);
                }
                else {
                    std::cout << "[FATAL] Something went wrong creating new customer" << std::endl;
                }
            }
            // clear customer_vector and rentals_vector for next customer
            customer_vector.clear();
            rentals_vector.clear();
            // change customer into a customer vector
            if (valid_customer_data(lines[x])) customer_vector = get_customer_as_vector(lines[x]);
            else
                std::cout << "[ERROR] Invalid customer info at line ["
                          << x + 1
                          << "]. Any dangling items or info that is not a customer info will be ignored"
                          << std::endl;

        } else if (lines[x][0] == 'I') {
            if (item_id_is_valid(lines[x])) {
                // check for duplicate items
                if (already_have_item(rentals_vector, lines[x])) {
                    std::cout << "[ERROR] Duplicate item: " << lines[x] << ", ignoring line " << x << std::endl;
                    // check if item does not exists
                } else if (!item_exists_with_id(items, lines[x])) {
                    std::cout << "[ERROR] No item exists with ID: " << lines[x] << " ignoring line " << x << std::endl;
                } else {
                    rentals_vector.push_back(lines[x]);
                }
            }
        }
        // for the final customer
        if (!customer_vector.empty() && x == lines.size() - 1) {
            load_customer(customer_vector, rentals_vector, items, items_quantity_msg);
        }
    }
    std::cout << "[INFO] Done loading customers!" << std::endl;
    infile.close();
    return mockCustomers;
}

void TextFileCustomerPersistence::save(std::vector<Customer *>) {

}
