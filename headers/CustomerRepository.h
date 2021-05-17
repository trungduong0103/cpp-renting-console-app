#pragma once
#include "Customer.h"
#include <iostream>

//Repository
struct ModificationIntent {
protected:
	Customer* customer;
public:
	ModificationIntent() = default;
	ModificationIntent(Customer*);
	inline void set_customer(Customer* cust) { customer = cust; }
	virtual void modify() = 0;
};

struct NameModificationIntent : public ModificationIntent {
	std::string name;
	NameModificationIntent() = default;
	NameModificationIntent(std::string  name);
	void modify() override;
};

struct AddressModificationIntent : public ModificationIntent {
	std::string address;
	AddressModificationIntent() = default;
	AddressModificationIntent(std::string  address);
	void modify() override;
};

struct PhoneModificationIntent : public ModificationIntent {
	std::string phone;
	PhoneModificationIntent() = default;
	PhoneModificationIntent(std::string  phone);
	void modify() override;
};

struct CustomerRepository {
	virtual void add_customer(Customer* customer) = 0;
    virtual Customer* get_customer(std::string const&) = 0;
	virtual void remove_customer(std::string const& customer_id) = 0;
	virtual void update_customer(std::string const& customer_id, ModificationIntent& intent) = 0;
	virtual std::vector<Customer*> get_customers() = 0;
	virtual void set_customers(std::vector<Customer*> const&) = 0;
};

struct InMemoryCustomerRepository : public CustomerRepository {
	std::vector<Customer*> customers;
public:
	InMemoryCustomerRepository() = default;
	InMemoryCustomerRepository(std::vector<Customer*>  customers);
	Customer* get_customer(std::string const&) override;
	void set_customers(std::vector<Customer*> const& customers) override;
	void add_customer(Customer* customer) override;
	void remove_customer(std::string const& customer_id) override;
	void update_customer(std::string const& customer_id, ModificationIntent& intent) override;
	std::vector<Customer*> get_customers() override { return customers; }
};

//Persistence
struct CustomerPersistence {
	virtual std::vector<Customer*> load() = 0;
	virtual void save(std::vector<Customer*>) = 0;
};

struct MockCustomerPersistence : public CustomerPersistence {
	std::vector<Customer*> load() override;
	void save(std::vector<Customer*>) override;
};

struct TextFileCustomerPersistence : public CustomerPersistence {
	std::vector<Customer*> load() override;
	void save(std::vector<Customer*>) override;
};


//Displayer
struct CustomerOrder {
	virtual void order(std::vector<Customer*>& customers) const = 0;
};

struct CustomerNoOrder : public CustomerOrder {
	void order(std::vector<Customer*>& customers) const override;
};

struct CustomerNameOrder : public CustomerOrder {
	void order(std::vector<Customer*>& customers) const override;
};

struct CustomerIdOrder : public CustomerOrder {
	void order(std::vector<Customer*>& customers) const override;
};

struct CustomerLevelOrder : public CustomerOrder {
	void order(std::vector<Customer*>& customers) const override;
};

struct CustomerDisplayer {
	virtual void display(std::vector<Customer*> customers, CustomerOrder const* order) = 0;
};

struct ConsoleCustomerDisplayer : public CustomerDisplayer {
	void display(std::vector<Customer*> customers, CustomerOrder const* order) override;
};

//Filterer
struct FilterSpecification {
	virtual bool is_satisfied(Customer const* customer) const = 0;
};

struct IdFilterSpecification : public FilterSpecification {
	std::string id;

	IdFilterSpecification(std::string  id);
	bool is_satisfied(Customer const* customer) const override;
};

struct NameFilterSpecification : public FilterSpecification {
	std::string name;

	NameFilterSpecification(std::string  name);
	bool is_satisfied(Customer const* customer) const override;
};

struct AllFilterSpecification : public FilterSpecification {
	AllFilterSpecification() = default;
	bool is_satisfied(Customer const* customer) const override;
};

struct CustomerFilterer {
	std::vector<Customer*> filter(std::vector<Customer*> const& customers, FilterSpecification const*);
};

//Aggregated class
class CustomerService {
	CustomerRepository* repository;
	CustomerDisplayer* displayer;
	CustomerFilterer* filterer;
	CustomerPersistence* persistence;

public:
	//Destruct and construct
	CustomerService(CustomerRepository* repo, CustomerDisplayer* display, CustomerFilterer* filterer, CustomerPersistence* persistence);
	~CustomerService();

	//Methods
	void load();
	void save();
	Customer* get(std::string const&);
	void add(Customer* customer);
	void remove(std::string const& id);
	void update(std::string const& id, ModificationIntent& intent);
	void display(CustomerOrder const* order);
	void filter(FilterSpecification const* spec);
};