#include "trafficSimulator.h"

void printRoadData( int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength );
int max( int a, int b );

/* printNames
 * input: none
 * output: none
 *
 * Prints names of the students who worked on this solution
 * REMINDER: You may only work with one other cs2124 student
 */
void printNames( )
{
    /* TODO : Fill in you and your partner's names (or N/A if you worked individually) */
    printf("\nThis solution was completed by:\n");
    printf("Soren Watterson\n");
    printf("N/A\n\n");
}

/* createTrafficData
 * input: char* filename of file to read
 * output: TrafficData* which stores the data of this road network
 *
 * Create a new TrafficData variable and read in the traffic data from the file whose name is stored in char* filename
 */
TrafficData* createTrafficData( char* filename )
{
    /* open the file */
    FILE *pFile = fopen( filename, "r" );
    if ( pFile == NULL )
    {
        printf( "Error opening file\n" );
        return NULL;
    }
    int numIntersections;
    int numRoads;
    fscanf( pFile, "%d %d", &numIntersections, &numRoads );

    TrafficData* trafficData = (TrafficData*)malloc(sizeof(TrafficData));
    trafficData->roadNetwork = createGraph(numIntersections);
    
    trafficData->numCars = 0; 
    trafficData->numRoads = numRoads;

    int* maxLightcycleTime = (int*)malloc(sizeof(int));
    *maxLightcycleTime = -1;
    trafficData->maxLightCycleTime = *maxLightcycleTime;

    trafficData->eventQueue = createPQ();
    trafficData->roads = (RoadData**)malloc(sizeof(RoadData*) * numRoads);
    
    int incomingRoads, from, length, GreenOn, GreenOff, CycleReset;
    int i, j;
    int tempRoadCount = 0;
    printf("Add the roads:\n");
    for (i = 0; i < numIntersections ; i++){

        fscanf( pFile, "%d", &incomingRoads );

        for (j = 0; j < incomingRoads; j++){
            fscanf( pFile, "%d %d %d %d %d", &from, &length, &GreenOn, &GreenOff, &CycleReset );

            // RoadData* createRoad(int length, int from, int to, int GreenOn, int GreenOff, int CycleReset)
            RoadData* road = createRoad(length, from, i, GreenOn, GreenOff, CycleReset, &trafficData->maxLightCycleTime);
            trafficData->roads[tempRoadCount] = road;
            
            tempRoadCount++;

            
            // add roads to graph here
            setEdge(trafficData->roadNetwork, from, i, length);
            setEdgeData(trafficData->roadNetwork, from, i, road);

            printRoadData(length, from, i, GreenOn, GreenOff, CycleReset);


        }
    }

    int numCarCommands;
    int to, carTimeStep, numCars, destination;

    fscanf( pFile, "%d", &numCarCommands );
    printf("Add the cars events:\n");
    for (i = 0; i < numCarCommands; i++){
        fscanf( pFile, "%d %d %d ", &from, &to, &carTimeStep );
        fscanf( pFile, "%d", &numCars);
        RoadData* temp = getEdgeData(trafficData->roadNetwork, from, to);
        Event* addCar = createAddCarEvent(carTimeStep, temp);//trafficData->roads[from]
        printf("Created Event for time step %d on road from %d to %d\n", carTimeStep, from, to);
        printf("Destinations of added cars:");
        
        for (j = 0; j < numCars; j++){
            fscanf( pFile, "%d", &destination );
            Car* car = createCar(carTimeStep, from, to, destination);
            trafficData->numCars++;
            
            enqueue(addCar->pCarQueue, car);

            printf(" %d", destination);
            if (j < numCars - 1){
                printf(", ");
            }
        }
        printf("\n\n");
        enqueueByPriority(trafficData->eventQueue, addCar, carTimeStep);
    }

    int numPrintCommands;
    int printCarTimeStep = 0;
    fscanf( pFile, "%d", &numPrintCommands );
    printf("Add the roads events:\nTime steps: ");
    for (i = 0; i < numPrintCommands; i++){
        fscanf( pFile, "%d", &printCarTimeStep );
        printf("%d", printCarTimeStep);
        if (i < numPrintCommands - 1){
            printf(", ");
        }
        Event* printCar =  createPrintRoadsEvent(printCarTimeStep);

        enqueueByPriority(trafficData->eventQueue, printCar, printCarTimeStep);
        // print the cars at this time step
    }
    printf("\n");
    fclose(pFile);
    
    /* TODO: read in all the data in pFile */
    /* HINT: use fscanf( pFile, "%d", &<your int variable here> ) to read an int from pFile */
    /* HINTs:
     * Each road can be stored in a `RoadData` struct (see `road.h`).
     *
     * You'll want to store all of these `RoadData` structs in a single array.  This is important to track the order that
     * roads were added to your graph.  These are also the order they are processed in each time step.  You can store this
     * array in your TrafficData struct.
     *
     * You'll add edges to a graph to represent each road using `setEdge` (see `graph.c`).  The vertices of the graph
     * represent intersections.  The graph will do the work of finding shortest paths (see `getNextOnShortestPath` in graph.c
     * and the longest shortest path problem on Blackboard).
     *
     * It will also help to add your `RoadData` structs to your graph using `setEdgeData` (see `graph.c`).   You can then
     * retrieve that `RoadData` struct by calling `getEdgeData`.  This will be helpful for finding roads when cars pass
     * through intersections.
     *
     * Each event will be stored in an Event struct (see `event.h`).
     *
     * It's recommended to store all of the `Event` structs in a single priority queue (using time step as the priority).  This
     * allows you to quickly find the next event to execute by time step (see `enqueueByPriority`, `getFrontPriority`, and
     * `dequeuePQ`).
     *
     * Each car is stored in a Car struct (see `car.h`).
     */
    

    /* close file */

    return trafficData; /* TODO: Replace this with your TrafficData pointer */
}

