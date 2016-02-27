//
// Created by Simone on 22/02/2016.
//

#ifndef JOBSSCHEDULER_RESOURCE_H
#define JOBSSCHEDULER_RESOURCE_H

#include <unordered_map>

using namespace std;

class Resource {
private:
    int availability;
    unordered_map<int, int> usedResources;
public:
    Resource(int availability);
    int findStartTimeAvailableResource(int readyTime, int processingTime, int requiredResources);
    bool useResources(int start, int processingTime, int quantity);
    void releaseResources(int start, int processingTime, int quantity);
    bool isAvailable(int t, int quantity);
    int getAvailabilityAtTime(int t);
    void reset();


};

class ResourceRequest {
private:
    int resourceId;
    int quantity;
public:
    ResourceRequest(int r, int q): resourceId(r), quantity(q) {}

    int getResourceId() { return resourceId; }
    int getQuantity() { return quantity; }

};


#endif //JOBSSCHEDULER_RESOURCE_H
