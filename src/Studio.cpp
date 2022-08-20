#include "../include/Studio.h"
#include <iostream>
#include <fstream>

using namespace  std;
extern Studio* backup;


Studio::Studio():open(false){}
Studio::Studio(const std::string &filepath) {
    std::ifstream file(filepath);
    char line[256];
    int phase = 0;
    int numOfTrainers;
    int workout_id=0;
    bool inInfoLine = false;
    while(file){
        file.getline(line, 256);
        if(line[0] != '#' && line[0] !='\0'){
            switch(phase){
                case 1:
                    numOfTrainers = line[0] - '0';
                    inInfoLine=false;
                        break;

                case 2:
                    int capacity;
                    for (int i = 0; i < numOfTrainers; ++i) {
                        capacity = line[2*i] - '0';
                        Trainer* t = new Trainer(capacity);
                        trainers.push_back(t);
                    }
                    inInfoLine=false;
                    break;

                case 3: string s = line; //Setup the variables for each Workout object
                    int index = s.find(',');
                    string name = s.substr(0, index);

                    int index2 = s.substr(index+2).find(',');
                    string type_Str = s.substr(index+2, index2);

                    int amount = stoi(s.substr(index+index2+4));

                    WorkoutType type;
                    if(type_Str == "Anaerobic"){
                        type= WorkoutType::ANAEROBIC;
                    }
                    else if(type_Str=="Mixed"){
                        type = WorkoutType::MIXED;
                    }
                    else if(type_Str=="Cardio"){
                        type = WorkoutType::CARDIO;
                    }

                    Workout workout(workout_id, name, amount, type); //Create the Workout object
                    workout_options.push_back(workout);
                    workout_id = workout_id + 1;
                    inInfoLine=false;
                    break;

            }
        }
        else if(line[0] == '#'){ //Info line
            if(!inInfoLine) {
                phase = phase + 1;
                inInfoLine = true;
            }
        }
        else inInfoLine = false;

    }
    file.close();
}
Studio::Studio(const Studio &other):open(other.open){

        for(Trainer* trainer : other.trainers){
            Trainer* copyT = new Trainer (*trainer); //The backed up trainer will have different properties.
            trainers.push_back(copyT);
        }
        for(BaseAction* action: other.actionsLog){ //Once done, the actions remain unchanged.
            actionsLog.push_back(action);
        }
        for(Workout workout : other.workout_options){ //Unchanged.
            workout_options.push_back(workout);
        }

}

Studio::Studio(Studio &&other) {

        int trainerSize = other.trainers.size();
        int actionSize = other.actionsLog.size();
        int workoutSize = other.workout_options.size();

    for (int i = 0; i < trainerSize; ++i) {
        Trainer* copyT = new Trainer (*other.trainers[i]);
        trainers.push_back(copyT);
        delete other.trainers[i];
    }
        for (int i = 0; i < actionSize; ++i) {
            actionsLog.push_back(other.actionsLog[i]);
        }
        for (int i = 0; i < workoutSize; ++i) {
            workout_options.push_back(other.workout_options[i]);
        }
        other.trainers.clear();
        other.actionsLog.clear();
        other.workout_options.clear();

        delete &other;

}
Studio & Studio::operator=(const Studio &other){

    if(&other != this){
        open=other.open;
        for(Trainer* trainer : other.trainers){
            Trainer* copyT = new Trainer (*trainer); //The backed up trainer will have different properties.
            trainers.push_back(copyT);
        }
        std::cout << "here"<< std::endl;
        for(BaseAction* action: other.actionsLog){
            actionsLog.push_back(action);
        }
        for(Workout workout : other.workout_options){
            workout_options.push_back(workout);
        }
    }
    return *this;
}

Studio & Studio::operator=(Studio &&other) {
    if(&other != this){
        open=other.open;
        int trainerSize = other.trainers.size();
        int actionSize = other.actionsLog.size();
        int workoutSize = other.workout_options.size();

        for (int i = 0; i < trainerSize; ++i) {
            Trainer* copyT = new Trainer (*other.trainers[i]);
            trainers.push_back(copyT);
            delete other.trainers[i];
        }
        for (int i = 0; i < actionSize; ++i) {
            actionsLog.push_back(other.actionsLog[i]);
        }
        for (int i = 0; i < workoutSize; ++i) {
            workout_options.push_back(other.workout_options[i]);
        }
        other.trainers.clear();
        other.actionsLog.clear();
        other.workout_options.clear();
    }
    return *this;
}



