#ifndef _road_h
#define _road_h
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "car.h"
#include "queue.h"

typedef struct RoadData
{
    /* TODO - Add data associated with road.  Some suggested data: */
    /* length of this road */
    int length;
    
    /* information used to record/update whether the light at the end of this road is green or red */
    bool green;
    int GreenOn;
    int GreenOff;
    int CycleReset;
    int TimeStep;
    /* intersections this road starts from and moves to */
    int from;
    int to;
    /* Each road should have an array to track cars currently driving on them.  The type of this array can be either `Car**` 
     * (array of car pointers) or `Car*` (array of cars) but the former may be easier to use since locations on the road can 
     * be set to `NULL` if no car is present.
     */
    Car** cars;
    /* For each road it is recommended to have a `queue` of cars waiting to enter the simulated road system (see `queue.h` for 
     * the queues functions which have already been implemented for you).  Cars will be put into this queue only when an add 
     * car event occurs for this road.
     */
    Queue* waitingCars;

}  RoadData;

void printCar(RoadData* road);
RoadData* createRoad(int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength, int* maxLightCycleTime);
void freeLL(LLNode* head);
void freeRoad(RoadData* road);
bool moveCarsForward(RoadData* road);
bool moveCarToRoad(RoadData* currRoad, RoadData* nextRoad, int* currCycle, int* remainingCars, int* maxStep, int* carsComplete, int* totalSteps);
bool addCarToRoad(RoadData* road, int* carsRemaining);
void updateLights(RoadData** roads, int numRoads);
void printDestinations(RoadData** roads, int numRoads);
void resetCarsMoved(RoadData* road);

#endif

