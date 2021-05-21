cmake_minimum_required(VERSION 3.17)
project(cpp_renting_console_app)

set(CMAKE_CXX_STANDARD 11)

add_executable(cpp_renting_console_app main.cpp headers/Customer.h headers/CustomerRepository.h headers/Item.h headers/ItemRepository.h headers/Menu.h sources/Customer.cpp sources/CustomerRepository.cpp sources/Item.cpp sources/ItemRepository.cpp sources/Menu.cpp sources/ItemHelpers.cpp headers/ItemHelpers.h headers/ServiceBuilder.h sources/ServiceBuilder.cpp headers/CustomerHelpers.h sources/CustomerHelpers.cpp headers/StringHelper.h sources/StringHelper.cpp)