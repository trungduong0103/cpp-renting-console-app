#pragma once
#include "Item.h"
#include <iostream>
#include <vector>

//Repository
struct ItemModificationIntent {
protected:
    Item* item;
public:
    ItemModificationIntent() = default;
    ItemModificationIntent(Item*);
    inline void set_item(Item* ite) { item = ite; }
    virtual void modify() = 0;
};

struct ItemTitleModificationIntent : public ItemModificationIntent {
    std::string title;
    ItemTitleModificationIntent() = default;
    ItemTitleModificationIntent(std::string  title);
    void modify() override;
};

struct ItemRentalTypeModificationIntent : public ItemModificationIntent {
    Item::RentalType rental_type;
    ItemRentalTypeModificationIntent() = default;
    ItemRentalTypeModificationIntent(Item::RentalType rental_type);
    void modify() override;
};

struct ItemFeeModificationIntent : public ItemModificationIntent {
    float fee;
    ItemFeeModificationIntent() = default;
    ItemFeeModificationIntent(float fee);
    void modify() override;
};

struct ItemNumStockModificationIntent : public ItemModificationIntent {
    unsigned int number_in_stock;
    ItemNumStockModificationIntent() = default;
    ItemNumStockModificationIntent(unsigned int number_in_stock);
    void modify() override;
};


struct ItemNumberOfStockIncreaseIntent : public ItemModificationIntent {
    unsigned int value = 1;
    ItemNumberOfStockIncreaseIntent() = default;
    ItemNumberOfStockIncreaseIntent(unsigned int const value);
    void modify() override;
};

struct ItemNumberOfStockDecreaseIntent : public ItemModificationIntent {
    unsigned int value = 1;
    ItemNumberOfStockDecreaseIntent() = default;
    ItemNumberOfStockDecreaseIntent(unsigned int const value);
    void modify() override;
};

struct ItemRepository {
    virtual void add_item(Item* item) = 0;
    virtual void remove_item(std::string const& item_id) = 0;
    virtual void update_item(std::string const& item_id, ItemModificationIntent& intent) = 0;
    virtual Item* get_item(std::string const& id) = 0;
    virtual std::vector<Item*> get_items() = 0;
    virtual void set_items(std::vector<Item*> const&) = 0;
};

// TODO:
//  If an item is currently borrowed, a delete action with that item will fail with an error message
//  A customer can only be removed from the system once he/she returned all borrowed items

struct InMemoryItemRepository : public ItemRepository {
    std::vector<Item*> items;
    unsigned int starting_index = 0;
public:
    InMemoryItemRepository() = default;
    ~InMemoryItemRepository();
    InMemoryItemRepository(std::vector<Item*>  items);
    unsigned int get_starting_index() { return starting_index; }
    void increment_starting_index() { this->starting_index++; }
    void decrement_starting_index() { this->starting_index--; }
    std::vector<Item*> get_items() override { return items; }
    void set_items(std::vector<Item*> const& items) override;
    void add_item(Item* item) override;
    void remove_item(std::string const& item_id) override;
    void update_item(std::string const& item_id, ItemModificationIntent& intent) override;
    Item* get_item(std::string const& id) override;
    int get_item_index(std::string const &item_id);
};

//Persistence
struct ItemPersistence {
    virtual std::vector<Item*> load() = 0;
    virtual void save(std::vector<Item*>) = 0;
};

struct TextFileItemPersistence : public ItemPersistence {
    std::vector<Item*> load() override;
    void save(std::vector<Item*>) override;
};

//Displayer
struct ItemOrder {
    virtual void order(std::vector<Item*>& items) const = 0;
};

struct ItemNoOrder : public ItemOrder {
    void order(std::vector<Item*>& items) const override;
};

struct ItemTitleOrder : public ItemOrder {
    void order(std::vector<Item*>& items) const override;
};

struct ItemIdOrder : public ItemOrder {
    void order(std::vector<Item*>& items) const override;
};

struct ItemDisplayer {
    virtual void display(std::vector<Item*> items, ItemOrder const* order) = 0;
};

struct ConsoleItemDisplayer : public ItemDisplayer {
    void display(std::vector<Item*> items, ItemOrder const* order) override;
};

//Filterer
struct ItemFilterSpecification {
    virtual bool is_satisfied(Item const* item) const = 0;
};

struct ItemNumStockFilterSpecification : public ItemFilterSpecification {
    int number_in_stock;

    ItemNumStockFilterSpecification(unsigned int const number_in_stock);
    bool is_satisfied(Item const* item) const override;
};

struct ItemIdFilterSpecification : public ItemFilterSpecification {
    std::string id;

    ItemIdFilterSpecification(std::string);
    bool is_satisfied(Item const* item) const override;
};

struct ItemTitleFilterSpecification : public ItemFilterSpecification {
    std::string title;

    ItemTitleFilterSpecification(std::string title);
    bool is_satisfied(Item const* item) const override;
};

struct ItemAllFilterSpecification : public ItemFilterSpecification {
    ItemAllFilterSpecification() = default;
    bool is_satisfied(Item const* item) const override;
};

struct ItemFilterer {
    std::vector<Item*> filter(std::vector<Item*> const& items, ItemFilterSpecification const*);
};

//Aggregated class
class ItemService {
    ItemRepository* repository;
    ItemDisplayer* displayer;
    ItemFilterer* filterer;
    ItemPersistence* persistence;

public:
    //Destruct and construct
    ItemService(ItemRepository* repo, ItemDisplayer* display, ItemFilterer* filterer, ItemPersistence* persistence);
    ~ItemService();

    //Methods
    void load();
    void save();
    Item* get(std::string const&);
    bool check_if_exists(std::string const&);
    std::vector<Item*> get_all();
    void add(Item* item);
    void remove(std::string const& id);
    void update(std::string const& id, ItemModificationIntent& intent);
    void display(ItemOrder const* order);
    void filter(ItemFilterSpecification const* spec);
};