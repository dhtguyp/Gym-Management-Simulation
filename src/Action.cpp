#include "../include/Action.h"
#include "Studio.h" //TODO Will probably need to remove this and add every function I created, back to studio class.

//BaseAction
BaseAction::BaseAction():status(ActionStatus::ERROR) {}
BaseAction::BaseAction(std::string _errorMsg, ActionStatus _status):errorMsg(_errorMsg), status(_status){}

BaseAction::~BaseAction() {}
ActionStatus BaseAction::getStatus() const {
    return status;
}





void BaseAction::complete() {
    status = ActionStatus::COMPLETED; // will be set to error by default, so if we receive a legitimate order, only then will the value change.
}

void BaseAction::error(std::string errorMsg) {
    this->errorMsg = errorMsg;
}
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

//ACTIONS

//OpenTrainer
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):trainerId(id){
    error("Workout session doesn't exist or is already open.");

    int size = (int) customersList.size();
    for (int i = 0; i < size; ++i) { //Configure customers
        customers.push_back(customersList[i]);
    }
}
OpenTrainer::OpenTrainer(const OpenTrainer &other):BaseAction(other.getErrorMsg(), other.getStatus()), trainerId(other.trainerId) {
    int size = (int) other.customers.size();
    Customer *cust = nullptr;
    for (int i = 0; i < size; ++i) { //Configure customers
        std::string to_str = customers[i]->toString();
        int comma = to_str.find(',');
        std::string type = to_str.substr(comma);
        std::string cust_name = customers[i]->getName();
        int custID = customers[i]->getId();
        if (type == "swt")
            cust = new SweatyCustomer(cust_name, custID);
        else if (type == "chp")
            cust = new CheapCustomer(cust_name, custID);
        else if (type == "mcl")
            cust = new HeavyMuscleCustomer(cust_name, custID);
        else //fbd
            cust = new SweatyCustomer(cust_name, custID);
        customers.push_back(cust);
    }
} //Copy

OpenTrainer::OpenTrainer(OpenTrainer&&other):BaseAction(other.getErrorMsg(), other.getStatus()), trainerId(other.trainerId) {
    int size = (int) other.customers.size();
    Customer *cust = nullptr;
    for (int i = 0; i < size; ++i) { //Configure customers
        std::string to_str = customers[i]->toString();
        int comma = to_str.find(',');
        std::string type = to_str.substr(comma);
        std::string cust_name = customers[i]->getName();
        int custID = customers[i]->getId();
        if (type == "swt")
            cust = new SweatyCustomer(cust_name, custID);
        else if (type == "chp")
            cust = new CheapCustomer(cust_name, custID);
        else if (type == "mcl")
            cust = new HeavyMuscleCustomer(cust_name, custID);
        else //fbd
            cust = new SweatyCustomer(cust_name, custID);
        customers.push_back(cust);
    }
    for (int i = 0; i < size; ++i) {
        delete other.customers[i];
    }
    other.customers.clear();
    delete &other;
} //Copy

OpenTrainer& OpenTrainer::operator=(const OpenTrainer& other){
    if(&other != this && other.getStatus() == ActionStatus(0)){

        int size = (int) other.customers.size();
        Customer *cust = nullptr;
        for (int i = 0; i < size; ++i) { //Configure customers
            std::string to_str = customers[i]->toString();
            int comma = to_str.find(',');
            std::string type = to_str.substr(comma);
            std::string cust_name = customers[i]->getName();
            int custID = customers[i]->getId();
            if (type == "swt")
                cust = new SweatyCustomer(cust_name, custID);
            else if (type == "chp")
                cust = new CheapCustomer(cust_name, custID);
            else if (type == "mcl")
                cust = new HeavyMuscleCustomer(cust_name, custID);
            else //fbd
                cust = new SweatyCustomer(cust_name, custID);
            customers.push_back(cust);
        }

    }
    return *this;
}

