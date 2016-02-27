//
// Created by simone on 20/02/16.
//

#include "Solver.h"
#include "RandomUtil.h"

Solver::Solver(DataContainer *dataContainer) {
    this->data = dataContainer;

    currentSolution = Solution::generateRandomSolution(data);
    bestSolution = new Solution(*currentSolution);
    temp = INIT_T;
    localSearch();
}

Solution *Solver::getBestSolution() {
    return bestSolution;
}

void Solver::revertSwap(Job *j1, Job *j2, Machine *m, int oldReady) {
    vector<Resource*> resources = currentSolution->getResources();
    j1->releaseResources(*data, resources);
    j2->releaseResources(*data, resources);
    currentSolution->swapJobs(j1, j2);
    m->setReadyTime(oldReady);
    j1->schedule(*data, resources);
    j2->schedule(*data, resources);
}

void Solver::localSearchOnMachine(Machine *m) {
    long oldObjective =  currentSolution->getObjective();
    vector<Resource*> resources = currentSolution->getResources();

    vector<Job*> scheduledJobs = m->getScheduledJobs();
    for(int i = 0; i < scheduledJobs.size() - 1; ++i) {
        scheduledJobs = m->getScheduledJobs();
        Job *j1 = scheduledJobs[i];
        Job *j2 = scheduledJobs[i+1];
        int oldReady = j1->getReadyTime();
        int oldEnd = j2->getEndTime();
        int oldCost = j1->getCost(*data) + j2->getCost(*data);
        j1->releaseResources(*data, resources);
        j2->releaseResources(*data, resources);
        // swap j1 and j2 and compute the new cost and end time
        int readyTime = oldReady;
        if( i > 0) {
            // Remove old setup time with the previous job and add the new one
            Job *prev = scheduledJobs[i-1];
            readyTime -= data->getSetupTime(prev->getId(), j1->getId(), m->getId());
            readyTime += data->getSetupTime(prev->getId(), j2->getId(), m->getId());
        }
        if( i < scheduledJobs.size() - 2) {
            // Update oldEnd considering also the setup time
            Job *next = scheduledJobs[i+2];
            oldEnd = next->getReadyTime();
        }

        m->setReadyTime(readyTime);
        currentSolution->swapJobs(j1, j2);
        j2->schedule(*data, resources);
        j1->schedule(*data, resources);
        int newCost = j2->getCost(*data) + j1->getCost(*data);
        int newEnd = j1->getEndTime(); // Ready time after j1 and j2 have been scheduled (includes setup time)
        if((newCost > oldCost && newEnd > oldEnd) || (newCost == oldCost && newEnd > oldEnd)) {
            // Bad move, turn back!
            revertSwap(j1, j2, m, oldReady);
            continue;
        }
        // To check if the move improves the solution we have to update all subsequent jobs
        // that can start before their current ready time
        for(int j = i + 2; j < scheduledJobs.size(); ++j) {
            oldCost += scheduledJobs[j]->getCost(*data);
            scheduledJobs[j]->releaseResources(*data, resources);
            scheduledJobs[j]->schedule(*data, resources);
            newCost += scheduledJobs[j]->getCost(*data);
        }
        if(newCost <= oldCost) {
            // :) we improved our objective cost!

            long objective = currentSolution->getObjective();
            objective = objective - oldCost + newCost;
            currentSolution->setObjective(objective);
        } else {
            // :( the move made the objective worse, we have to roll back!
            for(int j = i; j < scheduledJobs.size(); ++j)
                scheduledJobs[j]->releaseResources(*data, resources);
            currentSolution->swapJobs(j1, j2);
            m->setReadyTime(oldReady);
            scheduledJobs = m->getScheduledJobs();
            // We have also to revert the update all subsequent changed jobs
            for(int j = i; j < scheduledJobs.size(); ++j)
                scheduledJobs[j]->schedule(*data, resources);
        }
    }
}

