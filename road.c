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
/* Suggested TODO - Write a function to create road.
 */
RoadData* createRoad(int length, int from, int to, int GreenOn, int GreenOff, int CycleReset, int* maxLightCycleTime){
    RoadData* road = (RoadData*)malloc(sizeof(RoadData));
    
    Car** cars = NULL;
    cars = (Car**)malloc(sizeof(Car*)*length);
    
    int i = 0;
    for (i = 0; i < length; i++){
        cars[i] = NULL;
    }   
    road->length = length;
    road->green = false;
    road->from = from;
    road->to = to;
    road->GreenOn = GreenOn;
    road->GreenOff = GreenOff;
    road->CycleReset = CycleReset;
    road->cars = cars;
    road->waitingCars = createQueue();
    
    if (CycleReset > *maxLightCycleTime){
        *maxLightCycleTime = CycleReset;
    }
    return road;
    
}
void freeLL(LLNode* head){
    if(head == NULL){
        return;
    }
    freeLL(head->pNext);
    free(head->qt);
    head->qt = NULL;
    free(head);
    head = NULL;
}
/* Suggested TODO - Write a function to free a road.
 */
void freeRoad(RoadData* road){
    int i = 0;
    for (i = 0; i < road->length; i++){
        free(road->cars[i]);
    }
    free(road->cars);
    freeQueue(road->waitingCars);
    freeLL(road->waitingCars->qFront);
    free(road);
}

/* Suggested TODO - Write a function that attempts to move the cars on a given road forward (if space ahead of them was empty).
 */
bool moveCarsForward(RoadData* road){
    int i;
    bool result = false;
    
    for(i=road->length-1;i > 0;i--){
        if(road->cars[i] != NULL && road->cars[i -1] == NULL && road->cars[i]->moved == false){
            
            road->cars[i-1] = road->cars[i];
            road->cars[i ] = NULL;
            result = true;
            road->cars[i-1]->moved = true;
        }
        
    }
    
    return result;
}

/* Suggested TODO - Write a function to attempt to move the car at the first position on a given road
 * to the last position of a another given road.
 */
//res1 = moveCarToRoad( pTrafficData->roads[i], nextRoad, &cycleCount, &remainingCars, &maxStep, &carsCompleted, &totalSteps);

bool moveCarToRoad(RoadData* currRoad, RoadData* nextRoad, int* currCycle, int* remainingCars, int* maxStep, int* carsComplete, int* totalSteps){
    int carStepsResult = 0;
    if(nextRoad == NULL){
        carStepsResult = (*currCycle - currRoad->cars[0]->stepAdded) +1;

        if(carStepsResult > *maxStep){
            *maxStep = carStepsResult;
        }
        *totalSteps = *totalSteps + carStepsResult;
        
        printf("STEP %d - CAR successfully traveled from %d to %d in %d time steps.\n", *currCycle, currRoad->cars[0]->origin, currRoad->to, carStepsResult);

        ++*carsComplete;
        --*remainingCars;
        currRoad->cars[0] = NULL;
        free(currRoad->cars[0]);
        return true;
    }
    else if(nextRoad->cars[(nextRoad->length)-1] == NULL){
        nextRoad->cars[(nextRoad->length)-1] = currRoad->cars[0];
        currRoad->cars[0] = NULL;
        nextRoad->cars[(nextRoad->length)-1]->moved = true;
        return true;
    }
    else{
        return false;
    }
}
/* Suggested TODO - Write a function to attempt to move a car from the add car queue onto the end of this road.*/
bool addCarToRoad(RoadData* road, int* carsRemaining){
    if(!isEmpty(road->waitingCars) && road->cars[(road->length)-1] == NULL){
        road->cars[(road->length) - 1] = dequeue(road->waitingCars);
        road->cars[(road->length) - 1]->moved = true;
        ++*carsRemaining;
        return true;
    }
    else{
        return false;
    }
}
/* Suggested TODO - Write a function to update the current state of the traffic light based on the current cycle.
 */
void updateLights(RoadData** roads, int numRoads){
    int i = 0;
    
    for (i = 0; i < numRoads; i++){
        if (roads[i]->CycleReset == roads[i]->TimeStep){
            roads[i]->TimeStep = 0;
        }
        if(roads[i]->GreenOn == roads[i]->TimeStep ){
            roads[i]->green = true;
            

        }
        if(roads[i]->GreenOff == roads[i]->TimeStep ){
            roads[i]->green = false;
            
        }
        roads[i]->TimeStep++;
        
        
    }
}

void resetCarsMoved(RoadData* road){
    int i;
    for(i = 0; i < road->length; i++){
        if (road->cars[i] != NULL){
            road->cars[i]->moved = false;
        }
        
    }
    
}
/* Suggested TODO - Write a function to print the destinations of the cars on the road
 */
void printDestinations( RoadData** roads, int numRoads )
{
    
    int i, j;
    for( i = 0; i < numRoads; i++){

        printf("Cars on the road from %d to %d: \n", roads[i]->from, roads[i]->to);

        for(j=0; j<roads[i]->length; j++){
            if(roads[i]->cars[j] != NULL){
                printf("%d ", roads[i]->cars[j]->destination);
            }
            else{
                printf("- ");
            }
        }
        if (roads[i]->green == true){
            printf("(GREEN light)");
        }
        else{
            printf("(RED light)");
        }
        printf("\n");
    }
    printf("\n");
}