OpenTrainer& OpenTrainer::operator=(OpenTrainer&& other){
    if(&other != this && other.getStatus() == ActionStatus(0)){

        int size = (int) other.customers.size();
        Customer *cust = nullptr;
        for (int i = 0; i < size; ++i) { //Configure customers
            std::string to_str = customers[i]->toString();
            int comma = to_str.find(',');
            std::string type = to_str.substr(comma);
            std::string cust_name = customers[i]->getName();
            int custID = customers[i]->getId();
            if (type == "swt")
                cust = new SweatyCustomer(cust_name, custID);
            else if (type == "chp")
                cust = new CheapCustomer(cust_name, custID);
            else if (type == "mcl")
                cust = new HeavyMuscleCustomer(cust_name, custID);
            else //fbd
                cust = new SweatyCustomer(cust_name, custID);
            customers.push_back(cust);
        }
            for (int i = 0; i < size; ++i) {
            delete other.customers[i];
            }
            other.customers.clear();
            delete &other;

    }

    return *this;
}

OpenTrainer::~OpenTrainer(){
        for (Customer *c: customers) {
            delete c;
        }
}

void OpenTrainer::act(Studio &studio) { //Checks if the trainer's session is full, also gives to the trainer the customers.
    if(trainerId >= studio.getNumOfTrainers()) {
        std::cout << "Error: " << getErrorMsg() << std::endl;
        return;
    }
    Trainer* trainer = (studio.getTrainer(trainerId)); //get access to trainer via studio

    if(trainer == nullptr || trainer->isOpen()){ //trainer is already mid session
        std::cout << getErrorMsg() << std::endl;
        return;
    }
    if(trainer->getCapacity() ==0) return; //trainer can't get more customers, don't do anything
    trainer->openTrainer();
    int size =(int) trainer->getCustomers().size();

    if(size > 0) { //// Delete previous session's customers if there are any left.
        std::vector<Customer *> curr_customers = trainer->getCustomers();

        for (int i = 0; i < size; ++i) {
            trainer->removeCustomer(curr_customers[i]->getId());
        }
        curr_customers.clear();
    }
    std::vector<Workout> workoutOptions = studio.getWorkoutOptions();




    for (Customer *customer : customers) {
        if(trainer->getCapacity() == 0)
            break; // session is full
        trainer->addCustomer(customer);
        std::vector<int> customer_orders(customer->order(workoutOptions));
        int customerId = customer->getId();
        trainer->order(customerId, customer_orders, workoutOptions); //TODO THOS
    }
    complete();
}
std::string OpenTrainer::toString() const { //TODO
    std::string str = "open " + std::to_string(trainerId);
    for (Customer *customer:customers) {
        str = str + " " + customer->toString();
    }
    if(getStatus() == ActionStatus::COMPLETED)
        str = str + " Completed";
    else
        str = str+ " Error: " + getErrorMsg();
    return str;
}

//Order // WIll only occur once per open call.
Order::Order(int id):trainerId(id){
    error("Trainer does not exist or is not open");
}
Order::Order(const Order &other):BaseAction(other.getErrorMsg(), other.getStatus()), trainerId(other.trainerId) {} //Copy

Order::~Order() {}

void Order::act(Studio &studio) {
    if(trainerId >= studio.getNumOfTrainers() || trainerId < 0 //Trainer doesn't exist
    || !studio.getTrainer(trainerId)->isOpen()){ //Trainer is closed
        std::cout << "Error: " << getErrorMsg() << std::endl;
        return;
    }

    Trainer* trainer(studio.getTrainer(trainerId)); //get access to trainer via studio
    std::vector<OrderPair> orders = trainer->getOrders();

    for (const OrderPair &order: orders) {
        std::cout << trainer->getCustomer(order.first)->getName() << " is doing " << order.second.getName() << std::endl;
    }
    complete();
}
std::string Order::toString() const {
    std::string str= "order " + std::to_string(trainerId);
    if(getStatus() == ActionStatus::COMPLETED)
        str = str + " Completed";
    else
        str = str+ " Error: " + getErrorMsg();
    return str;
}

