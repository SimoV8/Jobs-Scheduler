//
// Created by Simone on 21/02/2016.
//

#ifndef JOBSSCHEDULER_MACHINE_H
#define JOBSSCHEDULER_MACHINE_H

#include <vector>
#include "Job.h"


using namespace std;

class Machine {
private:
    vector<Job*> scheduledJobs;
    int machineId;
    int readyTime;

    int indexOf(Job *job);

public:
    Machine(int machineId);
    void addJob(Job* job);
    void removeJob(Job* job);
    int getId();
    vector<Job*>& getScheduledJobs();
    Job* nextJob(Job *job);
    Job* previousJob(Job *job);
    int getReadyTime();
    void setReadyTime(int t);
    void addSetupTimeAfterJob(Job *job, DataContainer& data);
    void swap(Job* j1, Job* j2);
    void printScheduledJobs();
    void moveJob(Job*job, int position);

};

#endif //JOBSSCHEDULER_MACHINE_H
