//
// Created by simone on 20/02/16.
//

#ifndef JOBSSCHEDULER_SOLUTION_H
#define JOBSSCHEDULER_SOLUTION_H

#include <vector>
#include <list>
#include "Job.h"
#include "Machine.h"
#include "DataContainer.h"
#include "Resource.h"

using namespace std;

class Solution {
private:
    DataContainer * data;
    long objective;
    vector<Job*> jobs;
    vector<Machine*> machines;
    vector<Resource*> resources;
    vector<Job*> schedulingList;

public:
    Solution(DataContainer* dataContainer);
    Solution(const Solution &s);
    ~Solution();
    Job* getJob(int jobId);
    long getObjective();
    void setObjective(long objective);
    void addInSchedulingList(int jobId, int machineId);
    void scheduleJobs();
    void swapJobs(Job *j1, Job *j2);
    vector<Machine*>& getMachines();
    vector<Resource*>& getResources();
    vector<Job*>& getSchedulingList();
    vector<Job*>& getJobs();
    void writeOfCsv(string filename);

    void resetJobPositionInMachine(Job* job);

    static Solution* generateRandomSolution(DataContainer * data);
    void printSchedulingListIds();

};


#endif //JOBSSCHEDULER_SOLUTION_H
