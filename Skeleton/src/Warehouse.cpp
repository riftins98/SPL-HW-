#include "WareHouse.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

WareHouse::WareHouse(const string &configFilePath)
{
    std::ifstream file(configFilePath);
    if (!file)
    {
        throw std::runtime_error("Could not open file");
    }
    customerCounter = 0;
    volunteerCounter = 0;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue; // Skip empty lines or comments
        }
        else
        {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "customer")
            {
                std::string name;
                std::string customerType;
                int distance;
                int maxOrders;
                iss >> name >> customerType >> distance >> maxOrders;
                int id = uniqeCustomerId();
                if (customerType == "soldier")
                {
                    AddCustomer(new SoldierCustomer(id, name, distance, maxOrders));
                }
                else if (customerType == "civilian")
                {
                    AddCustomer(new CivilianCustomer(id, name, distance, maxOrders));
                }
            }
            else if (type == "volunteer")
            {
                volunteerCounter= volunteers.size(); 
                std::string name, role;
                int coolDown, maxDistance, distancePerStep, maxOrders;
                iss >> name >> role;
                if (role == "collector" || role == "limited_collector")
                {
                    iss >> coolDown;
                    if (role == "limited_collector")
                    {
                        iss >> maxOrders;
                        volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, name, coolDown, maxOrders));
                    }
                    else
                    {
                        volunteers.push_back(new CollectorVolunteer(volunteerCounter, name, coolDown));
                    }
                }
                else if (role == "driver" || role == "limited_driver")
                    {
                        iss >> maxDistance >> distancePerStep;
                        if (role == "limited_driver")
                        {
                            iss >> maxOrders;
                            volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders));
                        }
                        else
                        {
                            volunteers.push_back(new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep));
                        }
                    }
                
            }
        }
    }
    file.close();
}

WareHouse &WareHouse::operator=(const WareHouse &other)
{
    if (this != &other)
    {
        actionsLog.erase(actionsLog.begin(), actionsLog.end());

        for (BaseAction *action : other.actionsLog)
        {
            BaseAction *actionCopy = action->clone();
            actionsLog.push_back(actionCopy);
        }

        volunteers.erase(volunteers.begin(), volunteers.end());
        for (Volunteer *volunteer : other.volunteers)
        {
            Volunteer *volunteerCopy = volunteer->clone();
            volunteers.push_back(volunteerCopy);
        }
        pendingOrders.erase(pendingOrders.begin(), pendingOrders.end());

        for (Order *order : other.pendingOrders)
        {
            Order *orderCopy = order->clone();
            pendingOrders.push_back(orderCopy);
        }

        inProcessOrders.erase(inProcessOrders.begin(), inProcessOrders.end());

        for (Order *order : other.inProcessOrders)
        {
            Order *orderCopy = order->clone();
            inProcessOrders.push_back(orderCopy);
        }
        completedOrders.erase(completedOrders.begin(), completedOrders.end());

        for (Order *order : other.completedOrders)
        {
            Order *orderCopy = order->clone();
            completedOrders.push_back(orderCopy);
        }
        allOrders.erase(allOrders.begin(), allOrders.end());

        for (Order *order : other.allOrders)
        {
            Order *orderCopy = order->clone();
            allOrders.push_back(orderCopy);
        }
        customers.erase(customers.begin(), customers.end());

        for (Customer *customer : other.customers)
        {
            Customer *customerCopy = customer->clone();
            customers.push_back(customerCopy);
        }
        this->isOpen = other.isOpen;
        this->customerCounter = other.customerCounter;
        this->volunteerCounter = other.volunteerCounter;
    }
}

WareHouse::~WareHouse()
{
    for (Customer *customer : customers)
    {
        customer->~Customer();
    }
    customers.clear();
    for (Volunteer *volunteer : volunteers)
    {
        volunteer->~Volunteer();
    }
    volunteers.clear();
    for (Order *order : pendingOrders)
    {
        order->~Order();
    }
    
    for (Order *order : inProcessOrders)
    {
        order->~Order();
    }
    for (Order *order : completedOrders)
    {
        order->~Order();
    }
    for (Order *order : allOrders)
    {
        order->~Order();
    }
    for (BaseAction *action : actionsLog)
    {
        action->~BaseAction();
    }
    delete this;
}

void WareHouse::start()
{
    open();
    string input;
    std::getline(std::cin, input);
    while (input != "close")
    {
        checkActionAct(input); // מתרגם לפעולות ונקרא לו בשם אחר
        std::getline(std::cin, input);
    }
    BaseAction *closeAction = new Close();
    closeAction->act(*this);
}

void WareHouse::addOrder(Order *order)
{
    pendingOrders.push_back(order);
}

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

