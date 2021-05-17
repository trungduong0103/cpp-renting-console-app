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
    ItemTitleModificationIntent(std::string  name);
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
<<<<<<< HEAD
    virtual void add_item(Item* item) = 0;
    virtual void remove_item(std::string const& item_id) = 0;
    virtual void update_item(std::string const& item_id, ItemModificationIntent& intent) = 0;
    virtual Item* get_item(std::string const& id) = 0;
    virtual std::vector<Item*> get_items() = 0;
    virtual void set_items(std::vector<Item*> const&) = 0;
=======
	virtual void add_item(Item* item) = 0;
	virtual void remove_item(std::string const& item_id) = 0;
	virtual void update_item(std::string const& item_id, ItemModificationIntent& intent) = 0;
  virtual Item* get_item(std::string const& id) = 0;
	virtual std::vector<Item*> get_items() = 0;
	virtual void set_items(std::vector<Item*> const&) = 0;
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
};

// TODO:
//  If an item is currently borrowed, a delete action with that item will fail with an error message
//  A customer can only be removed from the system once he/she returned all borrowed items

struct InMemoryItemRepository : public ItemRepository {
    std::vector<Item*> items;
    unsigned int starting_index = 0;
public:
<<<<<<< HEAD
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
=======
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
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
};

//Persistence
struct ItemPersistence {
    virtual std::vector<Item*> load() = 0;
    virtual void save(std::vector<Item*>) = 0;
};

struct MockItemPersistence : public ItemPersistence {
<<<<<<< HEAD
    std::vector<Item*> load() override;
    void save(std::vector<Item*>) override;
};

struct TextFileItemPersistence : public ItemPersistence {
    std::vector<Item*> load() override;
    void save(std::vector<Item*>) override;
=======
	std::vector<Item*> load() override;
	void save(std::vector<Item*>) override;
};

struct TextFileItemPersistence : public ItemPersistence {
	std::vector<Item*> load() override;
	void save(std::vector<Item*>) override;
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
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

struct ItemAllFilterSpecification : public ItemFilterSpecification {
    ItemAllFilterSpecification() = default;
    bool is_satisfied(Item const* item) const override;
};

struct ItemFilterer {
<<<<<<< HEAD
    std::vector<Item*> filter(std::vector<Item*> const& items, ItemFilterSpecification const*);
=======
	std::vector<Item*> filter(std::vector<Item*> const& items, ItemFilterSpecification const*);
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
};

//Aggregated class
class ItemService {
    ItemRepository* repository;
    ItemDisplayer* displayer;
    ItemFilterer* filterer;
    ItemPersistence* persistence;

public:
<<<<<<< HEAD
    //Destruct and construct
    ItemService(ItemRepository* repo, ItemDisplayer* display, ItemFilterer* filterer, ItemPersistence* persistence);
    ~ItemService();

    //Methods
    void load();
    void save();
    Item* get(std::string const&);
    void add(Item* item);
    void remove(std::string const& id);
    void update(std::string const& id, ItemModificationIntent& intent);
    void display(ItemOrder const* order);
    void filter(ItemFilterSpecification const* spec);
=======
	//Destruct and construct
	ItemService(ItemRepository* repo, ItemDisplayer* display, ItemFilterer* filterer, ItemPersistence* persistence);
	~ItemService();

	//Methods
	void load();
	void save();
	Item* get(std::string const&);
	void add(Item* item);
	void remove(std::string const& id);
	void update(std::string const& id, ItemModificationIntent& intent);
	void display(ItemOrder const* order);
	void filter(ItemFilterSpecification const* spec);
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
};