#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <ctime>
#include <algorithm>
#include <unistd.h>

#define MAX_ITERATIONS 10000
#define MAX_TASKS 300 // THIS... IS... SPARTAAAAAAA !!!
#define MAX_MACHINES 70
#define LAST_TASK(i) (atribs[(i)][N_TASKS]-1)
#define N_TASKS  MAX_TASKS - 1
#define MACHINE_CICLE MAX_TASKS - 2

typedef struct _candidate {
    int value;
    int index;
}Candidate;

int graph[MAX_TASKS][MAX_TASKS];
int costs[MAX_TASKS];
int n, m, currentSolution, cicle;
int atribs[MAX_MACHINES][MAX_TASKS];
int degree[MAX_TASKS], topSorted[MAX_TASKS];

Candidate rcl[MAX_TASKS];

float randomization = 1;

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

void insertRcl(Candidate c, int rclSize);
void printRcl(int rclSize);
void initRcl(int rclSize);

//void amain() {

//    int rclSize = n * randomization;
//    initRcl(rclSize);

//    Candidate c1 = {1,0}, c2 = {2,1} , c3 = {0,3}, c4 = {-1,4} ,c5 = {10,5}, c6 = {3,6};
//    insertRcl(c1,rclSize);
//        printRcl(rclSize);
//    insertRcl(c2,rclSize);
//        printRcl(rclSize);
//    insertRcl(c3,rclSize);
//            printRcl(rclSize);
//    insertRcl(c4,rclSize);
//            printRcl(rclSize);
//    insertRcl(c5,rclSize);
//    printRcl(rclSize);
//      insertRcl(c6,rclSize);

//    //insertRcl(c4,rclSize);
//    printRcl(rclSize);
//}

//int amain() {

//    printf("1,0 %d\n",canDelegate(1,0));
//    printf("0,0 %d\n",canDelegate(0,0));
//    atribs[0][0] = 0;
//    atribs[0][N_TASKS]++;
//    printf("2,1 %d\n",canDelegate(2,1));
//    printf("1,1 %d\n",canDelegate(1,1));
//    atribs[1][0] = 1;
//    atribs[1][N_TASKS]++;
//    printf("2,1 %d\n",canDelegate(2,1));
//    printf("7,0 %d\n",canDelegate(7,0));

//}

int main(int argc, char**argv) {

	if(argc < 2) {
		printf("Missing arguments. Use it like this:\n./bl number_of_machines [< input_file]\n");
		exit(1);
	}
	if(argc > 2) {
		printf("Too many arguments. Use it like this:\n./bl number_of_machines [< input_file]\n");
		exit(1);
	}

	m = atoi(argv[1]);
	init();

	readInput();
	printInput();

	for(int it = 0; it < MAX_ITERATIONS; it++) {
        //greedyFialho();
		randomGreedy();
		//greedySoia();

   		localSearch();
		updateSolution();
        //printAtribs();

	}

	//int localCicle = longestCicle();

	printf("Menor tempo de cicle: %i\n", cicle);
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

int comp (const void *e1, const void *e2) { return -(*(int*)e2 - *(int*)e1); }

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
        if ( c.value <= rcl[i].value ) {
            // shifts everyone to the front
            memmove( rcl+i+1, rcl+i , (rclSize - i)*sizeof(int) );
            rcl[i] = c;
            inserted = 1;
        }
    }

}

void initRcl(int rclSize) {

    for( int i=0; i < rclSize; i++) {
        rcl[i].value = INT_MAX;
    }

}

void printRcl(int rclSize) {

    for (int i = 0; i < rclSize; i++) {
        printf("[%d]\n",rcl[i].value);
    }

}

void randomGreedy() {

    int rclSize = n * randomization;
    initRcl(rclSize);

    //clean atributions
	for(int i = 0; i < MAX_TASKS; i++) {
		for(int j = 0; j < MAX_MACHINES; j++)
			atribs[j][i] = 0;
	}

	for(int i = 0; i < n; i++) {

        int task = i;

		for(int j = 0; j < m; j++) {

            if ( canDelegate( task , j ) ) {

                printRcl(rclSize);

                int new_cicle = atribs[j][MACHINE_CICLE] + costs[task];
                int localCicle = 0;

//                // calculates the longest cicle between all machines
//                for(int k = 0; k < m; k++) {

//                    int current_cicle = atribs[k][MACHINE_CICLE];

//                    if (k == j) {
//                        current_cicle = new_cicle;
//                    }

//	                if( current_cicle > localCicle) {
//		                localCicle = current_cicle;
//	                }
//                }

                Candidate local = { new_cicle, j };
                insertRcl(local,rclSize);

            } else {
                printf("Couldnt delegate\n");
            }

		}

        int index = rcl[i].index;
        printf("rclSize: %d\n",rclSize);
        exit(1);

        atribs[index][N_TASKS]++;
        atribs[index][LAST_TASK(index)] = task;
        atribs[index][MACHINE_CICLE] += costs[task];

	}
}

void localSearch() {
	int worstIndex, localCicle;
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

		for(int i = 0; i < m; i++) {
		    // if we're not looking in the same machine and also if we can
		    // delegate the last task of the worst machine to this new one
			if(i != worstIndex && canDelegate(atribs[worstIndex][LAST_TASK(worstIndex)] , i) ) {
			    int cost = costs[ atribs[worstIndex][LAST_TASK(worstIndex)] ];

				if(atribs[i][MACHINE_CICLE] + cost < atribs[worstIndex][MACHINE_CICLE]) {
					// GET CICLES RIGHT
					atribs[i][MACHINE_CICLE] += cost;
					atribs[worstIndex][MACHINE_CICLE] -= cost;

					atribs[i][N_TASKS]++;
					atribs[i][LAST_TASK(i)] = atribs[worstIndex][LAST_TASK(worstIndex)];
					atribs[worstIndex][N_TASKS]--;
					currentSolution = atribs[i][MACHINE_CICLE];
					improved = true;
				}
			}
		}
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
		int node = rand() % (top + 1);
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
	scanf("%i,%i",&n,&m);

	for(int i = 0; i < n; i++)
		scanf("%i", &costs[i]);

	int i,j;
	scanf("%i,%i", &i, &j);

	while(i != -1 && j != -1) {
		graph[i-1][j-1] = 1;
		degree[j-1]++;
		scanf("%i,%i", &i, &j);
	}
}

void init() {

	n = 0;
	currentSolution = INT_MAX;
	cicle = INT_MAX;

	srand(time(NULL));

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

    for (unsigned int i = 0; i < m; i += 1) {
        printf("%d: ",i);
        for (unsigned int j = 0; j < atribs[i][N_TASKS]; j += 1) {
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
    for (unsigned int i = 0; i < n; i += 1) {
        printf("%d: %d\n",i,costs[i]);
    }
}

