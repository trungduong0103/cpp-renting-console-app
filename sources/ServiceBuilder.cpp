#include "../headers/ServiceBuilder.h"
#include "../headers/ItemRepository.h"


CustomerService* StandardCustomerServiceBuilder::create() {
    //Create repo
    CustomerRepository* repo = new InMemoryCustomerRepository();

    //Create diplay
    CustomerDisplayer* displayer = new ConsoleCustomerDisplayer();

    //Create filterer
    CustomerFilterer* filterer = new CustomerFilterer();

    //Create persistence
    CustomerPersistence* persistence = new TextFileCustomerPersistence();

    //Create customer service
    CustomerService* service = new CustomerService(repo, displayer, filterer, persistence);
    return service;
}

ItemService* StandardItemServiceBuilder::create() {
    //Create repo
    ItemRepository* repo = new InMemoryItemRepository();

    //Create diplay
    ItemDisplayer* displayer = new ConsoleItemDisplayer();

    //Create filterer
    ItemFilterer* filterer = new ItemFilterer();

    //Create persistence
    ItemPersistence* persistence = new TextFileItemPersistence();

    //Create customer service
    ItemService* service = new ItemService(repo, displayer, filterer, persistence);
    return service;
}