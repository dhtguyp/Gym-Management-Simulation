#include <iostream>
#include "../include/Customer.h"
#include "../include/Trainer.h"

using namespace std;
Trainer::Trainer():capacity(0), open(false)  {}
Trainer::Trainer(int t_capacity) :capacity(t_capacity), open(false){ //Default con
    if (capacity < 0)
        capacity=0;
}


Trainer::Trainer(const Trainer &other):capacity(other.capacity), open(other.open) { //Copy con
    for(Customer* moved_cust: other.customersList){
        addCustomer(moved_cust);
    }
    for(OrderPair order: other.orderList){
        orderList.push_back(order);
    }
}

Trainer::Trainer(Trainer &&other):capacity(other.capacity), open(other.open) { //Move con

    for(Customer* moved_cust: other.customersList){

        int custID = moved_cust->getId();

        addCustomer(moved_cust);
        other.removeCustomer(custID);
    }
    for(OrderPair order: other.orderList){
        orderList.push_back(order);
    }

    other.customersList.clear();
    other.orderList.clear();
    other.capacity = 0;
}

Trainer &Trainer::operator=(const Trainer &other) { //Copy assign
    capacity = other.getCapacity();
    open = other.open;
    for (Customer* curr_cus: customersList){
        int id = curr_cus->getId();
        removeCustomer(id);
    }
    customersList.clear();
    for(Customer* moved_cust: other.customersList){
        addCustomer(moved_cust);
    }
    orderList.clear();
    for(OrderPair order: other.orderList){
        orderList.push_back(order);
    }
    return *this;
}

Trainer &Trainer::operator=(Trainer &&other){ //Move assign, TODO will be used for Restore?
    if (this != &other){
        capacity = other.getCapacity();
        open = other.open;
        for (Customer* curr_cus: customersList){
            int id = curr_cus->getId();
            removeCustomer(id);
        }
        customersList.clear();
        for(Customer* moved_cust: other.customersList){
            int custID = moved_cust->getId();
            addCustomer(moved_cust);
            other.removeCustomer(custID);
        }
        orderList.clear();
        for(OrderPair order: other.orderList){
            orderList.push_back(order);
        }

        other.customersList.clear();
        other.orderList.clear();
        other.capacity = 0;
    }
    return *this;
};




int Trainer::getCapacity() const {
    return capacity;
}



void Trainer::addCustomer(Customer* customer){
    customersList.push_back(customer);
    capacity = capacity - 1; //Since the trainer has one more customer to keep his eyes on
}

void Trainer::removeCustomer(int id) { //Will be called if getCustomer return true.

    int size = (int)customersList.size();
    int curr_Id;
    for (int i =0; i < size ; i++)
    {
        curr_Id = customersList[i]->getId();
        if (curr_Id == id)
        {
            //Sequencing delete from customerList
            customersList.erase(customersList.begin()+i);
            break;
        }
    }

    //Sequencing delete of relevant orders from OrderList
    size = (int) orderList.size();
    std::vector<OrderPair> updated_list;
    for (int j = 0; j < size; ++j) {
        if (orderList[j].first != id) {//order is NOT of targeted customer
            updated_list.push_back(orderList[j]);
        }
    }
    orderList.clear();

    for (OrderPair o : updated_list) {
        orderList.push_back(o);
    }

    capacity = capacity + 1;
}

Customer *Trainer::getCustomer(int id) {

    for (Customer *c : customersList){
        if (c->getId() == id){
            return c;
        }
    }
    return nullptr; //Customer isn't linked to this trainer
}

std::vector<Customer*>& Trainer::getCustomers() {//TODO WIll call this and delete customers when reopening
    return customersList;
}

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) { //check in studio if the capacity isn't 0.
    int size = (int)workout_ids.size();

    for(int i=0 ; i < size; i++){
        int customer_workoutId(workout_ids[i]);
        for (const Workout workout : workout_options) {
            if (workout.getId() != customer_workoutId)continue;
            OrderPair newOrder(customer_id, workout); //workout_options are ordered by their ids.
            orderList.push_back(newOrder);
        }
    }

}

void Trainer::openTrainer() {//Via Action
    open = true;
}

void Trainer::closeTrainer() {//Via Action
    open = false;
}

int Trainer::getSalary() {
    int salary=0;
    int size = (int) orderList.size();
    for (int i = 0; i <  size; ++i) {
        salary = salary + orderList[i].second.getPrice();
    }
    return  salary;
}

bool Trainer::isOpen() {
    return open;
}

Trainer::~Trainer() { //Destructor

}
