#include "headers/Item.h"
#include "headers/ItemRepository.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    // Create some items
    std::string id1{"I0001"};
    std::string title1{"Item1"};
    Item::RentalType rentaltype1 = Item::RentalType::TwoDay;
    unsigned int stock1 = 5;
    float fee1 = 100.5;
    Item::RentalStatus rentalstatus1 = Item::RentalStatus::Available;
    Game i1(id1, title1, rentaltype1, stock1, fee1, rentalstatus1);

    std::string id2{"I0002"};
    std::string title2{"Item2"};
    Item::RentalType rentaltype2 = Item::RentalType::OneWeek;
    unsigned int stock2 = 15;
    float fee2 = 100.5;
    Item::RentalStatus rentalstatus2 = Item::RentalStatus::Borrowed;
    Game i2(id2, title2, rentaltype2, stock2, fee2, rentalstatus2);

    // Add to repository
    ItemRepository *repository = new InMemoryItemRepository;
    ItemDisplayer *displayer = new ConsoleItemDisplayer;
    ItemFilterer *filterer = new ItemFilterer;
    ItemPersistence *persistence = new MockItemPersistence;

    // Create service
    ItemService service{repository, displayer, filterer, persistence};

    // Load
    service.load();

    // Add item
//    service.add(&i1);
//    service.add(&i2);
//
//    // Display item
//    ItemNoOrder order;
//    std::cout << "------------------" << std::endl;
//    service.display(&order);
//
//    // Modify item
//    ItemTitleModificationIntent intent{"Zook1"};
//    service.update("I0001", intent);
//
//    // Increase number in stock
//    ItemNumberOfStockIncreaseIntent intent1{10};
//    service.update("I0001", intent1);
//    ItemNumberOfStockIncreaseIntent intent4{};
//    service.update("I0001", intent4);
//
//    // Display customer
//    ItemTitleOrder titleOrder;
//    std::cout << "------------------" << std::endl;
//    service.display(&titleOrder);
//
//    // Decrease number in stock
//    ItemNumberOfStockDecreaseIntent intent3{};
//    service.update("I0002", intent3);
//    ItemNumberOfStockDecreaseIntent intent2{15};
//    service.update("I0002", intent2);
//
//    // Filter
//    ItemRentalStatusFilterSpecification spec{Item::RentalStatus::Borrowed};
//    std::cout << "------------------" << std::endl;
//    service.filter(&spec);

    // Save
    service.save();
}