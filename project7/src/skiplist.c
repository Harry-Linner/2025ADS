#include <stdio.h>
#include <stdlib.h>
#include "skiplist.h"

int randomLevel(int maxLevel) {
    // Generate the level for a certain node
    int level = 1;
    // In the skip list, we raise the level for a certain node by 1/2 chance
    while ((rand() % 2 == 0) && level < maxLevel) {
        // Elevate level by 1/2
        level ++;
    }
    return level;
}

