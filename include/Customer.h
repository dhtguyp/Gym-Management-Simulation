#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "../include/Workout.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);//constructor
    Customer();
    virtual ~Customer(); //TODO destructor

    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
private:
    const std::string name;
    const int id;
};


class SweatyCustomer : public Customer { //Cardio only
public:
	SweatyCustomer(std::string name, int id);
    SweatyCustomer();

    ~SweatyCustomer(); //TODO destructor


    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


class CheapCustomer : public Customer { //Just the cheapest
public:
	CheapCustomer(std::string name, int id);
    CheapCustomer();
    ~CheapCustomer(); //TODO destructor

    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


class HeavyMuscleCustomer : public Customer { //Anaerobic only
public:
	HeavyMuscleCustomer(std::string name, int id);
    HeavyMuscleCustomer();

    ~HeavyMuscleCustomer(); //TODO destructor

    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


class FullBodyCustomer : public Customer { //cheapest cardio, expensive mixed, cheapest anaerobic
public:
	FullBodyCustomer(std::string name, int id);
    FullBodyCustomer();
    ~FullBodyCustomer(); //TODO destructor

    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


#endif