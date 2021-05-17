#include "../headers/ServiceBuilder.h"
<<<<<<< HEAD
=======
#include "../headers/CustomerRepository.h"
>>>>>>> d4656bbd62471ec1cedaa24d04e05cc2c2f5be20
#include "../headers/ItemRepository.h"


CustomerService* StandardCustomerServiceBuilder::create() {
    //Create repo
    CustomerRepository* repo = new InMemoryCustomerRepository();

    //Create diplay
    CustomerDisplayer* displayer = new ConsoleCustomerDisplayer();

    //Create filterer
    CustomerFilterer* filterer = new CustomerFilterer();

    //Create persistence
    CustomerPersistence* persistence = new MockCustomerPersistence();

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