/* Print color coding for printRoadData (may not work outside of Linux environment) */
#define PRINT_COLOR false

/* provided code - DO NOT CHANGE
 * printRoadData
 * input: int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength
 * output: prints the given road
 *
 * Call this function to print out the road data you read in 
 */
void printRoadData( int length, int from, int to, int greenStartTime, int greenEndTime, int lightCycleLength )
{
	int i;
	char state='R';
	
	printf("Created road from %d to %d with length %d (green=%d; red=%d; reset=%d).\n", from, to, length, greenStartTime, greenEndTime, lightCycleLength );
	printf("Cycle number: ");
	for( i=0; i<25; i++ ){
		printf("%2d ", i );
	}
	printf("...\nLight state : ");	
	if(PRINT_COLOR) 
		printf("\033[0;31m");//Color = RED
	for( i=0; i<25; i++ ){	/* print example light cycle: */
		if( i%lightCycleLength==greenStartTime ){
			if(PRINT_COLOR) 
				printf("\033[0;32m");//Color = GREEN
			state = 'G';
		}
		if( i%lightCycleLength==greenEndTime ){
			if(PRINT_COLOR) 
				printf("\033[0;31m");//Color = RED
			state = 'R';
		}
		printf("%2c ", state );
	}
	if(PRINT_COLOR) 
		printf("\033[0m");//Reset color
	printf("...\n\n");
}

/* trafficSimulator
 * input: char* name of file to read
 * output: N/A
 *
 * Simulate the road network in the given TrafficData variable
 */
