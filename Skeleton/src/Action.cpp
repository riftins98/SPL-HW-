#include "Action.h"     
#include <iostream>

BaseAction::BaseAction(){}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg){
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const{
    return errorMsg;
}
//---------------------------------------------
SimulateStep::SimulateStep(int numOfSteps): numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse) {
    for (Order order: wareHouse.pendingOrders)
    {
        OrderStatus status = order->getStatus();
        
        for (Volunteer volenteer : wareHouse.get)
        {
            /* code */
        }
        
        
        bool availableVolunteers = false;
        int i= 0;
        while(availableVolunteers==false){
            if(wareHouse.volunteers[i]->canTakeOrder(order)){
                availableVolunteers = true;
                wareHouse.volunteers[i]->takeOrder(order);
            }
            i++;
        } 
    }
    
}

std::string SimulateStep::toString() const {
    return "Simulate Step";
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

//---------------------------------------------
AddOrder::AddOrder(int id): id(id) {}

void AddOrder::act(WareHouse &wareHouse) {
    wareHouse.addOrder(new Order(id));
}

std::string AddOrder::toString() const {
    return "Add Order";
}

AddOrder *AddOrder::clone() const {
    return new AddOrder(*this);
}

//---------------------------------------------
AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders): customerName(customerName), customerType(customerType), distance(distance), maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse) {}

std::string AddCustomer::toString() const {
    return "Add Customer";
}

AddCustomer *AddCustomer::clone() const {
    return new AddCustomer(*this);
}

//---------------------------------------------
PrintOrderStatus::PrintOrderStatus(int id): id(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse) {}

std::string PrintOrderStatus::toString() const {
    return "Print Order Status";
}

PrintOrderStatus *PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

//---------------------------------------------
PrintCustomerStatus::PrintCustomerStatus(int customerId): customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse) {}

std::string PrintCustomerStatus::toString() const {
    return "Print Customer Status";
}

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

//---------------------------------------------
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse) {}

std::string PrintActionsLog::toString() const {
    return "Print Actions Log";
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

//---------------------------------------------
Close::Close() {}

void Close::act(WareHouse &wareHouse) {}

std::string Close::toString() const {
    return "Close";
}

Close *Close::clone() const {
    return new Close(*this);
}

//---------------------------------------------
BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse) {}

std::string BackupWareHouse::toString() const {
    return "Backup WareHouse";
}

BackupWareHouse *BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

//---------------------------------------------
RestoreWareHouse::RestoreWareHouse() {}//RESTORE WAREHOUSE

void RestoreWareHouse::act(WareHouse &wareHouse) {}

std::string RestoreWareHouse::toString() const {
    return "Restore WareHouse";
}

RestoreWareHouse *RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