//MoveCustomer //TODO if src's capacity is 0, Close will be called. maybe in studio?
MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTrainer(src), dstTrainer(dst), id(customerId) {
    error("Cannot move costumer");
}
MoveCustomer::MoveCustomer(const MoveCustomer &other):BaseAction(other.getErrorMsg(), other.getStatus()), srcTrainer(other.srcTrainer),
dstTrainer(other.dstTrainer),id(other.id){} //Copy    
    
MoveCustomer::~MoveCustomer() {}
void MoveCustomer::act(Studio &studio) {

    Trainer* source(studio.getTrainer(srcTrainer));
    Trainer* destination(studio.getTrainer(dstTrainer));
    if(source == nullptr //Conditions for being able to pass customer.
    || destination == nullptr
    || source->getCustomer(id) == nullptr
    || destination->getCapacity() == 0
    || !source->isOpen()
    || !destination->isOpen()) {
        std::cout << "Error: " << getErrorMsg() << std::endl;
        return;
    }
    Customer* customer = source->getCustomer(id);
    std::vector<Workout> workoutOptions = studio.getWorkoutOptions();

    source->removeCustomer(id);
    int src_customers = (int) source->getCustomers().size();

    if(src_customers == 0){ //TODO
        Close* close = new Close(srcTrainer);
        close->act(studio);
        delete close;
        source->addCustomer(customer); // we wil keep a customer in trainer's list so we will know it had a session before.
    }


    destination->addCustomer(customer);
    destination->order(customer->getId(), customer->order(workoutOptions), workoutOptions);

    complete();
}

std::string MoveCustomer::toString() const {

    std::string str =  " move " + std::to_string(srcTrainer) + "S " + std::to_string(dstTrainer) + "D " + " " + std::to_string(id);
    if(getStatus() == ActionStatus::COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error: " + getErrorMsg();
    return str;
}

//Close
Close::Close(int id):trainerId(id) {
    error("Trainer does not exist or is not open");
}
Close::Close(const Close &other):BaseAction(other.getErrorMsg(), other.getStatus()), trainerId(other.trainerId){} //Copy    
Close::~Close() {}

void Close::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);
    if(trainer == nullptr || !trainer->isOpen()){
        std::cout << "Error: " << getErrorMsg() << std::endl;
        return;
    }
    std::cout << "Trainer " + std::to_string(trainerId) + " closed. Salary " + std::to_string(trainer->getSalary()) + "NIS." << std::endl; // print
    std::vector<Customer*> customers = trainer->getCustomers();
    for (Customer* c: customers) {
        trainer->removeCustomer(c->getId()); //TODO what else?
    }
    trainer->closeTrainer();
    complete();
}


std::string Close::toString() const {
    std::string str =  " close " + std::to_string(trainerId);
    if(getStatus() == ActionStatus::COMPLETED)
        str = str + " Completed";
    else
        str = str + " Error: " + getErrorMsg();
    return str;
}


//CLoseall
CloseAll::CloseAll() {
}
CloseAll::CloseAll(const CloseAll &other):BaseAction(other.getErrorMsg(), other.getStatus()){} //Copy 
void CloseAll::act(Studio &studio) {
    Trainer* trainer = nullptr;
    int numOfTrainers =studio.getNumOfTrainers();
    std::cout << "closeall"<< std::endl;
    for (int i = 0; i < numOfTrainers; ++i) {

        trainer = studio.getTrainer(i);
        if(trainer->isOpen()){

            Close close(i);
            close.act(studio);
        }
    }

    complete();
} // Studio while loop will be broken.

CloseAll::~CloseAll() {}
std::string CloseAll::toString() const {
    return "closeall";
}

