#include "Volunteer.h"
#include <iostream>

Volunteer::Volunteer(int id, const string &name): id(id), name(name){}

string Volunteer::getName(){
    return name;
}
int Volunteer::getId(){
    return id;
}

int Volunteer::getActiveOrderId(){
    return activeOrderId;
}

int Volunteer::getCompletedOrderId(){
    return completedOrderId;
}

bool Volunteer::isBusy(){
    return activeOrderId != -1;
}

bool Volunteer:: hasOrdersLeft(){}

virtual bool Volunteer::canTakeOrder(const Order &order){
    return false;
}

void Volunteer::acceptOrder(const Order &order){}

void Volunteer::step(){}

string Volunteer::toString() const{}


Volunteer::clone() const{}

///////////////////////////////////CollectorVolunteer////////////////////////////////////////////

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown): Volunteer(id,name), coolDown(coolDown){}

void CollectorVolunteer::step(){}
bool CollectorVolunteer::canTakeOrder(const Order &order){
    if (order.getStatus() == OrderStatus::PENDING && coolDown==0 && !isBusy()
    {
        return true;
    }
    else
    {
        return false;
    }    
}
int CollectorVolunteer::getTimeLeft() const{}
void CollectorVolunteer::decreaseCoolDown(){}
void CollectorVolunteer::acceptOrder() const{}
string CollectorVolunteer::toString() const{}

//////////////////////////////////LimitedCollectorVolunteer/////////////////////////////////////////////

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders):
                             CollectorVolunteer(id,name,coolDown), maxOrders(maxOrders){}
LimitedCollectorVolunteer::clone() const{}
bool LimitedCollectorVolunteer::hasOrdersLeft() const{}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    if (order.getStatus() == OrderStatus::PENDING && coolDown==0 && !isBusy() && getNumOrdersLeft()>0)
    {
        return true;
    }
    else
    {
        return false;
    }    
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order){}
int LimitedCollectorVolunteer::getMaxOrders() const{}
int LimitedCollectorVolunteer::getNumOrdersLeft() const{}
string LimitedCollectorVolunteer::toString() const{}

//////////////////////////////////DriverVolunteer/////////////////////////////////////////////

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep):
                        Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep){}

void DriverVolunteer::step(){}
bool DriverVolunteer::canTakeOrder(const Order &order){
    if (order.getStatus() == OrderStatus::COLLECTING  && !isBusy() && order.getDistance<=maxDistance)
    {
        return true;
    }
    else
    {
        return false;
    }    
}
int DriverVolunteer:: getDistanceLeft() const{}
int DriverVolunteer:: getMaxDistance() const{}
void DriverVolunteer:: decreaseDistanceLeft(){}
void DriverVolunteer:: acceptOrder(const Order &order){}
bool DriverVolunteer:: hasOrdersLeft() const{}
string DriverVolunteer:: toString() const{}
DriverVolunteer:: clone() const{}

//////////////////////////////////LimitedDriverVolunteer/////////////////////////////////////////////

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders): 
                        DriverVolunteer(id,name,0,maxDistance), distancePerStep(distancePerStep), maxOrders(maxOrders){}
LimitedDriverVolunteer::clone() const{}
int LimitedDriverVolunteer::getMaxOrders() const{}
int LimitedDriverVolunteer::getNumOrdersLeft() const{}
bool LimitedDriverVolunteer::hasOrdersLeft() const{}
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const{
    if (order.getStatus() == OrderStatus::COLLECTING  && !isBusy() && order.getDistance<=maxDistance && hasOrdersLeft())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void LimitedDriverVolunteer::acceptOrder(const Order &order){}
string LimitedDriverVolunteer::toString() const{}
