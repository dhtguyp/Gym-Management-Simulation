#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "../include/Customer.h"
#include "../include/Workout.h"

typedef std::pair<int, Workout> OrderPair;


class Trainer{
public:
    Trainer();
    Trainer(int t_capacity); //constructor
    Trainer(const Trainer & other); //copy constructor
    Trainer(Trainer && other); //TODO move constructor
    Trainer& operator=(const Trainer& other); //TODO copy assignment
    Trainer& operator=(Trainer&& other); //TODO move assignment
    ~Trainer(); //TODO destructor

    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif