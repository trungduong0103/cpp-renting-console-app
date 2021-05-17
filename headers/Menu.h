#pragma once
#include "ServiceBuilder.h"
#include "Customer.h"
#include "Item.h"
#include "ItemHelpers.h"
#include "CustomerHelpers.h"

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
    static bool read_customer(Customer*& customer);
    static bool read_item(Item*& item);
};