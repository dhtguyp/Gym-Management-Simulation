#include "../include/Customer.h"
#include <iostream>

//Customer
Customer::Customer(std::string c_name, int c_id): name(c_name),id(c_id) {}
Customer::Customer():name(""),id(-1) {}
Customer::~Customer() {}

std::string Customer::getName() const {
    return name;
}
int Customer::getId() const {
    return id;
}

//SweatyCustomer
SweatyCustomer::SweatyCustomer(std::string name, int id):Customer(name,id) {}
SweatyCustomer::SweatyCustomer():Customer() {}
SweatyCustomer::~SweatyCustomer(){}


std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> specOrder;
    for (Workout workout : workout_options){
        if(workout.getType() == WorkoutType::CARDIO)
            specOrder.push_back(workout.getId());
    }
    return specOrder;
}
std::string SweatyCustomer::toString() const {
    return getName() + ",swt";
}

//CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id) {}
CheapCustomer::CheapCustomer():Customer() {}
CheapCustomer::~CheapCustomer(){}
std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) { //return necessary IDs
    std::vector<int> specOrder(1, 0);
    int temp = INT16_MAX;
    for (Workout workout : workout_options){
        if(workout.getPrice() < temp) {
            temp = workout.getPrice();
            specOrder[0] = workout.getId();
        }
    }
    return specOrder;
}
std::string CheapCustomer::toString() const {
    return getName() + ",chp";
}

//HeavyMuscleCustomer
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):Customer(name,id) {}
HeavyMuscleCustomer::HeavyMuscleCustomer():Customer() {}
HeavyMuscleCustomer::~HeavyMuscleCustomer(){}
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> specOrder;
    for (Workout workout : workout_options){
        if(workout.getType() == WorkoutType::ANAEROBIC)
            specOrder.push_back(workout.getId());
    }
    return specOrder;
}
std::string HeavyMuscleCustomer::toString() const {
    return Customer::getName()  + ",mcl";
}

//FullBodyCustomer
FullBodyCustomer::FullBodyCustomer(std::string name, int id):Customer(name,id) {}
FullBodyCustomer::FullBodyCustomer():Customer() {}
FullBodyCustomer::~FullBodyCustomer(){}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {//cheapest cardio, expensive mixed, cheapest anaerobic
    std::vector<int> specOrder(3, 0); //check if legitimate
    int cheapest_c=INT16_MAX;
    int expensive_m=INT16_MIN;
    int cheapest_a=INT16_MAX;
    int curr_workout_price;
    int workoutID;
    WorkoutType type;
    for (Workout workout : workout_options){
        workoutID = workout.getId();
        curr_workout_price = workout.getPrice();
        type = workout.getType();
        if(type == WorkoutType::CARDIO &&  curr_workout_price < cheapest_c) {
            specOrder[0] = workoutID;
            cheapest_c =  curr_workout_price;
        }
        else if(type == WorkoutType::MIXED &&  curr_workout_price > expensive_m) {
            specOrder[1] = workoutID;
            expensive_m =  curr_workout_price;
        }
        else if(type == WorkoutType::ANAEROBIC &&  curr_workout_price < cheapest_a) {
            specOrder[2] = workoutID;
            cheapest_a =  curr_workout_price;
        }
    }
    return specOrder;
}
std::string FullBodyCustomer::toString() const {
    return Customer::getName()  + ",fbd";
}