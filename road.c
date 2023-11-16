#include "road.h"

/* Suggested TODO - Write a function to create road.
 */

/* Suggested TODO - Write a function to free a road.
 */

/* Suggested TODO - Write a function that attempts to move the cars on a given road forward (if space ahead of them was empty).
 */

/* Suggested TODO - Write a function to attempt to move the car at the first position on a given road
 * to the last position of a another given road.
 */

/* Suggested TODO - Write a function to attempt to move a car from the add car queue onto the end of this road.
 */

/* Suggested TODO - Write a function to update the current state of the traffic light based on the current cycle.
 */

/* Suggested TODO - Write a function to print the destinations of the cars on the road
 */
//printf("Cars on the road from %d to %d: \n", ????, ????);
void printCar(RoadData* road){
    int i;
    for(i=0; i<road->length; i++){
        if(road->cars[i] != NULL){
            printf("Car %d: %d\n", i, road->cars[i]->destination);
        }
    }
}
RoadData* createRoad(int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength){
    RoadData* road = (RoadData*)malloc(sizeof(RoadData));
    road->length = length;
    road->green = false;
    road->start = from;
    road->cars = (Car**)malloc(sizeof(Car*)*length);
    road->waitingCars = createQueue();
    return road;
}
void freeRoad(RoadData* road){
    free(road->cars);
    freeQueue(road->waitingCars);
    free(road);
}
void moveCarsForward(RoadData* road){
    int i;
    for(i=0; i<road->length; i++){
        if(road->cars[i] != NULL){
            if(i == road->length-1){
                road->cars[i] = NULL;
            }
            else{
                road->cars[i+1] = road->cars[i];
                road->cars[i] = NULL;
            }
        }
    }
}
void moveCarToRoad(Car* car, RoadData* road){
    int i;
    for(i=0; i<road->length; i++){
        if(road->cars[i] == NULL){
            road->cars[i] = car;
            break;
        }
    }
}
void addCarToRoad(Car* car, RoadData* road){
    enqueue(road->waitingCars, car);
}
void updateLight(RoadData* road, int cycle);
void printDestinations(RoadData* road){
    printf("Cars on the road from %d to %d: \n", road->start, road->length);
    int i;
    for(i=0; i<road->length; i++){
        if(road->cars[i] != NULL){
            printf("Car %d: %d\n", i, road->cars[i]->destination);
        }
    }
}

