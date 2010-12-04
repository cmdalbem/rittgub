#include <cstdio>
#include <cstdlib>
#include <climits>
#include <ctime>

#define MAX_ITERATIONS 10
#define MAX_TASKS 300 // THIS... IS... SPARTAAAAAAA !!!
#define MAX_MACHINES 70
#define LAST_TASK(i) (atribs[(i)][N_TASKS]-1)
#define N_TASKS  MAX_TASKS - 1
#define MACHINE_CICLE MAX_TASKS - 2

int graph[MAX_TASKS][MAX_TASKS];
int costs[MAX_TASKS];
int n, m, currentSolution, cicle;
int atribs[MAX_MACHINES][MAX_TASKS];
int degree[MAX_TASKS], topSorted[MAX_TASKS];

void init();
void readInput();
void topSort();
void printInput();
void randomGreedy();
void localSearch();
void updateSolution();
void printAtribs();
void printTop();

int main() {

	init();

	readInput();
	//printInput();

	for(int i = 0; i < MAX_ITERATIONS; i++) {
		topSort();
		printTop();
		randomGreedy();
        printAtribs();
   		//localSearch();
		//updateSolution();
	}

	printf("Menor tempo de cicle: %i\n", cicle);
}


void randomGreedy() {
	for(int i = 0; i < MAX_TASKS; i++) {
		for(int j = 0; j < MAX_MACHINES; j++)
			atribs[j][i] = 0;
	}

	for(int i = 0; i < n; i++) {

        int task = topSorted[i];
	    int best_bet = INT_MAX;
	    int index = -1;

		for(int j = 0; j < m; j++) {

            int new_cicle = atribs[j][MACHINE_CICLE] + costs[task];
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

            if (localCicle < best_bet) {
                best_bet = localCicle;
                index = j;
            }

		}

        atribs[index][N_TASKS]++;
        atribs[index][LAST_TASK(index)] = task;
        atribs[index][MACHINE_CICLE] += costs[task];

	}
}

void localSearch() {
	int worstIndex, localCicle;
	bool improved;

	do {
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
			if(i != worstIndex) {
			    int cost = costs[ atribs[worstIndex][LAST_TASK(worstIndex)] ];

				if(atribs[i][MACHINE_CICLE] + cost < atribs[worstIndex][MACHINE_CICLE]) {
					// GET CICLES RIGHT
					atribs[i][MACHINE_CICLE] += cost;
					atribs[worstIndex][MACHINE_CICLE] -= cost;

					atribs[i][N_TASKS] = atribs[worstIndex][N_TASKS-1];
					atribs[i][N_TASKS]++;
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
	m = 0;
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

