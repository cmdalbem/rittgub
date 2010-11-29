#include <cstdio>
#include <cstdlib>
#include <climits>
#include <ctime>

#define MAX_ITERATIONS 10
#define MAX_TASKS 300 // THIS... IS... SPARTAAAAAAA !!!
#define MAX_MACHINES 70
#define N_TASKS MAX_TASKS - 1
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
bool canDelegate(int task, int machine){}
void randomGreedy();
void localSearch();
void updateSolution();

int main() {
	
	init();
	
	readInput();
	topSort();
	//printInput();
	
	for(int i = 0; i < MAX_ITERATIONS; i++) {
		randomGreedy();
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
		for(int j = 0; j < m; j++) {
			
			
		}
	}
}

void localSearch() {
	int worstIndex, localCicle;
	bool improved, severalWorst;
	
	do {
		improved = false;
		severalWorst = false;
		localCicle = 0;
		
		// calculates the longest cicle between all machines
		for(int i = 0; i < m; i++) {
			if(atribs[i][MACHINE_CICLE] > localCicle) {
				localCicle = atribs[i][MACHINE_CICLE];
				worstIndex = i;
				severalWorst = false;
			}
			else if(atribs[i][MACHINE_CICLE] == localCicle)
				severalWorst = true;
		}
		
		currentSolution = localCicle;
		
		// if more then 1 machine shares the longest cicle, doesn't have better neighbor
		if(severalWorst)
			return;
		
		for(int i = 0; i < m; i++) {
			if(i != worstIndex) {
				if(canDelegate(atribs[worstIndex][N_TASKS-1],i)) {
					if(atribs[i][MACHINE_CICLE] + costs[atribs[worstIndex][N_TASKS-1]] < atribs[worstIndex][MACHINE_CICLE]) {
						atribs[i][N_TASKS] = atribs[worstIndex][N_TASKS-1];
						atribs[i][N_TASKS]++;
						atribs[worstIndex][N_TASKS]--;
						currentSolution = atribs[i][N_TASKS];
						improved = true;
					}
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
	int stack[MAX_TASKS], top, count, r, i, k;
	
	top = -1;
	count = 0;
	r = 0;
	
	for(i=0;i<n;i++)
		if(!degree[i])
			stack[++top]=i;
			

	while(top>-1)
	{
		i=stack[top--];
		//printf("%i ",i);
		topSorted[r++] = i;
		count++;
		for(k = 0;k < n;k++)
		{
			if(graph[i][k])
				if(!(--degree[k]))
					stack[++top]=k;
		}
	}
	
	if(count < n) {
		printf("Impossible to sort topologicaly\n");
		exit(1);
	}
}


void readInput() {
	scanf("%i",&n);
	
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
	
	for(int i = 0; i < MAX_TASKS; i++) {
		for(int j = 0; j < MAX_TASKS; j++)
			graph[i][j] = 0;
			
		costs[i] = 0;
		degree[i] = 0;
		
		for(int j = 0; j < MAX_MACHINES; j++)
			atribs[j][i] = 0;
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
		
	printf("Topological order:\n");
	for(int i = 0; i < n; ++i)
			printf("%i ", topSorted[i]);
	printf("\n");
}
