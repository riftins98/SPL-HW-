#include "Order.h"
#include <iostream>

Order::Order(int id, int customerId, int distance): id(id), customerId(customerId),distance(distance) {
    status = OrderStatus::PENDING;
    collectorId = -1;
    driverId = -1;
}

int Order::getId() const{
    return id;
}

int Order::getDistance() const{
    return distance;
}

int Order::getCustomerId() const{
    return customerId;
}

void Order::setStatus(OrderStatus status){
    this->status = status;
}       

void Order::setCollectorId(int collectorId){
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId){
    this->driverId = driverId;
}

int Order::getCollectorId() const{
    return collectorId;
}

int Order::getDriverId() const{
    return driverId;
}

OrderStatus Order::getStatus() const{
    return status;
}

const string Order::toString() const{
    string statusString;
    switch (status)
    {
    case OrderStatus::PENDING:
        statusString = "Pending";
        break;
    case OrderStatus::COLLECTING:
        statusString = "Collecting";
        break;
    case OrderStatus::DELIVERING:
        statusString = "Delivering";
        break;
    case OrderStatus::COMPLETED:
        statusString = "Completed";
        break;
    default:
        break;
    }
    return "Order " + std::to_string(id) + ": Customer " + std::to_string(customerId) + " Distance: " + std::to_string(distance) + " Status: " + statusString;
}



