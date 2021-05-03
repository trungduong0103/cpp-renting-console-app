#include <iostream>
#include <string>
#include "../headers/Menu.h"

void Menu::start(){
    while(true){
        if(!display_main_menu()){
            break;
        }; 
    };
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
                if(!display_customer_menu()){
                    break;
                }; 
            };
            break;
        case 2:
            while(true) {
                if(!display_item_menu()){
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
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
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
        // 
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
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