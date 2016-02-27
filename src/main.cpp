#include <iostream>
#include <random>
#include <chrono>
#include "DataContainer.h"
#include "Solution.h"
#include "Solver.h"
#include "RandomUtil.h"

using namespace std;

int main(int argc, char **argv) {
    if(argc < 2) {
        cout << "Error, arguments missing!\n";
        cout << "usage: " << argv[0] << " <input file>" << endl;
        return -1;
    }
    cout << "Loading data..." << endl;
    string inputFile = argv[1];
    DataContainer *data = new DataContainer(inputFile);
    int time = 0.12 * data->getNMachines() * data->getNJobs() * 1000;
    cout << "Time Available: " << time << "s" << endl;

    string outputFile = inputFile.substr(0, inputFile.size() - 4) + "_sol.csv";
    Solver solver(data);

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    long elapsed_ms = 0;
    while(elapsed_ms < time) {
        solver.updateSolution();
        cout << "--------------------------------------------------------------\n";
        end = std::chrono::system_clock::now();
        elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        cout << "elapsed time: " << elapsed_ms << "ms\n";
    }

    solver.getBestSolution()->scheduleJobs();

    ofstream out("output.csv", std::ofstream::out | std::ofstream::app);
    out << inputFile << ";" << elapsed_ms << " ms;" << solver.getBestSolution()->getObjective() << endl;

    cout << "Best solution objective: " << solver.getBestSolution()->getObjective() << endl;

    solver.getBestSolution()->writeOfCsv(outputFile);

    return 0;
}