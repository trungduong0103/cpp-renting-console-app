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
    static int process_input(const std::string& option);
    bool display_main_menu();
    bool display_customer_menu();
    bool display_item_menu();
    void read_customer(Customer*& customer);
    void modify_customer(const std::string& id);
    void read_item(Item*& item);
    void modify_item(const std::string& id, ItemType type);
};