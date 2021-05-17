#include <iostream>
#include <string>
#include "../headers/Menu.h"
#include "../headers/CustomerHelpers.h"
#include "../headers/ItemHelpers.h"

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
    while(true){
        if(!display_main_menu()){
            break;
        };
    };
}

int Menu::process_input(const std::string& option_string){
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
        //
        case 1: {
            Customer* customer = nullptr;
            bool successful = read_customer(customer);
            if(successful){
                std::cout << "Add customer successful" << std::endl;
                customer_service->add(customer);
            }
            else{
                std::cout << "Invalid input" << std::endl;
            }
        }
            break;
        case 2: {
            std::string id;
            std::cout << "Input customer ID that you want to edit:" << std::endl;
            std::cin >> id;
            if (customer_service->get(id) != nullptr){
                modify_customer(id);
            }
            else{
                std::cerr << "Customer is not exist" << std::endl;
            }
        }
            break;
        case 3:{
            Customer* customer = nullptr;
        }
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
        case 1: {
            Item* item = nullptr;
            bool successful = read_item(item);
            if(successful){
                std::cout << "Add item successful" << std::endl;
                item_service->add(item);
            }
            else{
                std::cout << "Invalid input" << std::endl;
            }
        }
            break;
        case 2:{
            std::string id;
            std::cout << "Input item ID that you want to edit:" << std::endl;
            std::cin >> id;
            if (item_service->get(id) != nullptr){

            }
            else{
                std::cerr << "Item is not exist" << std::endl;
            }
        }
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

bool Menu::modify_customer(std::string id){
    std::string name;
    std::string address;
    std::string phone;


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
    if (name == ""){
        NameModificationIntent intent{name};
        // customer_service->update("C123", intent);
        std::cout << "hello" << std::endl;
    }
    return true;
}

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
        std::cout << "Select item rental_type:" << std::endl;
        std::cout << "1.Two Day" << std::endl;
        std::cout << "2.One Week" << std::endl;
        std::cin >> rental_type;

        if (rental_type != "1" && rental_type != "2") {
            std::cerr << "Invalid input" << std::endl;
        }
        else {
            break;
        }
    }

    int rental_type_int = std::stoi(rental_type);

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
            std::cout << "4.Comedy" << std::endl;
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
}

