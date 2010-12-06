#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <ctime>
#include <algorithm>
#include <cassert>
#include <cmath>

#define MAX_ITERATIONS 100000
#define MAX_TASKS 300 // THIS... IS... SPARTAAAAAAA !!!
#define MAX_MACHINES 70
#define LAST_TASK(i) (atribs[(i)][N_TASKS]-1)
#define N_TASKS  MAX_TASKS - 1
#define MACHINE_CICLE MAX_TASKS - 2

typedef struct _candidate {
    int value;
    int index;
}Candidate;

/*
    GLOBAL VARIABLES
*/

int graph[MAX_TASKS][MAX_TASKS];
int costs[MAX_TASKS];
int n, m, currentSolution, cicle;
int atribs[MAX_MACHINES][MAX_TASKS];
int degree[MAX_TASKS], topSorted[MAX_TASKS];
int alpha = 0;

Candidate rcl[MAX_MACHINES];
int numCandidates = 5;
int seed = time(NULL);
int isSoia;

/*
    FUNCTIONS
    //TODO separate them
*/
void init();
void readInput();
void topSort();
void printInput();
void randomGreedy();
void localSearch();
void updateSolution();
void printAtribs();
void printTop();
void printCosts();
void greedySoia();
void greedyFialho();

int longestCicle();
int canDelegate(int task,int machine);

/*
    RCL
*/
void insertRcl(Candidate c, int rclSize);
void printRcl(int rclSize);
void initRcl(int rclSize);

int main(int argc, char**argv) {

    const char* message = "Missing arguments. Use it like this:\n\
            ./bl num_machines alpha type [seed] [< input_file]\n";
	if(argc < 4) {
		puts(message);
		exit(1);
	}
	if(argc > 5) {
		puts(message);
		exit(1);
	}

	m = atoi(argv[1]);
	alpha = atoi(argv[2]);
    isSoia = atoi(argv[3]);

    seed = argc == 5 ? atoi(argv[4]) : time(NULL);

	init();

	readInput();
	//printInput();

	for(int it = 0; it < MAX_ITERATIONS; it++) {

		if (isSoia)
		    greedySoia();
        else
            randomGreedy();

   		localSearch();
		updateSolution();
        //printAtribs();

	}

	//int localCicle = longestCicle();

	printf("%i\n", cicle);
}

int longestCicle() {

    int cicle = 0;
	for(int i = 0; i < m; i++) {
		if(atribs[i][MACHINE_CICLE] > cicle) {
			cicle = atribs[i][MACHINE_CICLE];
		}
	}
	return cicle;
}

int canDelegate(int task, int machine) {

    int can = 1;

    for (int i=0; i < n; i++) {
        if (graph[i][task]) {
            int taskCan = 0;
            //search in each machine that comes before it
            for (int j = 0; j <= machine; j++) {
                 for (int k=0; k < atribs[j][N_TASKS]; k++) {
                    taskCan = taskCan || atribs[j][k] == i;
                  }
            }
            can = can && taskCan;
        }
     }

    return can;
}

int comp (const void *e1, const void *e2) { return (*(int*)e2 - *(int*)e1); }

void greedyFialho() {
    qsort(costs,n, sizeof(int) , comp );
    randomGreedy();
}

