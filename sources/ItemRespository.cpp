#include "../headers/ItemRespository.h"
#include "../headers/Item.h"
#include <algorithm>

//Modification intents
ItemModificationIntent::ItemModificationIntent(Item* item) : item(item) {}

ItemTitleModificationIntent::ItemTitleModificationIntent(std::string const& title) : title(title) {}
void ItemTitleModificationIntent::modify() {
	item->set_title(title);
}

ItemNumberOfStockIncreaseIntent::ItemNumberOfStockIncreaseIntent(unsigned int value) : value(value){}
void ItemNumberOfStockIncreaseIntent::modify(){
    item->increase_num_in_stock(value);
}

ItemNumberOfStockDecreaseIntent::ItemNumberOfStockDecreaseIntent(unsigned int value) : value(value){}
void ItemNumberOfStockDecreaseIntent::modify(){
    item->decrease_num_in_stock(value);
}


//Repository
InMemoryItemRepository::InMemoryItemRepository(std::vector<Item*> const& items) : items(items) {}
void InMemoryItemRepository::set_items(std::vector<Item*> const& new_items) {
	items = new_items;
}
void InMemoryItemRepository::add_item(Item* item) {
	items.push_back(item);
}
void InMemoryItemRepository::remove_item(std::string const& item_id) {
	//Find the position of the item
	int position = -1;
	for (int i = 0; i != items.size(); ++i) {
		if (item_id == items[i]->get_id()) {
			position = i;
			break;
		}
	}

	//Remove if element exists
	if (position != -1) {
		items.erase(items.begin() + position);
	}
	else {
		std::cerr << "User does not exist" << std::endl;
	}
}
void InMemoryItemRepository::update_item(std::string const& item_id, ItemModificationIntent& intent) {
	//Find the position of the item
	int position = -1;
	for (int i = 0; i != items.size(); ++i) {
		if (item_id == items[i]->get_id()) {
			position = i;
			break;
		}
	}

	//Update if element exists
	if (position != -1) {
		intent.set_item(items[position]);
		intent.modify();
	}
	else {
		std::cerr << "User does not exist" << std::endl;
	}
}


//Persistence
std::vector<Item*> MockItemPersistence::load() {
	return {};
}
void MockItemPersistence::save(std::vector<Item*>) {
	//Do nothing
}

//Displayer
void ItemTitleOrder::order(std::vector<Item*>& items) const {
	std::sort(items.begin(), items.end(), [](Item const* a, Item const* b) {
		return a->get_title() < b->get_title();
	});
}

void ItemIdOrder::order(std::vector<Item*>& items) const {
	std::sort(items.begin(), items.end(), [](Item const* a, Item const* b) {
		return a->get_id() < b->get_id();
	});
}

void ItemNoOrder::order(std::vector<Item*>& items) const {
	//Do nothing
}

void ConsoleItemDisplayer::display(std::vector<Item*> items, ItemOrder const* order) {
	//Sort first
	order->order(items);
	for (auto& item : items) {
		std::cout << *item << std::endl;
	}
}

//Filters
ItemRentalStatusFilterSpecification::ItemRentalStatusFilterSpecification(Item::RentalStatus const rental_status) : rental_status(rental_status) {}

bool ItemRentalStatusFilterSpecification::is_satisfied(Item const* item) const {
	return item->get_rental_status() == rental_status;
}

bool ItemAllFilterSpecification::is_satisfied(Item const* item) const {
	return true;
}

std::vector<Item*> ItemFilterer::filter(std::vector<Item*> const& items, ItemFilterSpecification const* spec) {
	std::vector<Item*> result;

	for (auto item : items) {
		if (spec->is_satisfied(item)) {
			result.push_back(item);
		}
	}

	return result;
}

//Item service
ItemService::ItemService(ItemRepository* repo, ItemDisplayer* display, ItemFilterer* filterer, ItemPersistence* persistence) :
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

void ItemService::add(Item* item) {
	repository->add_item(item);
}

void ItemService::remove(std::string const& id) {
	repository->remove_item(id);
}

void ItemService::update(std::string const& id, ItemModificationIntent& intent) {
	repository->update_item(id, intent);
}

void ItemService::display(ItemOrder const* order) {
	displayer->display(repository->get_items(), order);
}

void ItemService::filter(ItemFilterSpecification const* spec) {
	//Get filtered element
	auto filtered = filterer->filter(repository->get_items(), spec);
	//Display
	ItemNoOrder order;
	displayer->display(filtered, &order);
}

