#include <cstdio>
#include <cstdlib>
#include <climits>
#include <ctime>

#define MAX_ITERATIONS 10
#define MAX_TASKS 300
#define MAX_MACHINES 70
#define N_TASKS MAX_TASKS - 1
#define MACHINE_CICLE MAX_TASKS - 2

int graph[MAX_TASKS][MAX_TASKS]; // THIS... IS... SPARTAAAAAAA !!!
int costs[MAX_TASKS];
int n, m, currentSolution, cicle;

int atribs[MAX_MACHINES][MAX_TASKS];

void init();
void readInput();
void printInput();
void randomGreedy();
void localSearch();
void updateSolution();

int main() {
	
	init();
	
	readInput();
	//printInput();
	
	for(int i = 0; i < MAX_ITERATIONS; i++) {
		randomGreedy();
		localSearch();
		updateSolution();
	}
	
	printf("Menor tempo de cicle: %i\n", cicle);
}


void randomGreedy() {
	
}

void localSearch() {
	int worstIndex, localCicle;
	bool improved, severalWorst;
	
	do {
		improved = false;
		severalWorst = false;
		localCicle = 0
		
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
		
		// if more then 1 machine shares the longest cicle, doesn't have better neighbor
		if(severalWorst)
			return;
		
		for(int i = 0; i < m; i++) {
			if(i != worstIndex) {
				if(canDelegate(atribs[worstIndex][N_TASKS-1],i) {
					if(atribs[i][MACHINE_CICLE] + costs[atribs[worstIndex][N_TASKS-1]] < atribs[worstIndex][MACHINE_CICLE]) {
						atribs[i][N_TASKS] = atribs[worstIndex][N_TASKS-1];
						atribs[i][N_TASKS]++;
						atribs[worstIndex][N_TASKS]--;
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


void readInput() {
	scanf("%i",&n);
	
	for(int i = 0; i < n; i++)
		scanf("%i", &costs[i]);
	
	int i,j;
	scanf("%i,%i", &i, &j);
	
	while(i != -1 && j != -1) {
		graph[i-1][j-1] = 1;
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
		
		for(int j = 0; j < MAX_MACHINES; j++)
			atribs[j][i] = 0;
	}
}



void printInput() {
	
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++)
			printf("%i ", graph[i][j]);
		printf("\n");
	}
	
	for(int i = 0; i < n; i++)
		printf("%i: %i\n", i, costs[i]);
}
