#include <cstdio>

#define MAX_TASKS 300
#define MAX_MACHINES 70
#define N_TASKS MAX_TASKS - 1
#define MACHINE_CICLE MAX_TASKS - 2

int graph[MAX_TASKS][MAX_TASKS]; // THIS... IS... SPARTAAAAAAA !!!
int costs[MAX_TASKS];
int n,m,cicle;

int atribs[MAX_MACHINES][MAX_TASKS];

void printInput() {
	
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++)
			printf("%i ", graph[i][j]);
		printf("\n");
	}
	
	for(int i = 0; i < n; i++)
		printf("%i: %i\n", i, costs[i]);
}

void init() {
	for(int i = 0; i < MAX_TASKS; i++) {
		for(int j = 0; j < MAX_TASKS; j++)
			graph[i][j] = 0;
			
		costs[i] = 0;
		
		for(int j = 0; j < MAX_MACHINES; j++)
			atribs[j][i] = 0;
	}
}


int main() {
	
	init();
	
	scanf("%i",&n);
	
	for(int i = 0; i < n; i++)
		scanf("%i", &costs[i]);
	
	int i,j;
	scanf("%i,%i", &i, &j);
	
	while(i != -1 && j != -1) {
		graph[i-1][j-1] = 1;
		scanf("%i,%i", &i, &j);
	}
	
	//printInput();
	
	//here goes GRASP
	
	printf("Menor tempo de cicle: %i\n", cicle);
}