void greedySoia() {
	topSort();
	//printTop();
	for(int i = 0; i < MAX_TASKS; i++) {
		for(int j = 0; j < MAX_MACHINES; j++)
			atribs[j][i] = 0;
	}

	int totalCost = 0, highestCost = 0, minPossibleCicle;

	for(int i = 0; i < n; i++) {
		totalCost+=costs[i];
		if(costs[i] > highestCost)
			highestCost = costs[i];
	}


	minPossibleCicle = std::max(highestCost, (int)(totalCost / m));
	//printf("minPossibleCicle: %d\n", minPossibleCicle);
	int currentTask = 0;

	for(int i = 0; i < m; i++) {
		int task = topSorted[currentTask];
		while(currentTask < n && atribs[i][MACHINE_CICLE] + costs[task] < minPossibleCicle) {

			atribs[i][N_TASKS]++;
			atribs[i][LAST_TASK(i)] = task;
			atribs[i][MACHINE_CICLE]+=costs[task];
			task = topSorted[++currentTask];
		}

		if(currentTask < n && (atribs[i][MACHINE_CICLE] + costs[task] - minPossibleCicle < minPossibleCicle - atribs[i][MACHINE_CICLE])) {
			atribs[i][N_TASKS]++;
			atribs[i][LAST_TASK(i)] = task;
			atribs[i][MACHINE_CICLE]+=costs[task];
			currentTask++;
		}
	}

	for(int i = currentTask; i < n; i++) {
		atribs[m-1][N_TASKS]++;
		atribs[m-1][LAST_TASK(m-1)] = topSorted[i];
		atribs[m-1][MACHINE_CICLE]+=costs[topSorted[i]];
	}
}

void insertRcl(Candidate c, int rclSize) {

    bool inserted = 0;

    for (int i=0; i < rclSize && !inserted ; i++) {
        if ( c.value < rcl[i].value ||
             (c.value == rcl[i].value && c.index <= rcl[i].index) )
           {
            // shifts everyone to the front
            memmove( rcl+i+1, rcl+i , (rclSize - i)*sizeof(Candidate) );
            rcl[i] = c;
            inserted = 1;
        }
    }

}

void initRcl(int rclSize) {

    for( int i=0; i < rclSize; i++) {
        rcl[i].value = INT_MAX;
        rcl[i].index = INT_MAX;
    }

}

void printRcl(int rclSize) {

    for (int i = 0; i < rclSize; i++) {
        printf("[%d,%d]\n",rcl[i].value,rcl[i].index);
    }

}

void randomGreedy() {

    int rclSize = 2;//ceil(m * randomization);

    //clean atributions
	for(int i = 0; i < MAX_TASKS; i++) {
		for(int j = 0; j < MAX_MACHINES; j++)
			atribs[j][i] = 0;
	}

    int numDelegated;

	for(int i = 0; i < n; i++) {
        initRcl(rclSize);
        numDelegated = 0;

		for(int j = 0; j < m; j++) {

            if ( canDelegate( i , j ) ) {
                //printf("Can delegate\n");
                numDelegated++;

                int new_cicle = atribs[j][MACHINE_CICLE] + costs[i];

                int localCicle = 0;

                // calculates the longest cicle between all machines
                for(int k = 0; k < m; k++) {
                    int current_cicle = atribs[k][MACHINE_CICLE];

                    if (k == j) {
                        current_cicle = new_cicle;
                    }

	                if( current_cicle > localCicle) {
		                localCicle = current_cicle;
	                }
                }

                Candidate local = { localCicle, j };
                insertRcl(local,rclSize);

            } else {
            //    printf("Couldnt delegate task %d in machine %d\n",i,j);
            //    printAtribs();
            }

		}

        if (numDelegated == 0) {
            printf("tarefa %d\n",i);
            printAtribs();
        }

        assert(numDelegated > 0);
        int size = std::min(numDelegated,rclSize);

        //printf("Size: %d\n",size);
        int chosen = rcl[rand()%size].index;
        //printf("Chosen: %d\n",chosen);
        //printRcl(rclSize);

        //printf("rclSize: %d\n",rclSize);
        //printRcl(rclSize);

        //printf("Index: %d\n",index);

        atribs[chosen][N_TASKS]++;
        atribs[chosen][LAST_TASK(chosen)] = i;
        atribs[chosen][MACHINE_CICLE] += costs[i];
        //printAtribs();

	}

}