//PrintWorkoutOptions
PrintWorkoutOptions::PrintWorkoutOptions() {}
PrintWorkoutOptions::PrintWorkoutOptions(const PrintWorkoutOptions &other):BaseAction(other.getErrorMsg(), other.getStatus()){} //Copy
PrintWorkoutOptions::~PrintWorkoutOptions() {}
void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout>* workoutOptions = &studio.getWorkoutOptions();

    for (const Workout &w: *workoutOptions) {
        WorkoutType type = w.getType();
        std::string str_type;

        if(type == WorkoutType::ANAEROBIC)
            str_type="Anaerobic";
        else if(type == WorkoutType::MIXED)
            str_type="Mixed";
        else //(type == WorkoutType::CARDIO)
        str_type="Cardio";

        std:: cout << w.getName() << ", " << str_type << ", " << w.getPrice() << std::endl;
    }

    complete();
}
std::string PrintWorkoutOptions::toString() const {
    return "workout_options Completed";
}

//PrintTrainerStatus
PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id) {}
PrintTrainerStatus::PrintTrainerStatus(const PrintTrainerStatus &other):BaseAction(other.getErrorMsg(), other.getStatus()),trainerId(other.trainerId){} //Copy
PrintTrainerStatus::~PrintTrainerStatus() {}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer* trainer(studio.getTrainer(trainerId));
    if(trainer != nullptr) {
        std::vector<Customer *> customers = trainer->getCustomers();

        if (trainerId >= studio.getNumOfTrainers() || trainerId < 0
            || (int) customers.size() ==
               0) { //After every opening and closing of a session, the trainer will keep his customers until the next opening.
            complete();
            return; //Invalid trainer Id
        }

        std::string str = "player_status: ";
        if (!trainer->isOpen()) {
            str = str + " closed";


        } else {
            std::vector<OrderPair> orders(trainer->getOrders());

            str = str + " open \nCustomers:\n";
            for (Customer *c: customers) {
                str = str + std::to_string(c->getId()) + " " + c->getName() + "\n";
            }

            str = str + "Orders:\n";
            for (OrderPair o: orders) {
                Workout workout = o.second;
                str = str + workout.getName() +" "+ std::to_string(workout.getPrice()) + " " + std::to_string(o.first) +
                      "\n";
            }
            str = str + "Current trainer's salary: " + std::to_string(trainer->getSalary());
        }

        std::cout << str << std::endl;

    }
    complete();
}

std::string PrintTrainerStatus::toString() const {
    std::string str = "print_status " + std::to_string(trainerId) + " Completed";
    return str;
}

//PrintActionsLog
PrintActionsLog::PrintActionsLog() {}
PrintActionsLog::PrintActionsLog(const PrintActionsLog &other):BaseAction(other.getErrorMsg(), other.getStatus()){} //Copy
PrintActionsLog::~PrintActionsLog() {}
void PrintActionsLog::act(Studio &studio) {
    std::string str;
    std::vector<BaseAction*> actionsLog = studio.getActionsLog();
    for (BaseAction* action: actionsLog) {
        str = str + action->toString() + "\n";
    }

    if(str.empty()) return;
    std::cout << str << std::endl;

    complete();
}
std::string PrintActionsLog::toString() const {
    return "log Completed";
}


extern Studio* backup;
//Backup
BackupStudio::BackupStudio() {}
BackupStudio::BackupStudio(const BackupStudio &other):BaseAction(other.getErrorMsg(), other.getStatus()){} //Copy
BackupStudio::~BackupStudio() {}
void BackupStudio::act(Studio &studio) { //backup will be updated in studio
    complete();
}
std::string  BackupStudio::toString() const {
    return "backup Complete";
}

//Restore

RestoreStudio::RestoreStudio() {
    error("No backup available");
}
RestoreStudio::RestoreStudio(const RestoreStudio &other):BaseAction(other.getErrorMsg(), other.getStatus()){} //Copy
RestoreStudio::~RestoreStudio() {}

void RestoreStudio::act(Studio &studio) {//TODO Studio will take care of acquiring previous session's data and releasing data.
    if(backup == nullptr) { //Will already be checked in Studio??
        std::cout << "Error: " << getErrorMsg() << std::endl;
        return;
    }
    complete();
}

std::string RestoreStudio::toString() const {
    std::string str = "restore ";
    if(getStatus() == ActionStatus::COMPLETED)
        str = str + "Completed";
    else
        str = str+ "Error: " + getErrorMsg();
    return str;

}




