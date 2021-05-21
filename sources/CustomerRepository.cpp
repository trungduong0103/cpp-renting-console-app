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

/*
	This class contains the Customer Service class
	which is responsible for all CRUD operation on customers
	This class is aggragated by combining different classes such as CustomerRepository (for CRUD operation)
	- Repository design pattern will be used here, CustomerPersistence (for reading and writing files),
	CustomerFilterer (for filtering customer based on their attributes) and CustomerDisplayer
	(for displaying purposes)

	This is to follow open-closed principle: whenever a specification changes, we can switch
	out CustomerRepository and replace by a new implementation, for instance,
	to make the CustomerService more robust to changes
*/

//Intent pattern
//This intent is used for updating a user attributes using the
//repository update method
ModificationIntent::ModificationIntent(Customer *customer) : customer(customer) {}

//Child of Modification intent
//Used when we want to change a customer's name
NameModificationIntent::NameModificationIntent(std::string const &name) : name(name) {}
void NameModificationIntent::modify() {
    customer->set_name(name);
}

//Child of Modification intent
//Used when we want to change a customer's phone
PhoneModificationIntent::PhoneModificationIntent(std::string const &phone) : phone(phone) {}
void PhoneModificationIntent::modify() {
    customer->set_phone(phone);
}

//Child of Modification intent
//Used when we want to change a customer's address
AddressModificationIntent::AddressModificationIntent(std::string const &address) : address(address) {}
void AddressModificationIntent::modify() {
    customer->set_address(address);
}

//Implementation of Repository pattern
//Where all CRUD operation will be done using an in-memory vector
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
    return check_field_contains(customer->get_id(), id);
}

NameFilterSpecification::NameFilterSpecification(std::string const &name) : name(name) {}

bool NameFilterSpecification::is_satisfied(Customer const *customer) const {
    return check_field_contains(customer->get_name(), name);
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

    //Check if there is no item
    if (filtered.size() == 0) {
        std::cout << "Such customer does not exist" << std::endl;
        return;
    }

    //Display
    CustomerNoOrder order;
    displayer->display(filtered, &order);
}

bool already_have_item(const std::vector<std::string> &vector, const std::string &item) {
    return std::count(vector.begin(), vector.end(), item) != 0;
}

Customer *load_customer(
        std::vector<std::string> &customer_vector,
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
        customer_vector[4] = std::to_string(rentals_vector.size());
        std::cout << "[LOG] Changed " << customer_vector[0] << " 's number of rentals to " << rentals_vector.size()
                  << std::endl;
    }

    std::cout << "[INFO] Loaded Customer: " << customer_vector[0] << items_quantity_msg << std::endl;
    for (const std::string &item : rentals_vector) {
        std::cout << "+ " << item << ::std::endl;
        Item *new_item = get_item_with_id(items, item);
        rental_items.push_back(new_item);
    }


    if (customer_vector[customer_vector.size() - 1] == "Guest") {
        std::cout << "[SUCCESS] Successfully created Guest customer with ID: " << customer_vector[0] << std::endl;
        CustomerState *guestState = new GuestState;
        auto *guest_customer = new Customer(
                customer_vector[0],
                customer_vector[1],
                customer_vector[2],
                customer_vector[3],
                std::stoi(customer_vector[4]),
                rental_items,
                guestState);
        return guest_customer;
    } else if (customer_vector[customer_vector.size() - 1] == "Regular") {
        std::cout << "[SUCCESS] Successfully created Regular customer with ID: " << customer_vector[0] << std::endl;
        CustomerState *regularState = new RegularState;
        auto *regular_customer = new Customer(
                customer_vector[0],
                customer_vector[1],
                customer_vector[2],
                customer_vector[3],
                std::stoi(customer_vector[4]),
                rental_items,
                regularState);
        return regular_customer;
    } else if (customer_vector[customer_vector.size() - 1] == "VIP") {
        std::cout << "[SUCCESS] Successfully created VIP customer with ID: " << customer_vector[0] << std::endl;
        CustomerState *vipState = new VIPState;
        auto *vip_customer = new Customer(
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

//Implementation of CustomerPersistence
//This is responsible for loading and saving
//customers from and to a text file
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
                } else {
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
            Customer *customer = load_customer(customer_vector, rentals_vector, items, items_quantity_msg);
            if (customer != nullptr) {
                mockCustomers.push_back(customer);
            } else {
                std::cout << "[FATAL] Something went wrong creating new customer" << std::endl;
            }
        }
    }
    std::cout << "[INFO] Done loading customers!" << std::endl;
    infile.close();
    std::cout << "[INFO] Rewriting content of customers.txt..." << std::endl;
    this->save(mockCustomers);
    return mockCustomers;
}

//Implementation of CustomerPersistence
//This is responsible for loading and saving
//customers from and to a text file
void TextFileCustomerPersistence::save(std::vector<Customer *> customers) {
    std::ofstream outfile("../textfiles/customers.txt", std::ios::trunc);
    if (!outfile) {
        std::cerr << "[ERROR] Cannot write to file items_out.txt" << std::endl;
        return;
    }
    unsigned int i = 0;
    std::string state;
    for (; i < customers.size(); i++) {
        outfile << customers[i]->to_string_file();
        // no newline EOF
        if (i < customers.size() - 1 && customers[i]->get_number_of_rentals() != 0) {
            outfile << "\n";
        }
    }
    std::cout << "[SUCCESS] Successfully saved customers.txt!" << std::endl;
    outfile.close();
}
