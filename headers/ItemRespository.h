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
	ItemTitleModificationIntent(std::string const& name);
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
    virtual std::vector<Item*> get_items() = 0;
	virtual void set_items(std::vector<Item*> const&) = 0;
};

struct InMemoryItemRepository : public ItemRepository {
	std::vector<Item*> items;
public:
	InMemoryItemRepository() = default;
	InMemoryItemRepository(std::vector<Item*> const& items);
    std::vector<Item*> get_items() override { return items; }
	void set_items(std::vector<Item*> const& items) override;
	void add_item(Item* item) override;
	void remove_item(std::string const& item_id) override;
	void update_item(std::string const& item_id, ItemModificationIntent& intent) override;
};

//Persistence
struct ItemPersistence {
	virtual std::vector<Item*> load() = 0;
	virtual void save(std::vector<Item*>) = 0;
};

struct MockItemPersistence : public ItemPersistence {
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

struct ItemRentalStatusFilterSpecification : public ItemFilterSpecification {
	Item::RentalStatus rental_status;

	ItemRentalStatusFilterSpecification(Item::RentalStatus const rental_status);
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
	void add(Item* item);
	void remove(std::string const& id);
	void update(std::string const& id, ItemModificationIntent& intent);
	void display(ItemOrder const* order);
	void filter(ItemFilterSpecification const* spec);
};