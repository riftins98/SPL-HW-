#include "WareHouse.h"
#include <iostream>

WareHouse::WareHouse(const string &configFilePath){
    isOpen = true;
    customerCounter = 0;
    volunteerCounter = 0;
    start();
} 

void WareHouse::start(){
    std::cout << "Warehouse is open!" << std::endl;
    }

void WareHouse::addOrder(Order* order){}

void WareHouse::addAction(BaseAction* action){}

Customer &WareHouse::getCustomer(int customerId) const{}

Volunteer &WareHouse::getVolunteer(int volunteerId) const{}

Order &WareHouse::getOrder(int orderId) const{}

const vector<BaseAction*> &WareHouse::getActions() const{}

const vector<Order*> &WareHouse::getOrders() const{
    return &orders;
}

void WareHouse::close(){}

void WareHouse::open(){}



<vector*<Volunteer*> WareHouse::getVolunteers() const{
    return &volunteers;
}