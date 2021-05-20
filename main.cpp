#include "headers/Item.h"
#include "headers/ItemRepository.h"
#include "headers/Menu.h"

using namespace std;

int main() {
    Menu menu;
    menu.start();

    /*
     * TEST

    string itemId1{ "I001-001" };
    string itemTitle1{ "none" };
    int stock1 { 10 };
    double fee1 { 10.99 };
    Item* item1 = new VideoRecord(itemId1, itemTitle1, Item::RentalType::OneWeek, stock1, fee1,
                                  Item::RentalStatus::Available, GenredItem::Genre::Action);

    string id{ "I001" };
    string name{ "John Cena" };
    string phone{ "0909"};
    string address{ "none" };
    CustomerState* state = new GuestState;
    vector<Item*> items = {
            item1,
            item1
    };

    Customer* customer = new Customer(id, name, address, phone, 0, items, state);
    std::cout << customer->to_string_file() << std::endl;
     */
}