void trafficSimulator( TrafficData* pTrafficData )
{
    /* TODO: complete this function */

    /* Loop until all events processed and either all cars reached destination or gridlock has occurred */

    /* Update the state of every traffic light */

    /* Loop on events associated with this time step */

    /* First try to move cars through the next intersection */

    /* Second move waiting cars onto the end of each road if possible */

    /* Third move cars forward on every road (only those that haven't moved yet this time step) */
    Event* e = NULL;
    int i = 0;
    int cycleCount = 0, remainingCars = 0, carsCompleted = 0, totalSteps = 0, maxStep = 0;
    int lastMoved = -1;
    bool res1, res2, res3 = false;

    while(!isEmptyPQ(pTrafficData->eventQueue) || remainingCars > 0){
        updateLights(pTrafficData->roads, pTrafficData->numRoads);
        while( !isEmptyPQ(pTrafficData->eventQueue) && getFrontPriority(pTrafficData->eventQueue) == cycleCount ){

            e = dequeuePQ(pTrafficData->eventQueue);
            switch (e->eventCode){
                case ADD_CAR_EVENT:
                    mergeQueues(e->pRoadData->waitingCars, e->pCarQueue);
                    printf("STEP %d - ADD_CAR_EVENT - Cars enqueued on road from %d to %d\n", e->eventTimeStep, e->pRoadData->from, e->pRoadData->to);
                    freeQueue(e->pCarQueue);
                    break;
                case PRINT_ROADS_EVENT:
                    printf("\nSTEP %d - PRINT_ROADS_EVENT - Current contents of the roads:\n", e->eventTimeStep);
                    printDestinations(pTrafficData->roads, pTrafficData->numRoads);
                    break;
                default:
                    printf("it is neither addCar or printRoads\n");
                    break;
            }
            free(e);
        }
        

        for(i = 0; i < pTrafficData->numRoads; i++)
        {
            Car* hold = pTrafficData->roads[i]->cars[0];
            RoadData* nextRoad = NULL;
            int nextT = -1;
            bool result = false;

            if(pTrafficData->roads[i]->green == true && hold != NULL)
            {
                result = getNextOnShortestPath(pTrafficData->roadNetwork, pTrafficData->roads[i]->to, hold->destination, &nextT);
                if(result)
                {
                    nextRoad = getEdgeData(pTrafficData->roadNetwork, pTrafficData->roads[i]->to, nextT); //should be... end of curr road, to
                    if(nextRoad == NULL || nextRoad->cars[nextRoad->length - 1] == NULL) //wtf
                    {
                        res1 = moveCarToRoad( pTrafficData->roads[i], nextRoad, &cycleCount, &remainingCars, &maxStep, &carsCompleted, &totalSteps);
                    }
                }
            }
        }

        for(i = 0; i < pTrafficData->numRoads; i++)
        {
            res3 = addCarToRoad(pTrafficData->roads[i], &remainingCars);
        }

        for(i = 0; i < pTrafficData->numRoads; i++)
        {
            res2 = moveCarsForward(pTrafficData->roads[i]);
        }
        
        for(i = 0; i < pTrafficData->numRoads; i++)
        {
            resetCarsMoved(pTrafficData->roads[i]);
        }

        
        

        

        
        if(res1 == false && res2 == false && res3 == false){
            lastMoved++;
        }
        else{
            lastMoved = 0;
        }

        if(lastMoved == pTrafficData->maxLightCycleTime){
            break;
        }
        
        cycleCount++;
        res1 = false;

        
        
        
    }
    if(lastMoved == pTrafficData->maxLightCycleTime){
        printf("\nSTEP %d - Gridlock has been detected.\n", cycleCount + 1);
    }
    else
    {
        printf("\nAverage number of time steps to the reach their destination is: %.2lf\n", (double)totalSteps/carsCompleted); //ONLY IF NOT GRIDLOCKD
        printf("Maximum number of time steps to the reach their destination is: %d\n", maxStep);
    }
    
    
}

/* freeTrafficData
 * input: TrafficData* pTrafficData
 * output: N/A
 *
 * Free the data in the TrafficData variable
 */
void freeTrafficData( TrafficData* pTrafficData )
{
    /* TODO: complete this function */
    free(pTrafficData->roads);
    freePQ(pTrafficData->eventQueue);
    freeGraph(pTrafficData->roadNetwork);
    free(pTrafficData);
}

int max( int a, int b )
{
    if(a>b)
        return a;
    return b;
}

