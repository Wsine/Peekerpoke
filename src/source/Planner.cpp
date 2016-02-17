/*
 * Planner.cpp
 *
 *  Created on: 2016.1.5
 *  Author: liboyang
 */
#include "Planner.h"


point Path[50];
point NextPosition;
bool isvisit[10][10];
int step;
bool flag;

//EAST WEST SOUTH NORTH
point oper[4] = { {0, -1}, {0, 1}, {1, 0}, {-1, 0}};

void DFS(point Start,  point End)  {
	point P;
    if (Start.x == End.x && Start.y == End.y)  {
    	int i;
    	for (i = 0 ; i < step ; i++)  {
    	    cout << "(" << Path[i].x << ", " << Path[i].y << ")";
    	}
		flag = true;
		NextPosition.x = Path[0].x,  NextPosition.y = Path[0].y;
		cout << endl << endl;
        return ;
    }
    else  {
        for (int i = 0; i < 4; i++)  {
            P.x = Start.x + oper[i].x;
            P.y = Start.y + oper[i].y;
            if (P.x >= 0 && P.x < 10 && P.y >= 0 && P.y < 10 && !isvisit[P.x][P.y] && Map[P.x][P.y] == 0)  {
            	Path[step].x = P.x,  Path[step].y = P.y;
            	step++;
            	isvisit[P.x][P.y] = true;
            	P.step = Start.step + 1;
            	DFS(P,  End);
            	step--;
            }
        }
    }
}
point GetNextPosition() {
	if (flag == true) {
		return NextPosition;
	}
	else {
		cout << "NO ROUTE!" << endl;
		point p;
		p.x = 0,  p.y = 0;
		return p;
	}
}
void MapInitial()  {	
	int shm_id;
	key_t key;
	int *MapMemory;
	char temp[8];
	char pathname[30];
	strcpy(pathname, "/tmp");
	key = ftok(pathname,0x03);
	if  (key == -1)  {
		perror("ftok error\n");
		return;
	}
	printf("key = %d\n",  key);
	shm_id = shmget(key,8192, IPC_CREAT|IPC_EXCL|0600);
	if  (shm_id == -1)  {
		perror("shmget error\n");
		return;
	}
	printf("shm_id = %d\n",  shm_id);
	MapMemory = (int*)shmat(shm_id,  NULL,  0);
	memset(temp,  0x00,  sizeof(temp));
	strcpy(temp,  "test");
	int j;
	for (j = 0; j < 100 ; j++) {
		MapMemory[j] = 1;
	}
	for (j = 0 ; j < 10 ; j++) {
		MapMemory[j * 10 + 4] = 0;
		MapMemory[40 + j] = 0;
	}
	int i;
	for (i = 0 ; i < 100 ; i++) {
		Map[i/10][i%10] = MapMemory[i];
	}
}

void MapReferesh() {
	int shm_id;
	key_t key;
	int *MapMemory;
	char pathname[30] ;
	strcpy(pathname,"/tmp") ;
	key = ftok(pathname,0x03);
	if (key == -1) {
		perror("ftok error");
		return;
	}
	shm_id = shmget(key,0, 0);   
	if (shm_id == -1) {
		perror("shmget error");
		return;
	}
	MapMemory = (int*)shmat(shm_id,NULL,0);
	int i;
	for (i = 0 ; i < 100 ; i++) {
		Map[i/10][i%10] = MapMemory[i];
	}
	for (i = 0 ; i < 100 ; i++) {
		Map[i/10][i%10] = MapMemory[i];
	}
	if (shmdt(MapMemory) == -1) {
		perror("detach error\n");
		return;
	}
}

int IfNextValuable(point next) {
	if (Map[next.x][next.y] == 0) {
		return 1;
	}
	return 0;
}

void print()  {
	cout << "Current map:" << endl;
	int i,  j;
	for (i = 0 ; i < 10 ; i++)  {
		for (j = 0 ; j < 10 ; j++)  {
			cout << Map[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
}

void Referesh(point Start,  point End)  {
	int i,  j;
	for (i = 0 ; i < 10 ; i++)  {
		for (j = 0 ; j < 10 ; j++)  {
			isvisit[i][j] = false;
		}
	}
	flag = false;
	step = 0;
	isvisit[Start.x][Start.y] = true;
	Start.step = 1;
}
