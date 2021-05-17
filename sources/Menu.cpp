#include <iostream>
#include <string>
#include "../headers/Menu.h"
#include "../headers/ServiceBuilder.h"
#include "../headers/ItemHelpers.h"
#include "../headers/CustomerHelpers.h"

//Constructor
Menu::Menu() {
    //Create customer service and item service using builder
    StandardCustomerServiceBuilder customer_builder;
    StandardItemServiceBuilder item_builder;
    item_service = item_builder.create();
    customer_service = customer_builder.create();

    //Load in items and customer
    item_service->load();
    customer_service->load();
}

//Destructor
Menu::~Menu() {
    //Save items and customers
    item_service->save();
    customer_service->save();

    //Clear memory
    delete item_service;
    delete customer_service;
}

//Constructor
Menu::Menu() {
  //Create customer service and item service using builder
  StandardCustomerServiceBuilder customer_builder;
  StandardItemServiceBuilder item_builder;
  item_service = item_builder.create();
  customer_service = customer_builder.create();

  //Load in items and customer
  item_service->load();
  customer_service->load();
}

//Destructor
Menu::~Menu() {
  //Save items and customers
  item_service->save();
  customer_service->save();

  //Clear memory
  delete item_service;
  delete customer_service;
}

void Menu::start(){
<<<<<<< HEAD
    while(true){
        if(!display_main_menu()){
            break;
        };
    };
=======
  while(true){
      if(!display_main_menu()){
          break;
      }; 
  };
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
}

int Menu::process_input(std::string option_string){
    try {
        return std::stoi(option_string);
    }
    catch (...) {
        return -1;
    }
}

bool Menu::display_main_menu(){
    // Display menu
    std::cout << "MAIN MENU" << std::endl;
    std::cout << "1. Access item functions." << std::endl;
    std::cout << "2. Access customer functions." << std::endl;
    std::cout << "0. Exit the menu." << std::endl;
    std::cout << "Select option:" << std::endl;

    // Get input
    std::string option_string;
    std::cin >> option_string;
    std::cout << std::endl;

    // Process input
    int option_int = process_input(option_string);
    switch(option_int)
    {
        //
        case 1:
            while(true) {
                if(!display_item_menu()){
                    break;
                };
            };
            break;
        case 2:
            while(true) {
                if(!display_customer_menu()){
                    break;
                };
            };
            break;
        case 0:
            return 0;
            break;
        default:
            std::cerr << "Invalid option. Please try again." << std::endl;
            std::cout << std::endl;
            break;
    }

    return 1;
}

bool Menu::display_customer_menu(){
    // Display menu
    std::cout << "CUSTOMER MENU" << std::endl;
    std::cout << "1. Add a new customer" << std::endl;
    std::cout << "2. Update an existing customer" << std::endl;
    std::cout << "3. Promote an existing customer" << std::endl;
    std::cout << "4. Display all customers" << std::endl;
    std::cout << "5. Display group of customers" << std::endl;
    std::cout << "6. Search customers" << std::endl;
    std::cout << "0. Exit." << std::endl;
    std::cout << "Select option:" << std::endl;

    // Get input
    std::string option_string;
    std::cin >> option_string;
    std::cout << std::endl;

    // Process input
    int option_int = process_input(option_string);
    switch(option_int)
    {
<<<<<<< HEAD
        //
=======
        // 
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
        case 1: {
            Customer* customer = nullptr;
            bool successful = read_customer(customer);
            if(successful){
                std::cout << "Add customer successful" << std::endl;
<<<<<<< HEAD
                customer_service->add(customer);
=======
                customer_service->add(customer);   
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
            }
            else{
                std::cout << "Invalid input" << std::endl;
            }
        }
            break;
<<<<<<< HEAD
        case 2: {
            std::string id;
            std::cout << "Input item ID that you want to edit:" << std::endl;
            if (customer_service->get(id) != nullptr){

            }
            else{
                std::cerr << "Customer is not exist" << std::endl;
            }
        }
            break;
        case 3:{
            Customer* customer = nullptr;
        }
=======
        case 2: 
            break;
        case 3: 
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
            break;
        case 4: {
            CustomerNoOrder no_order;
            customer_service->display(&no_order);
        }
            break;
        case 5:{
            std::string option;
            std::cout << "Select group of customer:" << std::endl;
            std::cout << "1. Guest customer" << std::endl;
            std::cout << "2. Regular customer" << std::endl;
            std::cout << "3. VIP customer" << std::endl;
            std::cin >> option;
            if (option == "1"){

            }
            else if (option == "2"){

            }
            else if (option == "3"){

            }
            else{
                std::cout << "Invalid input" << std::endl;
            }
        }
            break;
        case 6:
            break;
        case 0:
            return 0;
            break;
        default:
            std::cerr << "Invalid option. Please try again." << std::endl;
            std::cout << std::endl;
            break;
    }

    return 1;
}

