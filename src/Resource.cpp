//
// Created by Simone on 22/02/2016.
//

#include "Resource.h"

Resource::Resource(int availability) {
    this->availability = availability;
}

int Resource::findStartTimeAvailableResource(int readyTime, int processingTime, int requiredResources) {
    if(requiredResources > availability) return -1;
    int start = readyTime;
    int counter = 0;
    while(counter < processingTime) {
        auto elem = usedResources.find(start + counter);
        if(elem == usedResources.end() || elem->second + requiredResources <= availability)
            ++counter;
        else {
            start += counter + 1;
            counter = 0;
        }
    }

    return start;
}

bool Resource::useResources(int start, int processingTime, int quantity) {
    for(int i = start; i < start + processingTime; ++i) {
        usedResources[i] += quantity;
        if(usedResources[i] > availability) {
            // Not possible: revert update and return false
            for(int j = start; j <= i; ++j)
                usedResources[j] -= quantity;
            return false;
        }
    }
    return true;
}

void Resource::releaseResources(int start, int processingTime, int quantity) {
    for(int i = start; i < start + processingTime; ++i) {
        usedResources[i] = max(usedResources[i] - quantity, 0);
    }
}

bool Resource::isAvailable(int t, int quantity) {
    return getAvailabilityAtTime(t) > 0;
}

int Resource::getAvailabilityAtTime(int t) {
    auto elem = usedResources.find(t);
    int used = elem != usedResources.end() ? elem->second : 0;
    return availability - used;
}


void Resource::reset() {
    usedResources.clear();
}


