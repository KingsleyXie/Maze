// ***********************************************************************
// Author: Kingsley(Xie Yimin)
// Last modified date: 5th March, 2017
// This program was written as my homework project due 20170305
// You can download the code through https://github.com/KingsleyXie/Maze
// Copyright © Kingsley(Xie Yimin)	All rights reserved		HeiHeiHei
// ***********************************************************************



#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <queue>

using namespace std;

#define MAX_RANGE_OF_MAZE 73
#define MIN_RANGE_OF_MAZE 3
//The maximum and minimum number of Maze Range

queue <pair<int,int> > Q;	//The queue for BFS Algorithm

int generatex=12,generatey=18,outsetx=2,outsety=1,terminalx=2*generatex,terminaly=2*generatey+1;
/*
Ranges & Outset Point & Terminal Point of Maze, these can be modified through input.
*/

int mazerangex=2*generatex+1,mazerangey=2*generatey+1;

int maze[MAX_RANGE_OF_MAZE+6][MAX_RANGE_OF_MAZE+6];	//Prepare the map of Maze
int dx[4]={0,1,0,-1},dy[4]={1,0,-1,0},paththere=0;
/*
dx[] and dy[] are offsets when take a move in a search algorithm
paththere is here to check whether the program exit after finding a way or not
*/



void ClientInput();
/*
Todo: add
*/

void printmaze(int presentx,int presenty);
/*
The function to show real-time status of Maze.
presentx and presenty are cooridinates of the current move
todo: other options
*/

void GenerateMaze(int generatex, int generatey);
/*
The function to generate Maze
Inspiration from [http://www.wutianqi.com/?p=1490]
*/

int InitialGenerate();
/*
The function to initial some data before the generation of Maze
*/

void  Search_Maze_with_BFS(int x,int y);
/*
The function to do the Breadth_First_Search Algorithm
I use Queue to simplify some processes
Actually it is not perfectly done because it will not label the incorrect way like DFS does
*/

void Search_Maze_with_DFS(int x,int y);
/*
The function to do the Depth_First_Search Algorithm
*/



int main()
{
	InitialGenerate();
	srand((unsigned)time(NULL));
	GenerateMaze(rand()%generatex+1, rand()%generatey+1);

	Search_Maze_with_DFS(outsetx,outsety);
	//Search_Maze_with_BFS(outsetx,outsety);
	if(!paththere) printf("No Way Out! You are trapped!\nHei Hei Hei\n");
	return 0;
}


void ClientInput()
{

}

void printmaze(int presentx,int presenty)
{
	printf("Searching possible way out..\n");
	for(int i=1; i<=mazerangex; i++)
		for(int j=1; j<=mazerangey; j++)
		{
			if (i==presentx&&j==presenty) printf("00");
			else
				switch(maze[i][j])
				{
					case 0: printf("  "); break;
					case 1: printf("\u2588\u2588"); break;
					case 15: printf("--"); break; 
					case 16: printf("xx"); break; 
				}
			if (j==mazerangey) printf("\n");
		}
}

void GenerateMaze(int generatex, int generatey)
{
	int doublex = generatex*2,doubley = generatey*2;
	int phase = rand()%2 ? 1 : 3;

	maze[doublex][doubley] = 0;

	for(int i=0, step=rand()%4; i<4; i++, step=(step+phase)%4)
			if (doubley+dy[step]-1!=mazerangey&&maze[doublex+2*dx[step]][doubley+2*dy[step]])
			{
				maze[doublex+dx[step]][doubley+dy[step]] = 0;
				GenerateMaze(generatex+dx[step], generatey+dy[step]);
			}
}


int InitialGenerate()
{
	for(int i=0; i<=generatex*2+2; i++)
		for(int j=0; j<=generatey*2+2; ++j)
			maze[i][j] = 1;

	for(int i=0, j=2*generatex+2; i<=2*generatey+2; i++)
		{ maze[0][i] = 0; maze[j][i] = 0; }
	for(int i=0, j=2*generatey+2; i<=2*generatex+2; i++)
		{ maze[i][0] = 0; maze[i][j] = 0; }

	maze[outsetx][outsety]=15; maze[terminalx][terminaly]=0;
}

void  Search_Maze_with_BFS(int x,int y)
{
	Q.push(make_pair(x,y));
	while(!Q.empty())
	{
		x=Q.front().first; y=Q.front().second; Q.pop();
		for(int step=0; step<4; step++)
		{
			if ((x+dx[step]>0)&&(x+dx[step]<=mazerangex)&&(y+dy[step]>0)&&(y+dy[step]<=mazerangey)&&!maze[x+dx[step]][y+dy[step]])
			{
				
				int cx=x+dx[step],cy=y+dy[step];
				maze[cx][cy]=15;
				Q.push(make_pair(cx,cy));
				system("cls"); printmaze(cx,cy); Sleep(900);
				if (cx==terminalx && cy==terminaly)
				{
					system("cls");  printmaze(cx,cy);
					paththere=1; printf("Terminal Reached!\n"); exit(0);
				}
			}
		}
	}
}

void Search_Maze_with_DFS(int x,int y)
{
	if (x==terminalx && y==terminaly)
	{
		system("cls");  printmaze(x,y);
		paththere=1;
		printf("Terminal Reached!\n");	
		exit(0);
	}

	for(int step=0; step<4; step++)
			if ((x+dx[step]>0)&&(x+dx[step]<=mazerangex)&&(y+dy[step]>0)&&(y+dy[step]<=mazerangey)&&!maze[x+dx[step]][y+dy[step]])
		{
			system("cls"); printmaze(x,y);
			x=x+dx[step]; y=y+dy[step]; maze[x][y]=15;
			Sleep(600); Search_Maze_with_DFS(x,y);
			maze[x][y]=16; x=x-dx[step]; y=y-dy[step];
			system("cls"); printmaze(x,y); Sleep(600);
		}
}
