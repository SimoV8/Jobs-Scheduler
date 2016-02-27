//
// Created by simone on 20/02/16.
//

#include "Job.h"
#include "Machine.h"

Job::Job(int id) {
    this->id = id;
    machine = nullptr;
    readyTime = start = end = -1;
}

void Job::setMachine(Machine *machine) {
    if(this->machine != nullptr)
        this->machine->removeJob(this);
    this->machine = machine;
    this->machine->addJob(this);
}

int Job::getId() { return id; }

int Job::getReadyTime() { return readyTime; }

int Job::getStartTime() { return start; }

int Job::getEndTime() { return end; }

int Job::getIndex() { return index; }

void Job::setIndex(int index) { Job::index = index; }

Machine *Job::getMachine() { return machine; }

void Job::schedule(DataContainer &data, vector<Resource*>& resources) {
    int processingTime = data.getProcessingTime(this->id, machine->getId());

    //Check when the job can start the processing comparing the machine and job ready time
    int readyM = machine->getReadyTime();
    readyTime = readyM; //Used for local search
    int readyJ = data.getReadyTime(this->id);
    start = max(readyM, readyJ);
    //Check when the job can start the processing looking at the first instant in which all the resources are available
    int nextStart = start;
    vector<ResourceRequest> resRequests = data.getResourceRequests(this->id);
    do {
        start = nextStart;
        for(ResourceRequest& rr : resRequests) {
            Resource* resource = resources[rr.getResourceId()];
            int t = resource->findStartTimeAvailableResource(nextStart, processingTime, rr.getQuantity());
            nextStart = max(t, nextStart);
        }
    } while(nextStart != start);
    // Found a start time in which all the resources are available!
    // Use resources so that next jobs won't use them again
    for(ResourceRequest& rr : resRequests) {
        Resource* resource = resources[rr.getResourceId()];
        resource->useResources(start, processingTime, rr.getQuantity());
    }
    // Compute end time and udate machine ready time adding the setup time before the next job starts.
    end = start + processingTime;
    machine->setReadyTime(end);
    machine->addSetupTimeAfterJob(this, data);
}


int Job::getCost(DataContainer &data) {
    return max(0, end - data.getDueDate(id)) * data.getTardinessPenalty(id);
}


void Job::releaseResources(DataContainer &data, vector<Resource *> &resources) {
    vector<ResourceRequest> resRequests = data.getResourceRequests(this->id);
    for(ResourceRequest& rr : resRequests) {
        Resource *resource = resources[rr.getResourceId()];
        resource->releaseResources(start, data.getProcessingTime(this->id, machine->getId()), rr.getQuantity());
    }
}
