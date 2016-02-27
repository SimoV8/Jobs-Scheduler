//
// Created by simone on 20/02/16.
//

#ifndef JOBSSCHEDULER_JOB_H
#define JOBSSCHEDULER_JOB_H


#include "DataContainer.h"

class Machine;

class Job {
private:
    int id;
    int readyTime;
    int start;
    int end;
    int index;
    Machine* machine;

public:
    Job(int id);
    Job(const Job& job) = default;
    void setMachine(Machine *machine);
    int getId();
    int getReadyTime();
    int getStartTime();
    int getEndTime();
    int getIndex();
    void setIndex(int index);
    Machine * getMachine();
    void schedule(DataContainer &data, vector<Resource*>& resources);
    int getCost(DataContainer &data);
    void releaseResources(DataContainer &data, vector<Resource*>& resources);


};


#endif //JOBSSCHEDULER_JOB_H
