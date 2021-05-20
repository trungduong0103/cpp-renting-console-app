#pragma once

#include "Customer.h"
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
struct ModificationIntent {
protected:
    Customer *customer;
public:
    ModificationIntent() = default;

    ModificationIntent(Customer *);

    inline void set_customer(Customer *custm) { customer = custm; }

    virtual void modify() = 0;
};

//Child of Modification intent
//Used when we want to change a customer's name
struct NameModificationIntent : public ModificationIntent {
    std::string name;

    NameModificationIntent() = default;

    NameModificationIntent(std::string const &name);

    void modify() override;
};

//Child of Modification intent
//Used when we want to change a customer's address
struct AddressModificationIntent : public ModificationIntent {
    std::string address;

    AddressModificationIntent() = default;

    AddressModificationIntent(std::string const &address);

    void modify() override;
};

//Child of Modification intent
//Used when we want to change a customer's phone
struct PhoneModificationIntent : public ModificationIntent {
    std::string phone;

    PhoneModificationIntent() = default;

    PhoneModificationIntent(std::string const &phone);

    void modify() override;
};

//A blue print of repository pattern
//containing methods such as CRUD of customers
struct CustomerRepository {
    virtual void add_customer(Customer *customer) = 0;

    virtual Customer *get_customer(std::string const &) = 0;

    virtual void remove_customer(std::string const &customer_id) = 0;

    virtual void update_customer(std::string const &customer_id, ModificationIntent &intent) = 0;

    virtual std::vector<Customer *> get_customers() = 0;

    virtual void set_customers(std::vector<Customer *> const &) = 0;
};

//Implementation of Repository pattern
//Where all CRUD operation will be done using an in-memory vector
struct InMemoryCustomerRepository : public CustomerRepository {
    std::vector<Customer *> customers;
public:
    InMemoryCustomerRepository() = default;

    InMemoryCustomerRepository(std::vector<Customer *> const &customers);

    Customer *get_customer(std::string const &) override;

    void set_customers(std::vector<Customer *> const &customers) override;

    void add_customer(Customer *customer) override;

    void remove_customer(std::string const &customer_id) override;

    void update_customer(std::string const &customer_id, ModificationIntent &intent) override;

    std::vector<Customer *> get_customers() override { return customers; }
};

//Blueprint for Customer persistence
//Containing two methods: load() for loading customer data
//save() for saving customer data
struct CustomerPersistence {
    virtual std::vector<Customer *> load(std::vector<Item *>) = 0;

    virtual void save(std::vector<Customer *>) = 0;
};

//Implementation of CustomerPersistence
//This is responsible for loading and saving
//customers from and to a text file
struct TextFileCustomerPersistence : public CustomerPersistence {
    std::vector<Customer *> load(std::vector<Item *>) override;
    void save(std::vector<Customer *>) override;
};

//Order classes
//These classes are responsible for
//Sorting a customer list based on their attributes
struct CustomerOrder {
    virtual void order(std::vector<Customer *> &customers) const = 0;
};

//No order: This class will do nothing
struct CustomerNoOrder : public CustomerOrder {
    void order(std::vector<Customer *> &customers) const override;
};

//Order by name: This class will sort the customer based on their names
struct CustomerNameOrder : public CustomerOrder {
    void order(std::vector<Customer *> &customers) const override;
};

//Order by id: This class will sort the customer based on their ids
struct CustomerIdOrder : public CustomerOrder {
    void order(std::vector<Customer *> &customers) const override;
};

//Order by Level: This class will sort the customer based on their levels
//Guest -> Regular -> VIP
struct CustomerLevelOrder : public CustomerOrder {
    void order(std::vector<Customer *> &customers) const override;
};

//Blueprint for CustomerDisplayer
//Which is used to display customer through an interface
struct CustomerDisplayer {
    virtual void display(std::vector<Customer *> customers, CustomerOrder const *order) = 0;
};

//Implementation of CustomerDisplayer
//Responsible for displaying the customer through the console
struct ConsoleCustomerDisplayer : public CustomerDisplayer {
    void display(std::vector<Customer *> customers, CustomerOrder const *order) override;
};

//Specification Design Pattern for filtering
//Each speficification class will check if user satisfies a credential for filtering
struct FilterSpecification {
    virtual bool is_satisfied(Customer const *customer) const = 0;
};

//Filter customer based on their id
struct IdFilterSpecification : public FilterSpecification {
    std::string id;

    IdFilterSpecification(std::string const &id);

    bool is_satisfied(Customer const *customer) const override;
};

//Filter the customer based on their name
struct NameFilterSpecification : public FilterSpecification {
    std::string name;

    NameFilterSpecification(std::string const &name);

    bool is_satisfied(Customer const *customer) const override;
};

//Filter the customer based on their state
struct StateFilterSpecification : public FilterSpecification {
    Category state;

    StateFilterSpecification(Category state);

    bool is_satisfied(Customer const *customer) const override;
};

//Filter base on no conditions -> Every customer is satisfied
struct AllFilterSpecification : public FilterSpecification {
    AllFilterSpecification() = default;

    bool is_satisfied(Customer const *customer) const override;
};

//This class uses a spefication class (IdSpec, NameSpec or all Spec)
//to filter the list of customer and include only those who satistfies the spec
struct CustomerFilterer {
    std::vector<Customer *> filter(std::vector<Customer *> const &customers, FilterSpecification const *);
};

//Aggregated class
//Each attributes: repo, displayer, filterer and persistence
//can be switched out and replaced by another implementation
//to satisfy the Open-Closed principle
class CustomerService {
    CustomerRepository *repository;
    CustomerDisplayer *displayer;
    CustomerFilterer *filterer;
    CustomerPersistence *persistence;

public:
    //Destruct and construct
    CustomerService(CustomerRepository *repo, CustomerDisplayer *display, CustomerFilterer *filterer,
                    CustomerPersistence *persistence);

    ~CustomerService();

    //Customer Service mainly calls
    //methods of its attributes to perform CRUD operations
    void load(std::vector<Item *>);
    void save();
    Customer *get(std::string const &id);
    void add(Customer *customer);
    void remove(std::string const &id);
    void update(std::string const &id, ModificationIntent &intent);
    void display(CustomerOrder const *order);
    void filter(FilterSpecification const *spec);
};