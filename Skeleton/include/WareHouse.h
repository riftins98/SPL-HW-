#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Action.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        WareHouse &operator=(const WareHouse &other);
        ~WareHouse();
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        vector<Volunteer*> &getVolunteers();
        vector<Order*> &getPendingOrders();
        void moveOrderToInProcess(Order* order);
        void moveOrderToCompleted(Order* order);
        void moveOrderToInPending(Order* order);
        void removeVolunteer(Volunteer* volunteer);
        int uniqeOrderNum();  
        int uniqeCustomerId();
        int getCustomerCounter() const;
        int getOrderCounter() const;
        void AddCustomer(Customer* customer);
        int getVolunteerCounter() const;
        void addAction(BaseAction* action);
        void addToAllOrders(Order* order);
        vector<Order*> getAllOrders();
        vector<Customer*> getCustomers();
        void checkActionAct(string input);
        

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Order*> allOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
};