void WareHouse::checkActionAct(string input)
{
    std::istringstream iss(input);
    string action;
    iss >> action;

    if (action == "order")
    {
        string name;
        string address;
        int id;
        iss >> id;
        BaseAction *orderAction = new ::AddOrder(id);
        orderAction->act(*this);
    }
    else if (action == "customer")
    {
        string name;
        string type;
        int distance;
        int maxOrders;
        iss >> name >> type >> distance >> maxOrders;
        BaseAction *customersAction = new ::AddCustomer(name, type, distance, maxOrders);
        customersAction->act(*this);
    }
    else if (action == "step")
    {
        int numOfSteps;
        iss >> numOfSteps;
        BaseAction *stepAction = new SimulateStep(numOfSteps);
        stepAction->act(*this);
    }
    else if (action == "orderStatus")
    {
        int id;
        iss >> id;
        BaseAction *orderStatusAction = new PrintOrderStatus(id);
        orderStatusAction->act(*this);
    }
    else if (action == "customerStatus")
    {
        int id;
        iss >> id;
        BaseAction *customersAction = new PrintCustomerStatus(id);
        customersAction->act(*this);
    }
    else if (action == "volunteerStatus")
    {
        int id;
        iss >> id;
        BaseAction *volunteerStatusAction = new PrintVolunteerStatus(id);
        volunteerStatusAction->act(*this);
    }

    else if (action == "log")
    {
        BaseAction *logAction = new PrintActionsLog();
        logAction->act(*this);
    }
    else if (action == "close")
    {
        BaseAction *closeAction = new Close();
        this->close();
    }
    else if (action == "backup")
    {
        BaseAction *backupWareHouseAction = new BackupWareHouse();
        backupWareHouseAction->act(*this);
    }
    else if (action == "restore")
    {
        BaseAction *restoreAction = new RestoreWareHouse();
        restoreAction->act(*this);
    }
    else
    {
        std::cout << "Invalid input" << std::endl;
    }
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for (Customer *customer : customers)
    {
        if (customer->getId() == customerId)
        {
            return *customer;
        }
    }
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    return *volunteers[volunteerId];
}

Order &WareHouse::getOrder(int orderId) const
{
    for (Order *order : pendingOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (Order *order : inProcessOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (Order *order : completedOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
}

const vector<BaseAction *> &WareHouse::getActions() const
{
    return actionsLog;
    // todo
}

vector<Order *> &WareHouse::getPendingOrders()
{
    return pendingOrders;
}

void WareHouse::close()
{
    for (Order *order : getAllOrders())
    {
        std::cout << "OrderId: " << order->getId() << ", CustomerId: " << order->getCustomerId() << ", Status: " << order->getStatusString() << std::endl;
    }
    this->isOpen = false;
    this->~WareHouse();
}

void WareHouse::open()
{
    std::cout << "Warehouse is open!" << std::endl;
    isOpen = true;
}

vector<Volunteer *> &WareHouse::getVolunteers()
{
    return volunteers;
}

void WareHouse::moveOrderToInProcess(Order *order)
{
    if (order->getStatus() == OrderStatus::PENDING)
    {
        order->setStatus(OrderStatus::COLLECTING);
    }
    else if (order->getStatus() == OrderStatus::COLLECTING)
    {
        order->setStatus(OrderStatus::DELIVERING);
    }
    inProcessOrders.push_back(order);
    pendingOrders.erase(remove(pendingOrders.begin(), pendingOrders.end(), order), pendingOrders.end());
    ;
}

void WareHouse::moveOrderToCompleted(Order *order)
{
    order->setStatus(OrderStatus::COMPLETED);
    completedOrders.push_back(order);
    inProcessOrders.erase(remove(inProcessOrders.begin(), inProcessOrders.end(), order), inProcessOrders.end());
}

void WareHouse::moveOrderToInPending(Order *order)
{
    pendingOrders.push_back(order);
    inProcessOrders.erase(remove(inProcessOrders.begin(), inProcessOrders.end(), order), inProcessOrders.end());
}

void WareHouse::removeVolunteer(Volunteer *volunteer)
{
    volunteers.erase(remove(volunteers.begin(), volunteers.end(), volunteer), volunteers.end());
}

int WareHouse::uniqeOrderNum()
{
    return pendingOrders.size() + inProcessOrders.size() + completedOrders.size();
}

int WareHouse::uniqeCustomerId()
{
    customerCounter = customers.size();
    return customerCounter;
}

int WareHouse::getCustomerCounter() const

{
    return customerCounter;
}
    

int WareHouse::getOrderCounter() const
{
    return pendingOrders.size() + inProcessOrders.size() + completedOrders.size();
}

void WareHouse::AddCustomer(Customer *customer)
{
    customers.push_back(customer);
}

int WareHouse::getVolunteerCounter() const
{
    return volunteerCounter;
}

void WareHouse::addToAllOrders(Order *order)
{
    allOrders.push_back(order);
}

vector<Order *> WareHouse::getAllOrders()
{
    return allOrders;
}

vector<Customer *> WareHouse::getCustomers()
{
    return customers;
}