#pragma once
#include "ServiceBuilder.h"
#include "Customer.h"
#include "Item.h"
#include "ItemHelpers.h"
#include "CustomerHelpers.h"

class Menu {
<<<<<<< HEAD
    CustomerService* customer_service;
    ItemService* item_service;

public:
=======
  CustomerService* customer_service;
  ItemService* item_service;

  public:
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
    Menu();
    ~Menu();
    void start();
    int process_input(std::string option);
    bool display_main_menu();
    bool display_customer_menu();
    bool display_item_menu();
    bool read_customer(Customer*& customer);
<<<<<<< HEAD
    bool modify_customer(Customer*& customer);
=======
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
    bool read_item(Item*& item);
};