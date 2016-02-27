//
// Created by simone on 20/02/16.
//

#ifndef JOBSSCHEDULER_SOLVER_H
#define JOBSSCHEDULER_SOLVER_H


#include "DataContainer.h"
#include "Solution.h"
#include "Job.h"
#include "Resource.h"

#define INIT_T 120.0
#define ALPHA 0.9

class Solver {
private:
    DataContainer * data;
    Solution *bestSolution;
    Solution *currentSolution;
    double temp;

    void revertSwap(Job *j1, Job *j2, Machine *m, int oldReady);
    bool acceptMove(long oldObjective, long newObjective);

    void updateTemp();
    void updateBestSolution(Solution * s);
    void localSearchOnMachine(Machine * m);

public:
    Solver(DataContainer *dataContainer);
    Solution* getBestSolution();

    void localSearch();
    void explorationStep();
    void updateSolution();

    void trySwapping();
    void tryChangeMachine();


};


#endif //JOBSSCHEDULER_SOLVER_H