void localSearch() {
	int worstIndex = 0, localCicle;
	bool improved;

	do {
	    //printf(".");
		improved = false;
		localCicle = 0;

		// calculates the longest cicle between all machines
		for(int i = 0; i < m; i++) {
			if(atribs[i][MACHINE_CICLE] > localCicle) {
				localCicle = atribs[i][MACHINE_CICLE];
				worstIndex = i;
			}
		}

		currentSolution = localCicle;

		for(int k = 0; k < atribs[worstIndex][N_TASKS] && !improved; k++) {

			for(int i = 0; i < m && !improved; i++) {
				int task = atribs[worstIndex][k];
				// if we're not looking in the same machine and also if we can
				// delegate the last task of the worst machine to this new one
				if(i != worstIndex && canDelegate(task, i) ) {
					int cost = costs[ task ];

					if(atribs[i][MACHINE_CICLE] + cost < atribs[worstIndex][MACHINE_CICLE]) {
						// GET CICLES RIGHT
						atribs[i][MACHINE_CICLE] += cost;
						atribs[worstIndex][MACHINE_CICLE] -= cost;

						atribs[i][N_TASKS]++;
						atribs[i][LAST_TASK(i)] = task;

						for(int j = k+1; j < atribs[worstIndex][N_TASKS]; j++)
							atribs[worstIndex][j-1] = atribs[worstIndex][j];
						atribs[worstIndex][N_TASKS]--;

						//currentSolution = atribs[i][MACHINE_CICLE];
						improved = true;
					}
				}
			}
		}

		currentSolution = longestCicle();
	} while(improved);
}

void updateSolution() {
	if(currentSolution < cicle)
		cicle = currentSolution;
}

void topSort() {
	int stack[MAX_TASKS], tDegree[MAX_TASKS], top, count, r, i, k;

	top = -1;
	count = 0;
	r = 0;

	for(i=0;i<n;i++)
		tDegree[i]=degree[i];

	for(i=0;i<n;i++)
		if(!tDegree[i])
			stack[++top]=i;


	while(top>-1)
	{
		int node;
		if(alpha>0)
			node = rand() % (int)ceil((top + 1) * alpha/100.);
		else
			node = 0;
		i=stack[node];
		for(int b = node+1; b <= top; b++) {
			stack[b-1] = stack[b];
		}
		top--;
		//printf("%i ",i);
		topSorted[r++] = i;
		count++;
		for(k = 0;k < n;k++)
		{
			if(graph[i][k])
				if(!(--tDegree[k]))
					stack[++top]=k;
		}
	}

	if(count < n) {
		printf("Impossible to sort topologicaly\n");
		exit(1);
	}
}


void readInput() {
	int read;

	read = scanf("%i,%i",&n,&m);

	for(int i = 0; i < n; i++)
		read = scanf("%i", &costs[i]);

	int i,j;
	read = scanf("%i,%i", &i, &j);

	while(i != -1 && j != -1) {
		graph[i-1][j-1] = 1;
		degree[j-1]++;
		read = scanf("%i,%i", &i, &j);
	}
}

void init() {

	n = 0;
	currentSolution = INT_MAX;
	cicle = INT_MAX;

	srand(seed);

	for(int i = 0; i < MAX_TASKS; i++) {
		for(int j = 0; j < MAX_TASKS; j++)
			graph[i][j] = 0;

		costs[i] = 0;
		degree[i] = 0;

		for(int j = 0; j < MAX_MACHINES; j++)
			atribs[j][i] = 0;
	}
}

void printAtribs() {

    for (int i = 0; i < m; i += 1) {
        printf("%d: ",i);
        for (int j = 0; j < atribs[i][N_TASKS]; j += 1) {
            printf(" %d ",atribs[i][j]);
        }
        printf("\n");
    }

}

void printInput() {

	printf("Graph:\n");
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++)
			printf("%i ", graph[i][j]);
		printf("\n");
	}

	printf("Costs:\n");
	for(int i = 0; i < n; i++)
		printf("%i: %i\n", i, costs[i]);
}

void printTop() {
	printf("Topological order:\n");
	for(int i = 0; i < n; ++i)
			printf("%i ", topSorted[i]);
	printf("\n");
}

void printCosts() {
    for (int i = 0; i < n; i += 1) {
        printf("%d: %d\n",i,costs[i]);
    }
}

