//
// Created by simone on 12/02/16.
//

#include "DataContainer.h"
#define READY_TIME 0
#define DUE_DATE 1
#define TARDINESS_PENALTY 2

bool getRow(istream &in, vector<string> &row, int nElements) {
    string line, temp;

    if(!getline(in, line) || line == "\r" || line == "") return false;
    stringstream str(line);

    row.clear();
    while(getline(str, temp, ';'))
        row.push_back(temp);

    if(row.size() != nElements) {
        string message = "Expected number of elements in row: "+to_string(nElements)+", found: "+to_string(row.size());
        throw new invalid_argument(message);
    }

    return true;
}

void printMatrix(int ** matrix, int N, int M) {
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < M; ++j)
            cout << matrix[i][j] << "\t";
        cout << endl;
    }
    cout << endl;
}


int** inizializeMatrix(int N, int M) {
    int **matrix = new int*[N];
    for(int i = 0; i < N; ++i)
        matrix[i] = new int[M];

    for(int i = 0; i < N; ++i)
        for(int j = 0; j < M; ++j)
            matrix[i][j] = -1;

    return matrix;
}

void DataContainer::readParameters(istream &in) {
    vector<string> row;
    while(getRow(in, row, 2)) {

        string key = row[0];
        string value = row[1];

        if(key == "#Jobs"){
            nJobs = std::stoi(value);
        } else if(key == "#Machines") {
            nMachines = std::stoi(value);
        } else if(key == "#Resources") {
            nResources = std::stoi(value);
        } else { // Undefined key
            return;
        }
    }
}

void DataContainer::readProcessingTimeTable(istream &in) {
    processingTime = inizializeMatrix(nJobs, nMachines);
    availableMachinesPerJob = new vector<int>[nJobs];

    vector<string> row;
    int jobId, machineId, prTime;
    while(getRow(in, row, 3)) {
        jobId = std::stoi(row[0]);
        machineId = std::stoi(row[1]);
        prTime = std::stoi(row[2]);
        processingTime[jobId - 1][machineId] = prTime;
        availableMachinesPerJob[jobId - 1].push_back(machineId);
    }
}

void DataContainer::readTimeTable(istream &in) {
    timeTable = inizializeMatrix(nJobs, 3);
    vector<string> row;
    int jobId, readyTime, dueDate, tardinessPenalty;
    while(getRow(in, row, 4)) {
        jobId = std::stoi(row[0]);
        dueDate = std::stoi(row[1]);
        readyTime = std::stoi(row[2]);
        tardinessPenalty = std::stoi(row[3]);
        timeTable[jobId - 1][READY_TIME]        = readyTime;
        timeTable[jobId - 1][DUE_DATE]          = dueDate;
        timeTable[jobId - 1][TARDINESS_PENALTY] = tardinessPenalty;
    }
}

void DataContainer::readSetupTable(istream &in) {
    setupTable = new int**[nMachines];
    for(int i = 0; i < nMachines; ++i)
        setupTable[i] = inizializeMatrix(nJobs, nJobs);
    vector<string> row;
    int machineId, predJob, succJob, setupTime;
    while(getRow(in, row, 4)) {
        machineId = std::stoi(row[0]);
        predJob = std::stoi(row[1]);
        succJob = std::stoi(row[2]);
        setupTime = std::stoi(row[3]);

        setupTable[machineId][predJob - 1][succJob - 1] = setupTime;
    }
}

void DataContainer::readResourceAvailability(istream &in) {
    resourcesAvailability = new int[nResources];
    vector<string> row;
    int resId, availability;
    while(getRow(in, row, 2)) {
        resId = std::stoi(row[0]);
        availability = std::stoi(row[1]);
        resourcesAvailability[resId] = availability;
    }

}

void DataContainer::readResourceRequest(istream &in) {
    resourceRequests = new vector<ResourceRequest>[nJobs];
    vector<string> row;
    int jobId, resId, resqu;
    while(getRow(in, row, 3)) {
        jobId = std::stoi(row[0]);
        resId = std::stoi(row[1]);
        resqu = std::stoi(row[2]);
        resourceRequests[jobId - 1].push_back(ResourceRequest(resId, resqu));
    }
}

/***** PUBLIC METHODS *****/

DataContainer::DataContainer(string filename) {
    ifstream in(filename);
    string line;
    readParameters(in);
    getline(in, line); // Remove table header
    readProcessingTimeTable(in);
    getline(in, line); // Remove table header
    readTimeTable(in);
    getline(in, line); // Remove table header
    readSetupTable(in);
    getline(in, line); // Remove extra whiteline
    getline(in, line); // Remove table header
    readResourceAvailability(in);
    getline(in, line); // Remove table header
    readResourceRequest(in);

    in.close();
}

int DataContainer::getNJobs() { return nJobs; }

int DataContainer::getNMachines() { return nMachines; }

int DataContainer::getNResources() { return nResources; }

int DataContainer::getProcessingTime(int jobId, int machineId) {
    return processingTime[jobId - 1][machineId];
}

int DataContainer::getReadyTime(int jobId) {
    return timeTable[jobId - 1][READY_TIME];
}


int DataContainer::getDueDate(int jobId) {
    return timeTable[jobId - 1][DUE_DATE];
}

int DataContainer::getTardinessPenalty(int jobId) {
    return timeTable[jobId - 1][TARDINESS_PENALTY];
}

int DataContainer::getSetupTime(int jobId1, int jobId2, int machineId) {
    return setupTable[machineId][jobId1 - 1][jobId2 - 1];
}

int DataContainer::getResourceAvailability(int resourceId) {
    return resourcesAvailability[resourceId];
}

vector<ResourceRequest> &DataContainer::getResourceRequests(int jobId) {
    return resourceRequests[jobId - 1];
}

vector<int>& DataContainer::getAvailableMachines(int jobId) {
    return availableMachinesPerJob[jobId -1];
}
