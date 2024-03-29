#include "../headers/ItemRepository.h"
#include "../headers/ItemHelpers.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

/*
	This class contains the Item Service class
	which is responsible for all CRUD operation on items
	This class is aggragated by combining different classes such as ItemRepository (for CRUD operation)
	- Repository design pattern will be used here, ItemPersistence (for reading and writing files),
	ItemFilterer (for filtering customer based on their attributes) and ItemDisplayer
	(for displaying purposes)

	This is to follow open-closed principle: whenever a specification changes, we can switch
	out ItemRepository and replace by a new implementation, for instance,
	to make the ItemService more robust to changes
*/

//Intent pattern
//This intent is used for updating a user attributes using the
//repository update method
ItemModificationIntent::ItemModificationIntent(Item *item) : item(item) {}

//Child of Modification intent
//Used when we want to change an item's title
ItemTitleModificationIntent::ItemTitleModificationIntent(std::string title) : title(std::move(title)) {}
void ItemTitleModificationIntent::modify() {
    item->set_title(title);
}

//Child of Modification intent
//Used when we want to change an item's rental type
ItemRentalTypeModificationIntent::ItemRentalTypeModificationIntent(Item::RentalType rental_type) : rental_type(
        rental_type) {}
void ItemRentalTypeModificationIntent::modify() {
    item->set_rental_type(rental_type);
}

//Child of Modification intent
//Used when we want to change an item's fee
ItemFeeModificationIntent::ItemFeeModificationIntent(float fee) : fee(fee) {}
void ItemFeeModificationIntent::modify() {
    item->set_rental_fee(fee);
}

//Child of Modification intent
//Used when we want to change an item's stock
ItemNumStockModificationIntent::ItemNumStockModificationIntent(unsigned int number_in_stock) : number_in_stock(
        number_in_stock) {}

void ItemNumStockModificationIntent::modify() {
    item->set_num_in_stock(number_in_stock);
}

//Child of Modification intent
//Used when we want to change an item's stock
ItemNumberOfStockIncreaseIntent::ItemNumberOfStockIncreaseIntent(unsigned int value) : value(value) {}
void ItemNumberOfStockIncreaseIntent::modify() {
    item->increase_num_in_stock(value);
}

//Child of Modification intent
//Used when we want to change an item's stock
ItemNumberOfStockDecreaseIntent::ItemNumberOfStockDecreaseIntent(unsigned int value) : value(value) {}
void ItemNumberOfStockDecreaseIntent::modify() {
    item->decrease_num_in_stock(value);
}

//Child of Modification intent
//Used when we want to change an item's genre
GenredItemGenreModificationIntent::GenredItemGenreModificationIntent(GenredItem::Genre genre) : genre(
        genre) {}
void GenredItemGenreModificationIntent::modify() {
    genred_item->set_genre(genre);
}

//Implementation of Repository pattern
//Where all CRUD operation will be done using an in-memory vector
InMemoryItemRepository::InMemoryItemRepository(std::vector<Item *> items) : items(std::move(items)) {}
InMemoryItemRepository::~InMemoryItemRepository() {
    for (auto item_ptr : items) {
        if (item_ptr != nullptr) {
            delete item_ptr;
        }
    }
}

void InMemoryItemRepository::set_items(std::vector<Item *> const &new_items) {
    items = new_items;
}

void InMemoryItemRepository::add_item(Item *item) {
    items.push_back(item);
}

void InMemoryItemRepository::remove_item(std::string const &item_id) {
    //Find the position of the item
    int position = get_item_index(item_id);

    //Get the item and check if it is borrowed or not
    if (!items[position]->is_available()) {
        //Display error message
        std::cerr << "Item is currently borrowed and can not be deleted" << std::endl;
        return;
    }

    //Remove if element exists
    if (position != -1) {
        items.erase(items.begin() + position);
    } else {
        std::cerr << "User does not exist" << std::endl;
    }
}

void InMemoryItemRepository::update_item(std::string const &item_id, ItemModificationIntent &intent) {
    //Find the position of the item
    int position = get_item_index(item_id);

    //Update if element exists
    if (position != -1) {
        intent.set_item(items[position]);
        intent.modify();
    } else {
        std::cerr << "Item does not exist" << std::endl;
    }
}

void InMemoryItemRepository::update_genred_item(std::string const &item_id, GenredItemModificationIntent &intent) {
    //Find the position of the item
    int position = get_item_index(item_id);

    //Update if element exists
    if (position != -1) {
        intent.set_item((GenredItem *) items[position]);
        intent.modify();
    } else {
        std::cerr << "Item does not exist" << std::endl;
    }
}