void Solver::localSearch() {
    long oldObjective = currentSolution->getObjective();
    long newObjective = oldObjective;
    short counter = 0;
    do {
        oldObjective = newObjective;
        for(Machine * m : currentSolution->getMachines()) {
            localSearchOnMachine(m);
        }
        newObjective = currentSolution->getObjective();
        // cout << "Local search: from " << oldObjective << " to " << currentSolution->getObjective() << endl;
    } while(newObjective < oldObjective && ++counter < 1000);
}

void Solver::explorationStep() {
    long oldObjective = currentSolution->getObjective();
    long newObjective = oldObjective;
    int nJobs = data->getNJobs();
    RandomUtil& rand = RandomUtil::getInstance();
    //cout << "Old Objective: "<< oldObjective << endl;
    for(int i = 0; i < nJobs / 2 ; ++i) {
        if(rand.randomDouble() > 0.5) {
           trySwapping();
        } else {
            tryChangeMachine();
        }
        updateBestSolution(currentSolution);
    }
    //cout  << "New Objective: "<< oldObjective << endl;
}

bool Solver::acceptMove(long oldObjective, long newObjective) {
    if(newObjective <= oldObjective)
        return true;
    double pr = std::exp(-(newObjective - oldObjective)/ temp);
    return RandomUtil::getInstance().randomDouble() <= pr;
}

void Solver::updateTemp() {
    temp = ALPHA * temp;
}

void Solver::updateBestSolution(Solution *s) {
    //s->scheduleJobs();
    if(s->getObjective() < bestSolution->getObjective()) {
        delete bestSolution;
        bestSolution = new Solution(*s);
    }
}

void Solver::trySwapping() {
    long oldObjective = currentSolution->getObjective();
    int nJobs = data->getNJobs();
    RandomUtil& rand = RandomUtil::getInstance();
    int index1 = rand.randomInt(0, nJobs - 1);
    int index2 = rand.randomInt(0, nJobs - 1);
    vector<Job*> jobs = currentSolution->getSchedulingList();
    Job *j1 = jobs[index1];
    Job *j2 = jobs[index2];
    currentSolution->swapJobs(j1, j2);
    currentSolution->scheduleJobs();
    long newObjective = currentSolution->getObjective();
    if(acceptMove(oldObjective, newObjective)) {
        oldObjective = newObjective;
        //cout << "Job " << index1 << " swapped with job " << index2 << endl;
    } else {
        // Turn back
        currentSolution->swapJobs(j1, j2);
        currentSolution->scheduleJobs();
    }
}

void Solver::tryChangeMachine() {
    long oldObjective = currentSolution->getObjective();
    int nJobs = data->getNJobs();
    RandomUtil& rand = RandomUtil::getInstance();

    int counter = 0;
    vector<int> availableMachines;
    Job *j;
    do {
        // Try up to 5 times to get a job schedulable to more than one machine
        int index = rand.randomInt(0, nJobs - 1);
        j = currentSolution->getSchedulingList()[index];
        availableMachines = data->getAvailableMachines(j->getId());
        ++counter;
    } while(availableMachines.size() <= 1 && counter < 5);

    int index = rand.randomInt(0, availableMachines.size() - 1);
    int machineId = availableMachines[index];
    int oldMachineId = j->getMachine()->getId();
    if(machineId == oldMachineId)
        return;
    j->setMachine(currentSolution->getMachines()[machineId]);
    currentSolution->resetJobPositionInMachine(j);
    currentSolution->scheduleJobs();
    long newObjective = currentSolution->getObjective();
    // Move the job in the right position inside the machine
    do {
        newObjective = currentSolution->getObjective();
        localSearchOnMachine(j->getMachine());
    } while(currentSolution->getObjective() < newObjective);
    if(acceptMove(oldObjective, newObjective)) {
        //cout << "Job " << j->getId() << " moved from machine " << oldMachineId << " to machine " << machineId << endl;
        oldObjective = newObjective;
    } else {
        j->setMachine(currentSolution->getMachines()[oldMachineId]);
        currentSolution->resetJobPositionInMachine(j);
        currentSolution->scheduleJobs();
    }
}

void Solver::updateSolution() {
    explorationStep();
    localSearch();
    updateTemp();
    updateBestSolution(currentSolution);
}


