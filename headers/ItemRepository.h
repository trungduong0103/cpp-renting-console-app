#pragma once
#include "Item.h"
#include "StringHelper.h"
#include <iostream>
#include <vector>

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
struct ItemModificationIntent {
protected:
    Item* item;
public:
    ItemModificationIntent() = default;
    ItemModificationIntent(Item*);
    inline void set_item(Item* ite) { item = ite; }
    virtual void modify() = 0;
};

//Child of Modification intent
//Used when we want to change an item's title
struct ItemTitleModificationIntent : public ItemModificationIntent {
    std::string title;
    ItemTitleModificationIntent() = default;
    ItemTitleModificationIntent(std::string  title);
    void modify() override;
};

//Child of Modification intent
//Used when we want to change an item's stock
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

//Child of Modification intent
//Used when we want to change an item's stock
struct ItemNumStockModificationIntent : public ItemModificationIntent {
    unsigned int number_in_stock;
    ItemNumStockModificationIntent() = default;
    ItemNumStockModificationIntent(unsigned int number_in_stock);
    void modify() override;
};

//Child of Modification intent
//Used when we want to change an item's stock
struct ItemNumberOfStockIncreaseIntent : public ItemModificationIntent {
    unsigned int value = 1;
    ItemNumberOfStockIncreaseIntent() = default;
    ItemNumberOfStockIncreaseIntent(unsigned int const value);
    void modify() override;
};

//Child of Modification intent
//Used when we want to change an item's stock
struct ItemNumberOfStockDecreaseIntent : public ItemModificationIntent {
    unsigned int value = 1;
    ItemNumberOfStockDecreaseIntent() = default;
    ItemNumberOfStockDecreaseIntent(unsigned int const value);
    void modify() override;
};

//Intent pattern
//This intent is used for updating a user attributes using the
//repository update method
struct GenredItemModificationIntent {
protected:
    GenredItem* genred_item;
public:
    GenredItemModificationIntent() = default;
    GenredItemModificationIntent(GenredItem*);
    inline void set_item(GenredItem* gen_ite) { genred_item = gen_ite; }
    virtual void modify() = 0;
};

//Child of Modification intent
//Used when we want to change an genre of genred item
struct GenredItemGenreModificationIntent : public GenredItemModificationIntent {
    GenredItem::Genre genre;
    GenredItemGenreModificationIntent() = default;
    GenredItemGenreModificationIntent(GenredItem::Genre genre);
    void modify() override;
};

//A blue print of repository pattern
//containing methods such as CRUD of customers
struct ItemRepository {
    virtual void add_item(Item* item) = 0;
    virtual void remove_item(std::string const& item_id) = 0;
    virtual void update_item(std::string const& item_id, ItemModificationIntent& intent) = 0;
    virtual void update_genred_item(std::string const& item_id, GenredItemModificationIntent& intent) = 0;
    virtual Item* get_item(std::string const& id) = 0;
    virtual std::vector<Item*> get_items() = 0;
    virtual void set_items(std::vector<Item*> const&) = 0;
};

// TODO:
//  If an item is currently borrowed, a delete action with that item will fail with an error message
//  A customer can only be removed from the system once he/she returned all borrowed items

//Implementation of Repository pattern
//Where all CRUD operation will be done using an in-memory vector
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
    void update_genred_item(std::string const& item_id, GenredItemModificationIntent& intent) override;
    Item* get_item(std::string const& id) override;
    int get_item_index(std::string const &item_id);
};

//Blueprint for Item persistence
//Containing two methods: load() for loading item data
//save() for saving item data
struct ItemPersistence {
    virtual std::vector<Item*> load() = 0;
    virtual void save(std::vector<Item*>) = 0;
};

//Implementation of ItemPersistence
//This is responsible for loading and saving
//customers from and to a text file
struct TextFileItemPersistence : public ItemPersistence {
    std::vector<Item*> load() override;
    void save(std::vector<Item*>) override;
};

//Order classes
//These classes are responsible for
//Sorting an items list based on their attributes
struct ItemOrder {
    virtual void order(std::vector<Item*>& items) const = 0;
};

//No order: This class will do nothing
struct ItemNoOrder : public ItemOrder {
    void order(std::vector<Item*>& items) const override;
};

//Order by name: This class will sort the items based on their titles
struct ItemTitleOrder : public ItemOrder {
    void order(std::vector<Item*>& items) const override;
};

//Order by id: This class will sort the items based on their ids
struct ItemIdOrder : public ItemOrder {
    void order(std::vector<Item*>& items) const override;
};

//Blueprint for CustomerDisplayer
//Which is used to display customer through an interface
struct ItemDisplayer {
    virtual void display(std::vector<Item*> items, ItemOrder const* order) = 0;
};

//Implementation of ItemDisplayer
//Responsible for displaying the items through the console
struct ConsoleItemDisplayer : public ItemDisplayer {
    void display(std::vector<Item*> items, ItemOrder const* order) override;
};


//Specification Design Pattern for filtering
//Each speficification class will check if an item satisfies a credential for filtering
struct ItemFilterSpecification {
    virtual bool is_satisfied(Item const* item) const = 0;
};

//Filter item based on their number of stock
struct ItemNumStockFilterSpecification : public ItemFilterSpecification {
    int number_in_stock;

    ItemNumStockFilterSpecification(unsigned int const number_in_stock);
    bool is_satisfied(Item const* item) const override;
};

//Filter item based on their number of stock
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

//Filter base on no conditions -> Every item is satisfied
struct ItemAllFilterSpecification : public ItemFilterSpecification {
    ItemAllFilterSpecification() = default;
    bool is_satisfied(Item const* item) const override;
};

//This class uses a spefication class (TitleSpec, StockSpec or all Spec)
//to filter the list of items and include only those which satistfies the spec
struct ItemFilterer {
    std::vector<Item*> filter(std::vector<Item*> const& items, ItemFilterSpecification const*);
};

//Aggregated class
//Each attributes: repo, displayer, filterer and persistence
//can be switched out and replaced by another implementation
//to satisfy the Open-Closed principle
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
    void update_genre(std::string const& id, GenredItemModificationIntent& intent);
    void display(ItemOrder const* order);
    void filter(ItemFilterSpecification const* spec);
};