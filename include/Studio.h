#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "../include/Workout.h"
#include "../include/Trainer.h"
#include "../include/Action.h"
#include <fstream>

class Studio{		
public:
	Studio(); //empty con
    Studio(const std::string &configFilePath); //Def con
    Studio(const Studio& other); //Copy con
    Studio(Studio&& other); //Move con
    Studio& operator=(const Studio& other); //copy assign
    Studio& operator=(Studio&& other); //move assign
    ~Studio(); //Destructor
    void DeleteLogs();

    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
};

#endif