bool Menu::display_item_menu(){
    // Display menu
    std::cout << "ITEM MENU" << std::endl;
    std::cout << "1. Add a new item" << std::endl;
    std::cout << "2. Update an existing item" << std::endl;
    std::cout << "3. Delete an existing item" << std::endl;
    std::cout << "4. Rent an item" << std::endl;
    std::cout << "5. Renturn an item" << std::endl;
    std::cout << "6. Display all items" << std::endl;
    std::cout << "7. Display out of stock item" << std::endl;
    std::cout << "8. Search items" << std::endl;
    std::cout << "0. Exit." << std::endl;
    std::cout << "Select option:" << std::endl;

    // Get input
    std::string option_string;
    std::cin >> option_string;
    std::cout << std::endl;

    // Process input
    int option_int = process_input(option_string);
    switch(option_int)
    {
<<<<<<< HEAD
        case 1: {
=======
        // 
        case 1:{
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
            Item* item = nullptr;
            bool successful = read_item(item);
            if(successful){
                std::cout << "Add item successful" << std::endl;
<<<<<<< HEAD
                item_service->add(item);
=======
                std::cout << item << std::endl;
                item_service->add(item);   
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
            }
            else{
                std::cout << "Invalid input" << std::endl;
            }
        }
            break;
        case 2:
            break;
        case 3:{
            std::string id;
            std::cout << "Input item ID that you want to delete:" << std::endl;
            std::cin >> id;
            item_service->remove(id);
        }
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:{
            ItemNoOrder no_order;
            item_service->display(&no_order);
        }
            break;
        case 7:{
            ItemNumStockFilterSpecification spec{0};
            item_service -> filter(&spec);
        }
            break;
        case 8:
            break;
        case 0:
            return 0;
            break;
        default:
            std::cerr << "Invalid option. Please try again." << std::endl;
            std::cout << std::endl;
            break;
    }

    return 1;
}

bool Menu::read_customer(Customer*& customer){
    std::string id;
    std::string name;
    std::string address;
    std::string phone;

    std::cout << "Input user ID:" << std::endl;
    std::cin >> id;

    std::cout << "Input user name:" << std::endl;
    std::cin >> name;

    std::cout << "Input user address:" << std::endl;
    std::cin >> address;

    while (true){
        std::cout << "Input user phone:" << std::endl;
        std::cin >> phone;
        if (!customer_phone_is_valid(phone)) {
            std::cerr << "Invalid input" << std::endl;
        }
        else {
            break;
        }
    }


    int number_of_rentals = 0;
    std::vector<std::string> items;
    CustomerState* state = new GuestState;

    customer = new Customer(id, name, address, phone, number_of_rentals, items, state);

    return true;
}

<<<<<<< HEAD
bool Menu::modify_customer(Customer*& customer){
    std::string id;
    std::string name;
    std::string address;
    std::string phone;

    std::cout << "Input user ID:" << std::endl;
    std::cin >> id;

    std::cout << "Input user name:" << std::endl;
    std::cin >> name;

    std::cout << "Input user address:" << std::endl;
    std::cin >> address;

    while (true){
        std::cout << "Input user phone:" << std::endl;
        std::cin >> phone;
        if (!customer_phone_is_valid(phone)) {
            std::cerr << "Invalid input" << std::endl;
        }
        else {
            break;
        }
    }


    int number_of_rentals = 0;
    std::vector<std::string> items;
    CustomerState* state = new GuestState;

    customer = new Customer(id, name, address, phone, number_of_rentals, items, state);

    return true;
}

=======
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
bool Menu::read_item(Item*& item){
    std::string type;
    std::string id;
    std::string title;
    std::string rental_type;
    std::string stock;
    std::string fee;
    std::string genre;

    while (true){
        std::cout << "Select item type:" << std::endl;
        std::cout << "1.Game" << std::endl;
        std::cout << "2.Record" << std::endl;
        std::cout << "3.DVD" << std::endl;
        std::cin >> type;
        if (type != "1" && type != "2" && type != "3") {
            std::cerr << "Invalid input" << std::endl;
        }
        else {
            break;
        }
    }
<<<<<<< HEAD

    while (true){
        std::cout << "Input item ID:" << std::endl;
        std::cin >> id;
        std::cin.ignore();
        if (!item_id_is_valid(id)) {
            std::cerr << "Invalid input" << std::endl;
        }
        else if (item_service->get(id) != nullptr){
            std::cerr << "Duplicate" << std::endl;
        }
        else {
            break;
        }
    }

    std::cout << "Input item title:" << std::endl;
    std::getline(std::cin, title);

    while (true){
=======
    
    std::cout << "Input item ID:" << std::endl;
    std::cin >> id;

    std::cout << "Input item title:" << std::endl;
    std::cin >> title;

    while (true){    
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
        std::cout << "Select item rental_type:" << std::endl;
        std::cout << "1.Two Day" << std::endl;
        std::cout << "2.One Week" << std::endl;
        std::cin >> rental_type;
<<<<<<< HEAD

=======
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
        if (rental_type != "1" && rental_type != "2") {
            std::cerr << "Invalid input" << std::endl;
        }
        else {
            break;
        }
    }
<<<<<<< HEAD

    int rental_type_int = std::stoi(rental_type);

=======
    int rental_type_int = std::stoi(rental_type);


>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
    while (true){
        std::cout << "Input item number in stock:" << std::endl;
        std::cin >> stock;
        if (!item_stock_is_valid(stock)) {
            std::cerr << "Invalid input" << std::endl;
        }
        else {
            break;
        }
    }
    int stock_int = std::stoi(stock);

    while (true){
        std::cout << "Input item fee:" << std::endl;
        std::cin >> fee;
        if (!item_price_is_valid(stock)) {
            std::cerr << "Invalid input" << std::endl;
        }
        else {
            break;
        }
    }
    float fee_float = std::stof(fee);

    int genre_int;
    if (type != "1"){
        while (true){
            std::cout << "Select genre:" << std::endl;
            std::cout << "1.Action" << std::endl;
            std::cout << "2.Horror" << std::endl;
            std::cout << "3.Drama" << std::endl;
<<<<<<< HEAD
            std::cout << "4.Comedy" << std::endl;
=======
            std::cout << "4.Comedy" << std::endl; 
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
            std::cin >> genre;
            if (genre != "1" && genre != "2" && genre != "3" &&genre != "4") {
                std::cerr << "Invalid input" << std::endl;
            }
            else {
                break;
            }
        }
        genre_int = std::stoi(genre);
    }
<<<<<<< HEAD

=======
        
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
    Item::RentalStatus rental_status = Item::RentalStatus::Available;

    if(type == "1"){
        item = new Game(id, title, Item::RentalType(rental_type_int-1), stock_int, fee_float, rental_status);
    }
    else if(type == "2"){
        item = new DVD(id, title, Item::RentalType(rental_type_int-1), stock_int, fee_float, rental_status, GenredItem::Genre(genre_int - 1));
    }
    else if(type == "3"){
        item = new VideoRecord(id, title, Item::RentalType(rental_type_int-1), stock_int, fee_float, rental_status, GenredItem::Genre(genre_int - 1));
    }

    return true;
<<<<<<< HEAD
}

=======
}
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
