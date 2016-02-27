//
// Created by simone on 12/02/16.
//

#ifndef JOBSSCHEDULER_DATACONTAINER_H
#define JOBSSCHEDULER_DATACONTAINER_H

#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Resource.h"

using namespace std;

class DataContainer {
private:
    int nJobs;
    int nMachines;
    int nResources;
    int **processingTime;
    int **timeTable;
    int ***setupTable;
    int *resourcesAvailability;
    vector<ResourceRequest> *resourceRequests;
    vector<int> *availableMachinesPerJob;

    void readParameters(istream &in);
    void readProcessingTimeTable(istream &in);
    void readTimeTable(istream &in);
    void readSetupTable(istream &in);
    void readResourceAvailability(istream &in);
    void readResourceRequest(istream & in);

public:

    DataContainer(string filename);

    virtual ~DataContainer() { }

    int getNJobs();
    int getNMachines();
    int getNResources();
    int getProcessingTime(int jobId, int machineId);
    int getReadyTime(int jobId);
    int getDueDate(int jobId);
    int getTardinessPenalty(int jobId);
    int getSetupTime(int jobId1, int jobId2, int machineId);
    int getResourceAvailability(int resourceId);
    vector<ResourceRequest>& getResourceRequests(int jobId);
    vector<int>&getAvailableMachines(int jobId);

};


#endif //JOBSSCHEDULER_DATACONTAINER_H
