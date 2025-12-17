#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "data.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include <string.h>


void gameLoop(Room rooms[], Item items[], int startingRoomId, int itemMaxId);
char* name_to_ucase(const char* name);

#endif 