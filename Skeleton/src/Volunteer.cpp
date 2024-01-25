#include "Volunteer.h"
#include <iostream>

Volunteer::Volunteer(int id, const string &name): id(id), name(name){}

const string &Volunteer::getName() const{
    return name;
}
int Volunteer::getId() const{
    return id;
}

int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

bool Volunteer::isBusy() const{
    return activeOrderId != -1;
}

bool Volunteer::hasOrdersLeft() const{
    return true;
}

bool Volunteer::canTakeOrder(const Order &order) const{
    return false;
}

void Volunteer::acceptOrder(const Order &order){}

void Volunteer::step(){}

string Volunteer::toString() const{}


Volunteer* Volunteer::clone() const{}

///////////////////////////////////CollectorVolunteer////////////////////////////////////////////

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown): Volunteer(id,name), coolDown(coolDown){}

void CollectorVolunteer::step(){
    if(isBusy){
        bool isDone = decreaseCoolDown();
        if(isDone){
            completedOrderId = activeOrderId;
            activeOrderId = -1;
            //לאפס cooldown
        }
    }
}
bool CollectorVolunteer::canTakeOrder(const Order &order) const{
    if (order.getStatus() == OrderStatus::PENDING && coolDown==0 && !isBusy())
    {
        return true;
    }
    else
    {
        return false;
    }    
}

int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return coolDown;
}
bool CollectorVolunteer::decreaseCoolDown() {
    timeLeft--;
    return (timeLeft==0);
}
void CollectorVolunteer::acceptOrder(const Order &order){
    //למלא שנבין מה צריך לעשות פה
}
string CollectorVolunteer::toString() const{}
CollectorVolunteer* CollectorVolunteer::clone() const{}

//////////////////////////////////LimitedCollectorVolunteer/////////////////////////////////////////////

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders):
                             CollectorVolunteer(id,name,coolDown), maxOrders(maxOrders){}
LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const{}
bool LimitedCollectorVolunteer::hasOrdersLeft() const{
    return ordersLeft>0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    if (order.getStatus() == OrderStatus::PENDING && getCoolDown()==0 && !isBusy() && getNumOrdersLeft()>0)
    {
        return true;
    }
    else
    {
        return false;
    }    
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order){
    ordersLeft--;
    //למלא שנבין מה צריך לעשות פה
    }
int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}
int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}
string LimitedCollectorVolunteer::toString() const{}

//////////////////////////////////DriverVolunteer/////////////////////////////////////////////

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep):
                        Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep){}

void DriverVolunteer::step(){
    if(isBusy){
        bool isDone = decreaseDistanceLeft();
        if(isDone){
            completedOrderId = activeOrderId;
            activeOrderId = -1;
            //לאפס distanceLeft
        }
    }
}
bool DriverVolunteer::canTakeOrder(const Order &order) const{
    return(order.getStatus() == OrderStatus::COLLECTING  && !isBusy() && order.getDistance()<=maxDistance);
}
int DriverVolunteer:: getDistanceLeft() const{
    return distanceLeft;
}
int DriverVolunteer:: getMaxDistance() const{
    return maxDistance;
}
bool DriverVolunteer:: decreaseDistanceLeft(){
    distanceLeft-=distancePerStep;
    if(distanceLeft<0){
        distanceLeft=0;
    }
    return (distanceLeft==0);
}
void DriverVolunteer:: acceptOrder(const Order &order){
     //למלא שנבין מה צריך לעשות פה
}
bool DriverVolunteer:: hasOrdersLeft() const{
    return true;
}
string DriverVolunteer:: toString() const{}
DriverVolunteer* DriverVolunteer:: clone() const{}
int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}
int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}

//////////////////////////////////LimitedDriverVolunteer/////////////////////////////////////////////

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
                        DriverVolunteer(id,name,maxDistance,distancePerStep), maxOrders(maxOrders){}
LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const{}
int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}
int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}
bool LimitedDriverVolunteer::hasOrdersLeft() const{
    return ordersLeft>0;
}
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const{
    if (order.getStatus() == OrderStatus::COLLECTING  && !isBusy() && order.getDistance() <= getMaxDistance() && hasOrdersLeft())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void LimitedDriverVolunteer::acceptOrder(const Order &order){
    ordersLeft--;
    //למלא שנבין מה צריך לעשות פה
}
string LimitedDriverVolunteer::toString() const{}
bool LimitedDriverVolunteer::hasOrdersLeft() const{
    return ordersLeft>0;
}


