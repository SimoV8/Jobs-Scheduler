//
// Created by Simone on 21/02/2016.
//

#include "Machine.h"

int Machine::indexOf(Job * job) {
    for(int i = 0; i < scheduledJobs.size(); ++i)
        if(scheduledJobs[i] == job)
            return i;
    return -1;
}

Machine::Machine(int machineId) {
    this->machineId = machineId;
    readyTime = 0;
}

void Machine::addJob(Job *job) {
    scheduledJobs.push_back(job);
}

void Machine::removeJob(Job *job) {
    int index = indexOf(job);
    if(index < 0) return;
    for(int i = index + 1; i < scheduledJobs.size(); i++)
        scheduledJobs[i - 1] = scheduledJobs[i];
    scheduledJobs.pop_back();
}

int Machine::getId() {
    return machineId;
}

Job* Machine::nextJob(Job *job) {
    int index = indexOf(job);
    if(index < 0 || index >= scheduledJobs.size() - 1)
        return nullptr;
    return scheduledJobs[index + 1];
}

Job *Machine::previousJob(Job *job) {
    int index = indexOf(job);
    if(index <= 0)
        return nullptr;
    return scheduledJobs[index - 1];
}

int Machine::getReadyTime() {
    return readyTime;
}

void Machine::setReadyTime(int t) {
    readyTime = t;
}

void Machine::addSetupTimeAfterJob(Job *job, DataContainer &data) {
    Job *nextJob = this->nextJob(job);
    if(nextJob == nullptr) return;
    readyTime += data.getSetupTime(job->getId(), nextJob->getId(), machineId);
}

void Machine::swap(Job *j1, Job *j2) {
    int index1 = indexOf(j1);
    int index2 = indexOf(j2);
    if(index1 < 0 || index2 < 0) return;
    scheduledJobs[index1] = j2;
    scheduledJobs[index2] = j1;
}

vector<Job *> &Machine::getScheduledJobs() { return scheduledJobs; }

void Machine::printScheduledJobs() {
    cout << "Machine " << machineId << ": ";
    for(Job * j : scheduledJobs) {
        cout << j->getId() << " ";
    }
    cout << endl;
}

void Machine::moveJob(Job *job, int position) {
    int index = indexOf(job);
    if(index < position) {
        for(int i = index; i <= position; ++i)
            scheduledJobs[i] = scheduledJobs[i + 1];
    } else {
        for(int i = index; i >= position; --i)
            scheduledJobs[i] = scheduledJobs[i - 1];
    }
    scheduledJobs[position] = job;
}
