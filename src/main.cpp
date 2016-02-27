#include <iostream>
#include <random>
#include <chrono>
#include "DataContainer.h"
#include "Solution.h"
#include "Solver.h"
#include "RandomUtil.h"

using namespace std;

int main(int argc, char **argv) {
    if(argc < 3) {
        cout << "Error, arguments missing!\n";
        cout << "usage: " << argv[0] << " <input file> <time> [outputDir]" << endl;
        return -1;
    }
    //cout << "Loading data..." << endl;
    string inputFile = argv[1];
    DataContainer *data = new DataContainer(inputFile);

    int time = atoi(argv[2]) * 1000 - 50;

    // Remove directory if present.
    string filename = inputFile;
    const size_t last_slash_idx = inputFile.find_last_of("\\/");
    if (std::string::npos != last_slash_idx)
        filename = inputFile.substr(last_slash_idx+1);

    // If user specificied an output dir use it.
    string outputDir = "";
    if(argc > 3)
        outputDir = argv[3];

    string outputFile = outputDir + filename.substr(0, filename.size() - 4) + "_sol.csv";
    Solver solver(data);

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    long elapsed_ms = 0;
    while(elapsed_ms < time) {
        solver.updateSolution();
         //cout << "--------------------------------------------------------------\n";
        end = std::chrono::system_clock::now();
        elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        // cout << "elapsed time: " << elapsed_ms << "ms\n";
        //cout << "Current Best solution: " << solver.getBestSolution()->getObjective() << endl;
    }

    solver.getBestSolution()->scheduleJobs();

    ofstream out("output.csv", std::ofstream::out | std::ofstream::app);
    out << filename << ";" << elapsed_ms << " ms;" << solver.getBestSolution()->getObjective() << endl;
    out.close();

    cout << "Best solution objective: " << solver.getBestSolution()->getObjective() << endl;

    solver.getBestSolution()->writeOfCsv(outputFile);

    return 0;
}