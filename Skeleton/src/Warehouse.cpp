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

Customer &WareHouse::getCustomer(int customerId) const{
    for(Customer* customer : customers){
        if(customer->getId() == customerId){
            return *customer;
        }
    }
    //return nullptr;
    //TODO: throw exception
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const{
    return *volunteers[volunteerId];
}

Order &WareHouse::getOrder(int orderId) const{}

const vector<BaseAction*> &WareHouse::getActions() const{
    return actionsLog;
}

vector<Order*> &WareHouse::getPendingOrders() {
    return pendingOrders;
}

void WareHouse::close(){}

void WareHouse::open(){}

vector<Volunteer*> &WareHouse::getVolunteers() {
    return volunteers;
}

void WareHouse::moveOrderToInProcess(Order* order){
    pendingOrders.erase(pendingOrders.begin());
    inProcessOrders.push_back(order);
}

void WareHouse::moveOrderToCompleted(Order* order){
    inProcessOrders.erase(inProcessOrders.begin());
    completedOrders.push_back(order);
}

void WareHouse::moveOrderToInPending(Order* order){
    inProcessOrders.erase(completedOrders.begin());
    pendingOrders.push_back(order);
}

void WareHouse::removeVolunteer(Volunteer* volunteer){
    volunteers.erase(volunteers.begin());
}

int WareHouse::uniqeOrderNum() {
    return pendingOrders.size() + inProcessOrders.size() + completedOrders.size() + 1;
}

int WareHouse::uniqeCustomerId() {
    customerCounter += 1;
    return customerCounter;
}

void WareHouse::AddCustomer(Customer* customer){
    customers.push_back(customer);
}