//
// Created by simone on 20/02/16.
//

#include "Solution.h"
#include "RandomUtil.h"

Solution::Solution(DataContainer *dataContainer) {
    data = dataContainer;
    for(int i = 0; i < data->getNJobs(); ++i)
        jobs.push_back(new Job(i + 1));
    for(int i = 0; i < data->getNMachines(); ++i)
        machines.push_back(new Machine(i));
    for(int i = 0; i < data->getNResources(); ++i)
        resources.push_back(new Resource(data->getResourceAvailability(i)));
}

Solution::Solution(const Solution &s) {
    data = s.data;
    objective = s.objective;
    for(int i = 0; i < data->getNMachines(); ++i) {
        machines.push_back(new Machine(i));
        machines[i]->setReadyTime(s.machines[i]->getReadyTime());
    }
    for(int i = 0; i < data->getNJobs(); ++i)
        jobs.push_back(new Job(*s.jobs[i]));
    for(int i = 0; i < data->getNResources(); ++i)
        resources.push_back(new Resource(*s.resources[i]));
    for(Job * j : s.schedulingList) {
        addInSchedulingList(j->getId(), j->getMachine()->getId());
    }
}

Solution::~Solution() {
    for(Job * j: jobs)
        delete j;
    for(Machine * m: machines)
        delete m;
    for(Resource * r: resources)
        delete r;
}

Job *Solution::getJob(int jobId) {
    if(jobId > 0 && jobId <= jobs.size())
        return jobs[jobId - 1];
    else
        return nullptr;
}

long Solution::getObjective() { return objective; }

void Solution::setObjective(long objective) {
    this->objective = objective;
}

void Solution::addInSchedulingList(int jobId, int machineId) {
    Job * j = getJob(jobId);
    j->setMachine(machines[machineId]);
    schedulingList.push_back(j);
    j->setIndex(schedulingList.size() - 1);
}

Solution *Solution::generateRandomSolution(DataContainer *data) {
    Solution* solution = new Solution(data);
    vector<int> jobIds;
    RandomUtil &randomUtil = RandomUtil::getInstance();
    randomUtil.inizializeAndShuffle(jobIds, 1, data->getNJobs());

    for(int jobId : jobIds) {
        vector<int> &availableMachines = data->getAvailableMachines(jobId);
        int index = randomUtil.randomInt(0, availableMachines.size() - 1);
        int machineId = availableMachines[index];
        solution->addInSchedulingList(jobId, machineId);
    }

    solution->scheduleJobs();

    return solution;
}


void Solution::scheduleJobs() {
    objective = 0;
    for(Resource* r : resources)
        r->reset();
    for(Machine* m : machines)
        m->setReadyTime(0);
    for(Job* j : this->schedulingList) {
        j->schedule(*data, resources);
        objective += j->getCost(*data);
    }
}

void Solution::writeOfCsv(string filename) {
    ofstream outf(filename);
    outf << "TWT;" << objective << ";;" << endl;
    outf << "JobId;MachineId;Start;Completion" << endl;
    for(Job *j : schedulingList) {
        outf << j->getId() << ";" << j->getMachine()->getId() << ";" << j->getStartTime() << ";" << j->getEndTime() << endl;
    }
    outf.close();

}

void Solution::swapJobs(Job *j1, Job *j2) {
    int temp = j1->getIndex();
    j1->setIndex(j2->getIndex());
    j2->setIndex(temp);
    schedulingList[j1->getIndex()] = j1;
    schedulingList[j2->getIndex()] = j2;

    if(j1->getMachine() == j2->getMachine())
        j1->getMachine()->swap(j1, j2);
    else {
        resetJobPositionInMachine(j1);
        resetJobPositionInMachine(j2);
    }
}

vector<Resource *>& Solution::getResources() { return resources; }

vector<Machine *>& Solution::getMachines() { return machines; }

vector<Job *>& Solution::getSchedulingList() { return schedulingList; }

vector<Job *>& Solution::getJobs() { return jobs; }


void Solution::printSchedulingListIds() {
    for(Job *j : schedulingList) {
        cout << j->getId() << " ";
    }
    cout << endl;
}

void Solution::resetJobPositionInMachine(Job *job) {
    int pos = 0;
    for(int i = 0; i < schedulingList.size(); ++i) {
        if(schedulingList[i] == job)
            break;
        if(schedulingList[i]->getMachine() == job->getMachine())
            ++pos;
    }
    job->getMachine()->moveJob(job, pos);
}
