//
// Created by simone on 21/02/16.
//

#ifndef JOBSSCHEDULER_RANDOMUTIL_H
#define JOBSSCHEDULER_RANDOMUTIL_H

#include <random>
#include <algorithm>
#include <iterator>

class RandomUtil {
private:

    std::random_device rd;
    std::default_random_engine gen;
    std::uniform_real_distribution<> real_distribution;

protected:
    RandomUtil();

public:
    static RandomUtil& getInstance();
    // delete copy and move constructors and assign operators
    RandomUtil(RandomUtil const&) = delete;             // Copy construct
    RandomUtil(RandomUtil&&) = delete;                  // Move construct
    RandomUtil& operator=(RandomUtil const&) = delete;  // Copy assign
    RandomUtil& operator=(RandomUtil&&) = delete;       // Move assign

    void shuffle(std::vector<int>& v);
    void inizializeAndShuffle(std::vector<int>& v, int from, int to);
    int randomInt(int from, int to);
    double randomDouble();

};


#endif //JOBSSCHEDULER_RANDOMUTIL_H
