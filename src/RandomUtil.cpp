//
// Created by simone on 21/02/16.
//

#include <chrono>
#include "RandomUtil.h"

RandomUtil::RandomUtil() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    gen = std::default_random_engine(seed);
}

RandomUtil& RandomUtil::getInstance() {
    static RandomUtil randomUtil;
    return randomUtil;
}

void RandomUtil::shuffle(std::vector<int> &v) {
    std::shuffle(v.begin(), v.end(), gen);
}

void RandomUtil::inizializeAndShuffle(std::vector<int> &v, int from, int to) {
    v.clear();
    for(int i = from; i <= to; ++i)
        v.push_back(i);
    shuffle(v);
}

int RandomUtil::randomInt(int from, int to) {
    std::uniform_int_distribution<> dis(from, to);
    return dis(gen);
}

double RandomUtil::randomDouble() {
    return real_distribution(gen);
}
