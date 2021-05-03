#pragma once

class Menu {
    public:
        void start();
        int process_input(std::string option);
        bool display_main_menu();
        bool display_customer_menu();
        bool display_item_menu();
};