#include "Action.h"
#include "Volunteer.h"
#include <iostream>

BaseAction::BaseAction() {}

ActionStatus BaseAction::getStatus() const
{
    return status;
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
//---------------------------------------------
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){}

void SimulateStep::act(WareHouse &wareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        for (Order *order : wareHouse.getPendingOrders())
        {
            bool isTaken = false;
            for (Volunteer *volenteer : wareHouse.getVolunteers())
            {
                if (volenteer->canTakeOrder(*order))
                {
                    volenteer->acceptOrder(*order);
                    wareHouse.moveOrderToInProcess(order);
                    isTaken = (bool)true;
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
}

std::string SimulateStep::toString() const
{
    return "Simulate Step";
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

//---------------------------------------------
AddOrder::AddOrder(const int customerId) : customerId(customerId){}

void AddOrder::act(WareHouse &wareHouse)
{
    int id = wareHouse.uniqeOrderNum();
    Customer *customer = &wareHouse.getCustomer(customerId);
    // fix what happen if customer is not found
    if (!(customer->canMakeOrder()))
    {
        error("Cannot place this order");
    }
    else
    {
        Order *order = new Order(id, customerId, customer->getCustomerDistance());
        wareHouse.addOrder(order);
        customer->addOrder(id);
    }
}

std::string AddOrder::toString() const
{
    return "Add Order";
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

//---------------------------------------------
AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) :
                     customerName(customerName), customerType(typeFromString(customerType)), distance(distance), maxOrders(maxOrders){}

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
    if (customerType == CustomerType::Soldier)
    {
        customer = new SoldierCustomer(wareHouse.uniqeCustomerId(), customerName, distance, maxOrders);
    }
    else if (customerType == CustomerType::Civilian)
    {
        customer = new CivilianCustomer(wareHouse.uniqeCustomerId(), customerName, distance, maxOrders);
    }
    wareHouse.AddCustomer(customer);
}

std::string AddCustomer::toString() const
{
    return "Add Customer";
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}

//---------------------------------------------
PrintOrderStatus::PrintOrderStatus(int id) : id(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse) {}

std::string PrintOrderStatus::toString() const
{
    return "Print Order Status";
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

//---------------------------------------------
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse) {}

std::string PrintCustomerStatus::toString() const
{
    return "Print Customer Status";
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

//---------------------------------------------
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse) {}

std::string PrintActionsLog::toString() const
{
    return "Print Actions Log";
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

//---------------------------------------------
Close::Close() {}

void Close::act(WareHouse &wareHouse) {}

std::string Close::toString() const
{
    return "Close";
}

Close *Close::clone() const
{
    return new Close(*this);
}

//---------------------------------------------
BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse) {}

std::string BackupWareHouse::toString() const
{
    return "Backup WareHouse";
}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

//---------------------------------------------
RestoreWareHouse::RestoreWareHouse() {} // RESTORE WAREHOUSE

void RestoreWareHouse::act(WareHouse &wareHouse) {}

std::string RestoreWareHouse::toString() const
{
    return "Restore WareHouse";
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}