Item *InMemoryItemRepository::get_item(std::string const &id) {
    //Get the item position
    int position = get_item_index(id);

    //Then get item at that position
    return position == -1 ? nullptr : items[position];
}

int InMemoryItemRepository::get_item_index(std::string const &item_id) {
    int position = 0;

    for (; position < items.size(); position++) {
        if (item_id == items[position]->get_id()) {
            return position;
        }
    }

    return -1;
}

std::vector<std::string> get_item_as_vector(std::string &line) {
    std::vector<std::string> item_as_vector;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;

    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        if (token.empty()) {
            return {};
        }
        remove_carriage_return(token);
        item_as_vector.push_back(token);
        line.erase(0, pos + delimiter.length());
    }
    remove_carriage_return(line);
    item_as_vector.push_back(line.substr(0, line.length()));
    return item_as_vector;
}

//Implementation of ItemPersistence
//This is responsible for loading and saving
//customers from and to a text file
std::vector<Item *> TextFileItemPersistence::load() {
    std::ifstream infile("../textfiles/items.txt");
    if (!infile) {
        std::cerr << "Cannot read file items.txt..." << std::endl;
        return {};
    }
    std::cout << "[INFO] Loading items from items.txt..." << std::endl;
    unsigned int count = 1;
    std::vector<Item *> mockItems;
    std::string line;
    const std::string default_ignore = "[LOG] Ignoring line ";
    while (std::getline(infile, line)) {
        if (line[0] == '#') {
            std::cout << default_ignore << count << " (has # in the beginning)" << std::endl;
        } else if (line.empty()) {
            std::cout << default_ignore << count << " (line is empty)." << std::endl;
        } else if (!correct_info_length(line)) {
            std::cout << default_ignore << count << " (line can only have 5-6 commas)." << std::endl;
        } else {
            std::vector<std::string> item_vector = get_item_as_vector(line);
            if (item_vector.empty()) {
                std::cout << default_ignore << count << " (line is missing info)." << std::endl;
            } else {
                if (valid_item_data(
                        item_vector[0],
                        mockItems,
                        item_vector[2],
                        item_vector[item_vector.size() - 1],
                        item_vector.size(),
                        item_vector[3],
                        item_vector[4],
                        item_vector[5]
                )) {

                    // 6 means video game
                    if (item_vector.size() == 6) {
                        Item *game = new Game(
                                item_vector[0],
                                item_vector[1],
                                string_to_rental_type(item_vector[3]),
                                std::stoi(item_vector[4]),
                                std::stof(item_vector[5]),
                                Item::RentalStatus::Available
                        );
                        mockItems.push_back(game);
                        std::cout << "[SUCCESS] Successfully created Game listing with ID: "
                                  << item_vector[0]
                                  << std::endl;
                    }
                        // 7 means dvd or record
                    else if (item_vector.size() == 7) {
                        if (item_vector[2] == "DVD") {
                            Item *dvd = new DVD(
                                    item_vector[0],
                                    item_vector[1],
                                    string_to_rental_type(item_vector[3]),
                                    std::stoi(item_vector[3]),
                                    std::stof(item_vector[4]),
                                    Item::RentalStatus::Available,
                                    string_to_genre(item_vector[6])
                            );
                            mockItems.push_back(dvd);
                            std::cout << "[SUCCESS] Successfully created DVD listing with ID: "
                                      << item_vector[0]
                                      << std::endl;
                        } else if (item_vector[2] == "Record") {
                            Item *videoRecord = new VideoRecord(
                                    item_vector[0],
                                    item_vector[1],
                                    string_to_rental_type(item_vector[3]),
                                    std::stoi(item_vector[3]),
                                    std::stof(item_vector[4]),
                                    Item::RentalStatus::Available,
                                    string_to_genre(item_vector[6])
                            );
                            mockItems.push_back(videoRecord);
                            std::cout << "[SUCCESS] Successfully created Video Record listing with ID: "
                                      << item_vector[0]
                                      << std::endl;
                        }
                    } else {
                        std::cerr << "Unexpected length of item :/" << std::endl;
                    }
                } else {
                    continue;
                }
            }
        }
        count++;
    }
    std::cout << "[INFO] Done loading items!" << std::endl;
    infile.close();
    std::cout << "[INFO] Rewriting content of items.txt..." << std::endl;
    this->save(mockItems);
    return mockItems;
}

