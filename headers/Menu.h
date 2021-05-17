#pragma once
#include "ServiceBuilder.h"
#include "Customer.h"
#include "Item.h"


class Menu {
    CustomerService* customer_service;
    ItemService* item_service;

public:
    Menu();
    ~Menu();
    void start();
    int process_input(const std::string& option);
    bool display_main_menu();
    bool display_customer_menu();
    bool display_item_menu();
    bool read_customer(Customer*& customer);
    bool modify_customer(std::string id);
    bool read_item(Item*& item);
};