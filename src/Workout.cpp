#include "../include/Workout.h"

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type): id(w_id), name(w_name), price(w_price), type(w_type){ }
Workout::Workout():id(-1),name(""),price(0),type(WorkoutType(0)) {}




int Workout::getId() const{
    return id;
}
std::string Workout::getName() const{
        return name;

}

int Workout::getPrice() const{

        return price;

}

WorkoutType Workout::getType() const{

        return type;


}