//Implementation of ItemPersistence
//This is responsible for loading and saving
//customers from and to a text file
void TextFileItemPersistence::save(std::vector<Item *> items) {
    std::ofstream outfile("../textfiles/items.txt", std::ios::trunc);
    if (!outfile) {
        std::cerr << "[ERROR] Cannot write to file items_out.txt" << std::endl;
        return;
    }
    unsigned int i = 0;
    for (; i < items.size(); i++) {
        outfile << items[i]->to_string_file();
        // no newline EOF
        if (i < items.size() - 1) {
            outfile << "\n";
        }
    }
    std::cout << "[SUCCESS] Successfully saved items.txt!" << std::endl;
    outfile.close();
}

//Order by name: This class will sort the items based on their titles
void ItemTitleOrder::order(std::vector<Item *> &items) const {
    std::sort(items.begin(), items.end(), [](Item const *a, Item const *b) {
        return a->get_title() < b->get_title();
    });
}

//Order by id: This class will sort the items based on their ids
void ItemIdOrder::order(std::vector<Item *> &items) const {
    std::sort(items.begin(), items.end(), [](Item const *a, Item const *b) {
        return a->get_id() < b->get_id();
    });
}

//No order: This class will do nothing
void ItemNoOrder::order(std::vector<Item *> &items) const {
    //Do nothing
}

//Implementation of ItemDisplayer
//Responsible for displaying the items through the console
void ConsoleItemDisplayer::display(std::vector<Item *> items, ItemOrder const *order) {
    //Sort first
    order->order(items);
    for (auto &item : items) {
        std::cout << *item << std::endl;
    }
}

//Filters
//Filter item based on their number of stock
ItemNumStockFilterSpecification::ItemNumStockFilterSpecification(unsigned int const number_in_stock)
        : number_in_stock(number_in_stock) {}
bool ItemNumStockFilterSpecification::is_satisfied(Item const *item) const {
    return item->get_number_in_stock() == number_in_stock;
}

//Filter item based on their id
ItemIdFilterSpecification::ItemIdFilterSpecification(std::string id)
        : id(id) {}

bool ItemIdFilterSpecification::is_satisfied(Item const *item) const {
    return check_field_contains(item->get_id(), id);
}

//Filter item based on their title
ItemTitleFilterSpecification::ItemTitleFilterSpecification(std::string title)
        : title(title) {}
bool ItemTitleFilterSpecification::is_satisfied(Item const *item) const {
    return check_field_contains(item->get_title(), title);
}

//Filter base on no conditions -> Every item is satisfied
bool ItemAllFilterSpecification::is_satisfied(Item const *item) const {
    return true;
}

//This class uses a spefication class (TitleSpec, StockSpec or all Spec)
//to filter the list of items and include only those which satistfies the spec
std::vector<Item *> ItemFilterer::filter(std::vector<Item *> const &items, ItemFilterSpecification const *spec) {
    std::vector<Item *> result;

    for (auto item : items) {
        if (spec->is_satisfied(item)) {
            result.push_back(item);
        }
    }

    return result;
}

//Aggregated class
//Each attributes: repo, displayer, filterer and persistence
//can be switched out and replaced by another implementation
//to satisfy the Open-Closed principle
ItemService::ItemService(ItemRepository *repo, ItemDisplayer *display, ItemFilterer *filterer,
                         ItemPersistence *persistence) :
        repository(repo), displayer(display), filterer(filterer), persistence(persistence) {}

ItemService::~ItemService() {
    if (repository) {
        delete repository;
    }

    if (displayer) {
        delete displayer;
    }

    if (filterer) {
        delete filterer;
    }

    if (persistence) {
        delete persistence;
    }
}

void ItemService::load() {
    repository->set_items(persistence->load());
}

void ItemService::save() {
    persistence->save(repository->get_items());
}

Item *ItemService::get(std::string const &id) {
    return repository->get_item(id);
}

bool ItemService::check_if_exists(const std::string &id) {
    return get(id) != nullptr;
}

std::vector<Item *> ItemService::get_all() {
    return repository->get_items();
}

void ItemService::add(Item *item) {
    repository->add_item(item);
}

void ItemService::remove(std::string const &id) {
    repository->remove_item(id);
}

void ItemService::update(std::string const &id, ItemModificationIntent &intent) {
    repository->update_item(id, intent);
}

void ItemService::update_genre(std::string const &id, GenredItemModificationIntent &intent) {
    repository->update_genred_item(id, intent);
}

void ItemService::display(ItemOrder const *order) {
    displayer->display(repository->get_items(), order);
}

void ItemService::filter(ItemFilterSpecification const *spec) {
    //Get filtered element
    auto filtered = filterer->filter(repository->get_items(), spec);

    //Check if length is not 0
    if (filtered.size() == 0) {
        std::cout << "Such item does not exist" << std::endl;
        return;
    }

    //Display
    ItemNoOrder order;
    displayer->display(filtered, &order);
}

