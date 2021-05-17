#include "../headers/ItemRepository.h"
#include "../headers/ItemHelpers.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

//Modification intents
ItemModificationIntent::ItemModificationIntent(Item *item) : item(item) {}

ItemTitleModificationIntent::ItemTitleModificationIntent(std::string title) : title(std::move(title)) {}

void ItemTitleModificationIntent::modify() {
    item->set_title(title);
}

ItemRentalTypeModificationIntent::ItemRentalTypeModificationIntent(Item::RentalType rental_type) : rental_type(rental_type) {}

void ItemRentalTypeModificationIntent::modify() {
    item->set_rental_type(rental_type);
}

ItemFeeModificationIntent::ItemFeeModificationIntent(float fee) : fee(fee) {}

void ItemFeeModificationIntent::modify() {
    item->set_rental_fee(fee);
}

ItemNumberOfStockIncreaseIntent::ItemNumberOfStockIncreaseIntent(unsigned int value) : value(value) {}

void ItemNumberOfStockIncreaseIntent::modify() {
    item->increase_num_in_stock(value);
}

ItemNumberOfStockDecreaseIntent::ItemNumberOfStockDecreaseIntent(unsigned int value) : value(value) {}

void ItemNumberOfStockDecreaseIntent::modify() {
    item->decrease_num_in_stock(value);
}

//Repository
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
    }
    else {
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
    }
    else {
        std::cerr << "User does not exist" << std::endl;
    }
}

Item* InMemoryItemRepository::get_item(std::string const& id) {
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
        remove_whitespace(token);
        item_as_vector.push_back(token);
        line.erase(0, pos + delimiter.length());
    }

    remove_whitespace(line);
    item_as_vector.push_back(line.substr(0, line.length()));

    return item_as_vector;
}
// TODO: customer file is incorrect in C003 & C004

//Text file persistence
// TODO: combine validations into a single function
std::vector<Item *> TextFileItemPersistence::load() {
    std::ifstream infile("../textfiles/items.txt");
    if (!infile) {
        std::cerr << "Cannot read file items.txt" << std::endl;
        return {};
    }
    unsigned int count = 1;
    std::vector<Item *> mockItems;
    std::string line;
    while (std::getline(infile, line)) {
        std::cout << "LINE " << "[" << count << "]: " << line << std::endl;
        if (line[0] == '#') {
            std::cout << "Ignoring line " << count << " (has # in the beginning)" << std::endl;
        } else if (line.empty()) {
            std::cout << "Ignoring line " << count << " (line is empty)." << std::endl;
        } else if (!correct_info_length(line)) {
            std::cout << "Ignoring line " << count << " (line can only have 5-6 commas)." << std::endl;
        } else {
            std::vector<std::string> item_vector = get_item_as_vector(line);
            if (item_vector.empty()) {
                std::cout << "Ignoring line " << count << " (line is missing info)." << std::endl;
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
                    std::cout << "Line is OK" << std::endl;
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
    infile.close();
    return mockItems;
}

void TextFileItemPersistence::save(std::vector<Item *> items) {
    std::ofstream outfile("../outfiles/items_out.txt", std::ios::trunc);
    if (!outfile) {
        std::cerr << "Cannot read file items.txt" << std::endl;
        return ;
    }
    unsigned int i = 0;
    for (; i < items.size(); i++) {
        outfile << items[i]->to_string_file();
        if (i < items.size()) {
            outfile << "\n";
        }
    }
    outfile.close();
}

//Displayer
void ItemTitleOrder::order(std::vector<Item *> &items) const {
    std::sort(items.begin(), items.end(), [](Item const *a, Item const *b) {
        return a->get_title() < b->get_title();
    });
}

void ItemIdOrder::order(std::vector<Item *> &items) const {
    std::sort(items.begin(), items.end(), [](Item const *a, Item const *b) {
        return a->get_id() < b->get_id();
    });
}

void ItemNoOrder::order(std::vector<Item *> &items) const {
    //Do nothing
}

void ConsoleItemDisplayer::display(std::vector<Item *> items, ItemOrder const *order) {
    //Sort first
    order->order(items);
    for (auto &item : items) {
        std::cout << *item << std::endl;
    }
}

//Filters
ItemNumStockFilterSpecification::ItemNumStockFilterSpecification(unsigned int const number_in_stock)
        : number_in_stock(number_in_stock) {}

bool ItemNumStockFilterSpecification::is_satisfied(Item const *item) const {
    return item->get_number_in_stock() == number_in_stock;
}

bool ItemAllFilterSpecification::is_satisfied(Item const *item) const {
    return true;
}

std::vector<Item *> ItemFilterer::filter(std::vector<Item *> const &items, ItemFilterSpecification const *spec) {
    std::vector<Item *> result;

    for (auto item : items) {
        if (spec->is_satisfied(item)) {
            result.push_back(item);
        }
    }

    return result;
}

//Item service
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

Item* ItemService::get(std::string const& id) {
    return repository->get_item(id);
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

void ItemService::display(ItemOrder const *order) {
    displayer->display(repository->get_items(), order);
}

void ItemService::filter(ItemFilterSpecification const *spec) {
    //Get filtered element
    auto filtered = filterer->filter(repository->get_items(), spec);
    //Display
    ItemNoOrder order;
    displayer->display(filtered, &order);
}

