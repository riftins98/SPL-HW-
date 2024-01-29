#include "Action.h"
#include "Volunteer.h"
#include <iostream>

BaseAction::BaseAction() {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

BaseAction::~BaseAction() {
    delete this;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

string BaseAction::statusToString(ActionStatus status)
{
   
    if (status == ActionStatus::COMPLETED)
    {
        return "COMPLETED";
    }
    else if (status == ActionStatus::ERROR)
    {
        return "ERROR";
    }
}

//--------------------------------SimulateStep-------------
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        for (Order *order : wareHouse.getPendingOrders())
        {
            for (Volunteer *volenteer : wareHouse.getVolunteers())
            {
                if (volenteer->canTakeOrder(*order))
                {
                    volenteer->acceptOrder(*order);
                    if (order->getStatus() == OrderStatus::PENDING)
                    {
                        order->setCollectorId(volenteer->getId());
                    }
                    else if (order->getStatus() == OrderStatus::COLLECTING)
                    {
                        order->setDriverId(volenteer->getId());
                    }
                    wareHouse.moveOrderToInProcess(order);

                    break;
                }
            }
        }
        for (Volunteer *volenteer : wareHouse.getVolunteers())
        {
            volenteer->step();
            int orderId = volenteer->getCompletedOrderId();
            if (!volenteer->isBusy() && orderId != -1)
            {
                Order *order = &wareHouse.getOrder(orderId);
                if (order->getStatus() == OrderStatus::COLLECTING)
                {
                    wareHouse.moveOrderToInPending(order);
                }
                else if (order->getStatus() == OrderStatus::DELIVERING)
                {
                    wareHouse.moveOrderToCompleted(order);
                }
            }
            if (!volenteer->hasOrdersLeft())
            {
                wareHouse.removeVolunteer(volenteer);
            }
        }
    }
    wareHouse.addAction(this);
    complete();
}
std::string SimulateStep::toString() const
{
    return std::string("simulateStep ") + std::to_string(numOfSteps);
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

//---------------------------------------------
AddOrder::AddOrder(const int customerId) : customerId(customerId), orderId(-1) {}

void AddOrder::act(WareHouse &wareHouse)
{
    int id = wareHouse.uniqeOrderNum();
    orderId = id;
    if (customerId > wareHouse.getCustomerCounter() || customerId <= 0)
    {
        error("Cannot place this order");
        std::cout << "Error: " <<getErrorMsg() << std::endl;
    }
    else
    {
        Customer *customer = &wareHouse.getCustomer(customerId);
        if (!(customer->canMakeOrder()))
        {
            error("Cannot place this order");
            std::cout << "Error: "<< getErrorMsg() << std::endl;
        }
        else
        {
            Order *order = new Order(id, customerId, customer->getCustomerDistance());
            wareHouse.addOrder(order);
            wareHouse.addToAllOrders(order);
            customer->addOrder(id);
            complete();
        }
        wareHouse.addAction(this);
    }
}

std::string AddOrder::toString() const
{
    return std::string("order ") + std::to_string(customerId);
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

//---------------------------------------------
AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : customerName(customerName), customerType(typeFromString(customerType)), distance(distance), maxOrders(maxOrders), customerId(-1) {}

CustomerType AddCustomer::typeFromString(string customerType)
{
    if (customerType == "Soldier")
    {
        return CustomerType::Soldier;
    }
    else if (customerType == "Civilian")
    {
        return CustomerType::Civilian;
    }
}

void AddCustomer::act(WareHouse &wareHouse)
{
    Customer *customer;
    customerId = wareHouse.uniqeCustomerId();
    if (customerType == CustomerType::Soldier)
    {
        customer = new SoldierCustomer(customerId , customerName, distance, maxOrders);
    }
    else if (customerType == CustomerType::Civilian)
    {
        customer = new CivilianCustomer(customerId, customerName, distance, maxOrders);
    }
    wareHouse.AddCustomer(customer);
    complete();
}

std::string AddCustomer::toString() const
{
    return std::string("customer ") + std::to_string(customerId);
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

//---------------------------------------------
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    if (orderId > wareHouse.getOrderCounter() || orderId <= 0)
    {
        error("Order does not exist");
        std::cout << "Error: " <<getErrorMsg() << std::endl;
    }
    else
    {
        Order *order = &wareHouse.getOrder(orderId);
        std::cout << order->toString() << std::endl;
        wareHouse.addAction(this);
        complete();
    }
}

std::string PrintOrderStatus::toString() const
{
    // להחזור סטאטןס ושורה
    return std::string("orderStatus ") + std::to_string(orderId);
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

//---------------------------------------------
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    if (customerId > wareHouse.getCustomerCounter() || customerId <= 0)
    {
        error("Customer does not exist");
        std::cout << "Error: " <<getErrorMsg() << std::endl;
    }
    else{
        Customer *customer = &wareHouse.getCustomer(customerId);
        std::cout << "customer" << customerId << std::endl;
        const vector<int> &ordersIds = customer->getOrdersIds();
        for (size_t i = 0; i < ordersIds.size(); i++)
            {
            Order *order = &wareHouse.getOrder(ordersIds[i]);
            std::cout << "OrderId: " << order->getId() << std::endl;
            std::cout << "OrderStatus: " << order->getStatusString() << std::endl;
        }
        std::cout << "numOrdersLeft: " << customer->getMaxOrders() - customer->getNumOrders() << std::endl;
        wareHouse.addAction(this);
        complete();
    }
    
}

std::string PrintCustomerStatus::toString() const
{
    return  std::string("customerStatus ") + std::to_string(customerId);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}
//---------------------------------------------

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    if (volunteerId > wareHouse.getVolunteerCounter() || volunteerId <= 0)
    {
        error("Volunteer does not exist");
        std::cout << "Error: " <<getErrorMsg() << std::endl;

    }
    else
    {
        Volunteer *volunteer = &wareHouse.getVolunteer(volunteerId);
        std::cout << volunteer->toString() << std::endl;
        wareHouse.addAction(this);
        complete();
    }
}

std::string PrintVolunteerStatus::toString() const
{
    return  std::string("volunteerStatus ") + std::to_string(volunteerId);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

//---------------------------------------------
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    complete();
    wareHouse.addAction(this);
    for (BaseAction *action : wareHouse.getActions())
    {
        std::cout << action->toString() << " " <<statusToString(action->getStatus()) << std::endl;
    }
    wareHouse.addAction(this);
  
}

std::string PrintActionsLog::toString() const
{
    return  std::string("log ") ;
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

//---------------------------------------------
Close::Close(){}

void Close::act(WareHouse &wareHouse) {
    wareHouse.addAction(this);
    wareHouse.close();
    complete();
}

std::string Close::toString() const
{
    return "close";
}

Close *Close::clone() const
{
    return new Close(*this);
}

//---------------------------------------------
BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse) {
    extern WareHouse* backup;
    backup = &wareHouse;

    wareHouse.addAction(this);
    complete();
}

std::string BackupWareHouse::toString() const
{
    return "backupWareHouse";
}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

//---------------------------------------------
RestoreWareHouse::RestoreWareHouse() {} // RESTORE WAREHOUSE

void RestoreWareHouse::act(WareHouse &wareHouse) {
    extern WareHouse* backup;
    if (backup == nullptr)
    {
        error("No backup available");
        std::cout<< "Error: "<< getErrorMsg() << std::endl;
    }
    else{
            wareHouse = *backup;
    }
    wareHouse.addAction(this);
    complete();
}

std::string RestoreWareHouse::toString() const
{
    return "restoreWareHouse ";
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}
