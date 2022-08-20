#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "../include/Customer.h"

enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction{
public:
    BaseAction();
    BaseAction(std::string _errorMsg, ActionStatus _status);
    virtual ~BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
    std::string stringStatus(); //Returns string of the action's status.

private:
    std::string errorMsg;
    ActionStatus status;


};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);

    OpenTrainer(const OpenTrainer& other); //copy
    OpenTrainer(OpenTrainer&& other); //Move
    OpenTrainer& operator=(const OpenTrainer& other); //assign
    OpenTrainer& operator=(OpenTrainer&& other); //move assign

    ~OpenTrainer(); //TODO IMPORTANT
    void act(Studio &studio);
    std::string toString() const;
private:
	const int trainerId;
	std::vector<Customer *> customers; //TODO delete
};


class Order : public BaseAction {
public:
    Order(int id);
    Order(const Order& other);
    ~Order();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    MoveCustomer(const MoveCustomer& other);
    ~MoveCustomer();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    Close(const Close& other);
    ~Close();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    CloseAll(const CloseAll& other);
    ~CloseAll();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    PrintWorkoutOptions(const PrintWorkoutOptions& other);
    ~PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    PrintTrainerStatus(const PrintTrainerStatus& other);
    ~PrintTrainerStatus();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    PrintActionsLog(const PrintActionsLog& other);
    ~PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    BackupStudio(const BackupStudio& other);
    ~BackupStudio();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    RestoreStudio(const RestoreStudio& other);
    ~RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;
};


#endif