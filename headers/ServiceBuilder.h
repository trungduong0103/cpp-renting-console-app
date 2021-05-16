#pragma once
#include "ItemRepository.h"
#include "CustomerRepository.h"

class ItemServiceBuilder {
public:
    virtual ItemService* create() = 0;
};

class StandardItemServiceBuilder : ItemServiceBuilder {
public:
    ItemService* create() override;
};

class CustomerServiceBuilder {
public:
    virtual CustomerService* create() = 0;
};

class StandardCustomerServiceBuilder : CustomerServiceBuilder {
public:
    CustomerService* create() override;
};