void Studio::start() {
    open = true;
    std::cout << "Studio is now open!" << std::endl;
    std::string input;
    int totalCustomers=0;



    while(true){

        getline(std::cin, input); //Receive input TODO hererererer

        int size = (int) input.size();

        if (input.substr(0, 5) == "open " && (input.at(5) >= '0' && input.at(5) <= '9')) { //OrderTrainer
            std::vector<Customer*> customers;
            Customer* temp = nullptr;
            string idline = input.substr(5, 1);
            int id = stoi(idline);

            int startAt(7);
            int commaLocation;
            int endAt(8);
            char current_char;
            std::string type, name;

            while (endAt < size) {
                current_char = input.at(endAt);

                if (current_char == ',') {
                    commaLocation = endAt;
                } else if (current_char == ' ') {
                    type = input.substr(commaLocation + 1, endAt - commaLocation - 1);
                    name = input.substr(startAt, commaLocation - startAt);

                    //new Customer
                    if(type == "swt"){
                        temp = new SweatyCustomer(name, totalCustomers);
                    }
                    else if(type == "chp"){
                        temp = new CheapCustomer(name, totalCustomers);
                    }
                    else if(type == "mcl"){
                        temp = new HeavyMuscleCustomer(name, totalCustomers);
                    }
                    else { //(type == "fbd")
                        temp = new FullBodyCustomer(name, totalCustomers);
                    }

                    customers.push_back(temp);
                    temp = nullptr;
                    totalCustomers = totalCustomers+1;
                    startAt = endAt + 1;
                }


                endAt = endAt + 1;

            }
            //Last Customer

            type = input.substr(commaLocation + 1, endAt - commaLocation - 1);
            name = input.substr(startAt, commaLocation - startAt);
            if(type == "swt"){
                temp = new SweatyCustomer(name, totalCustomers);
            }
            else if(type == "chp"){
                temp = new CheapCustomer(name, totalCustomers);
            }
            else if(type == "mcl"){
                temp = new HeavyMuscleCustomer(name, totalCustomers);
            }
            else { //(type == "fbd")
                temp = new FullBodyCustomer(name, totalCustomers);
            }
            customers.push_back(temp);
            totalCustomers = totalCustomers+1;

            OpenTrainer* openTrainer = new OpenTrainer(id, customers);
            openTrainer->act(*this);
            actionsLog.push_back(openTrainer);
            customers.clear();
        }











        else if(input.substr(0, 6) == "order " && (input.at(6) >= '0' && input.at(6) <= '9')){ //OrderTrainer
            int trainerID = (input.at(6)) - '0';
            Order* order = new Order(trainerID);
            order->act(*this);
            actionsLog.push_back(order);
        }



        else if(input.substr(0, 5) == "move ") { //MoveCustomer
            int srcID = input.at(5) - '0';
            int toID = input.at(7) - '0';
            int custID = input.at(9) - '0';
            MoveCustomer* moveCustomer = new MoveCustomer(srcID, toID, custID);
            moveCustomer->act(*this);
            actionsLog.push_back(moveCustomer);

        }
        else if(input.substr(0, 6) == "close "){ //Close
            int ID = input.at(6) - '0';
            Close* close = new Close(ID);
            close->act(*this);
            actionsLog.push_back(close);
        }

        else if (input=="closeall") { //Closeall, will end the loop
            CloseAll* closeAll = new CloseAll();
            closeAll->act(*this);
            delete closeAll; // Doesn't matter that it's not store in actionsLog, since the program will be closed now.
            break;
        }

        else if(input == "workout_options"){
            PrintWorkoutOptions* pwo = new PrintWorkoutOptions();
            pwo->act(*this);
            actionsLog.push_back(pwo);
        }
        else if(input.substr(0, 7) == "status "){
            int ID = input.at(7) - '0';
            PrintTrainerStatus* pts = new PrintTrainerStatus(ID);
            pts->act(*this);
            actionsLog.push_back(pts);

        }
        else if(input == "log"){
            PrintActionsLog* log = new PrintActionsLog();
            log->act(*this);
            actionsLog.push_back(log);
        }

        else if(input == "backup"){ //BackupStudio
            if(backup != nullptr){
                backup = this;
            }

            BackupStudio *backupStudio = new BackupStudio();
            backupStudio->act(*this);
            backup = new Studio(*this);
            actionsLog.push_back(backupStudio);

        }

        else if(input == "restore"){ //RestoreStudio
            if(backup!= nullptr) {
                int backup_log_size = (int) backup->getActionsLog().size();
                int this_log_size = (int) actionsLog.size();
                int numOfTrainers = getNumOfTrainers();

                for (int i = this_log_size-1; i >= backup_log_size; i=i-1) { //Update log
                    delete actionsLog[i];
                    actionsLog.pop_back();
                }
                for (int i = 0; i < numOfTrainers; ++i) { //Delete current trainers
                    delete trainers[i];
                    trainers.pop_back();
                }

                trainers.clear();
                Trainer* temp;
                for (int i = 0; i < numOfTrainers; ++i) { //Update trainers
                    temp = new Trainer(*backup->getTrainer(i));
                    Trainer* backupTrainer = new Trainer(*temp);
                    delete temp;
                    trainers.push_back(backupTrainer);
                }

            }
            RestoreStudio* restore = new RestoreStudio();
            restore->act(*this);
            actionsLog.push_back(restore);
        }


    }
    DeleteLogs();
    open = false;
    //Finish
}




int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {  //Trainer Id's are initialized based on appearence from config file, from 0 to n-1, when n is the number of trainers.
    if(tid < (int)trainers.size() && tid >=0) {
        return trainers[tid];
    }
    return nullptr;
}
const std::vector<BaseAction *> &Studio::getActionsLog() const {return actionsLog;} //TODO

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}




void Studio:: DeleteLogs(){
    int actions_size = (int) getActionsLog().size();
    for (int i = 0; i < actions_size; ++i) {
        delete actionsLog[i];
    }
    actionsLog.clear();
}


Studio::~Studio() {
    actionsLog.clear();
    int trainers_size = (int) trainers.size();
    for (int i = 0; i < trainers_size; ++i) {
        delete trainers[i];
    }
}

