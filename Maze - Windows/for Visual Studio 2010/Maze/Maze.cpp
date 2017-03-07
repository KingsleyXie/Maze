/**************************************************************************************
* Note: This program was written as my homework project due 20170305
* Program Usage: Generate a maze randomly and find the path automatically
* Author: Kingsley ( Xie Yimin )
* Last modified date: 5th March, 2017 && Code Length: 423 Lines in Windows 10
* You can download the code through [https://github.com/KingsleyXie/Maze]
* Copyright © Kingsley( Xie Yimin )    All rights reserved    HeiHeiHei
**************************************************************************************
*/

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <queue>

using namespace std;

#define MAX_RANGE_OF_MAZE	73
#define MIN_RANGE_OF_MAZE	3
//The maximum and minimum number of Maze Range

queue <pair<int, int> > Q; //The queue for BFS Algorithm

int	Qpre[MAX_RANGE_OF_MAZE][MAX_RANGE_OF_MAZE][2] = { 0 },
QnextNum[MAX_RANGE_OF_MAZE][MAX_RANGE_OF_MAZE] = { 0 },
Qupdated;
/*
Qpre[] stores the coordinates before the present position
QnextNum[] stores the number of child nodes of the present position
Qupdated is a signal to set incorrect paths with 'xx' in BFS algorithm
*/

int generatex = 12, generatey = 15, outsetx = 2, outsety = 1,
terminalx = 2 * generatex, terminaly = 2 * generatey + 1,
mazerangex = 2 * generatex + 1, mazerangey = 2 * generatey + 1;
//Ranges & Outset Point & Terminal Point of Maze, these can be modified through input

int Print_Mode = 0, Show_Generate_Process = 0, Show_Search_Process = 1,
AutoPause = 1, Algorithm_Mode = 1, Sleep_Time = 600;
//Default setting values, which are changable through input as well

int maze[MAX_RANGE_OF_MAZE + 6][MAX_RANGE_OF_MAZE + 6]; //Prepare the map of Maze

int dx[4] = { 0, 1, 0, -1 }, dy[4] = { 1, 0, -1, 0 }, PathExist = 0;
/*
dx[] and dy[] are offsets when take a move in a search algorithm
PathExist is here to check whether the program exit after finding a way or not
*/

void UserInput();
/*
The function of this Function: Customize the changable variables
*/

void printmaze(int presentx, int presenty);
/*
The function of this Function: show real-time status of Maze

"presentx" and "presenty" are cooridinates of the current move
'\u2588' & '\u00B7' represent for [Full Block] & [Middle Dot] in Unicode. There are alternative ASCII characters to avoid garbled problems especially in Windows
In Visual Studio, the Unicode characters "\u2588\u2588" and "\u00B7\u00B7" should be "\u2588" and "\u00B7"
For more information about special characters, see [https://unicode-table.com]
Meaning of Characters:
"00":		present postion( maze[presentx][presenty] )
"  ":		Store as integer 0		accessable roads which are not tried
DOTS:		Store as integer 15		path of the maze, characters include "\u00B7\u00B7" & ".."
"xx":		Store as integer 16		unaccessable roads
"<>":		Store as integer 17		the trying path while in Breadth_First_Search
WALLS:		Store as integer 1-14	the initial walls of the maze, characters include "KK" & "\u2588\u2588" & a Poem =￣ω￣=
*/

void GenerateMaze(int generatex, int generatey);
/*
The function of this Function: generate Maze

Inspiration from [http://www.wutianqi.com/?p=1490]
*/

void InitialGenerate();
/*
The function of this Function: initial some data before the generation of Maze
*/

void Search_Maze_with_BFS(int x, int y);
/*
The function of this Function: do the Breadth_First_Search Algorithm

I used Queue to simplify some processes
*/

void Search_Maze_with_DFS(int x, int y);
/*
The function of this Function: do the Depth_First_Search Algorithm
*/



int main()
{
	printf("您希望以哪种算法搜索迷宫寻找出路？\t【深度优先搜索 - 0   广度优先搜索 - 1】\n\t");
	scanf("%d", &Algorithm_Mode);

	printf("是否需要显示搜索迷宫路径的过程？\t【否 - 0   是 - 1】\n\t");
	scanf("%d", &Show_Search_Process);

	printf("是否直接使用默认设置并运行？\t【否 - 0   是 - 1】\n\t");
	int defaultset; scanf("%d", &defaultset);
	if (!defaultset)
		UserInput();

	InitialGenerate();	srand((unsigned)time(NULL));
	GenerateMaze(rand() % generatex + 1, rand() % generatey + 1);

	if (Algorithm_Mode == 0)
		Search_Maze_with_DFS(outsetx, outsety);
	else Search_Maze_with_BFS(outsetx, outsety);

	if (!PathExist)
	{
		printf("你已经被困在迷宫里了\n嘿 嘿 嘿\n");
		printmaze(0, 0);
		printf("Press Enter To Continue."); getchar(); getchar();
	}

	return(0);
}



void UserInput()
{
	int Customize_points, inputx, inputy;

	printf("您希望以哪种格式显示迷宫？\t【默认模式 - 0   ASCII模式 - 1   念两句诗 - 2】\n\t");
	scanf("%d", &Print_Mode);

	printf("请输入迷宫的长和宽(默认值：31 × 25)\t【长宽值范围: 3~73   中间请使用空格隔开】\n\t");
	scanf("%d %d", &inputy, &inputx);

	while (inputy<MIN_RANGE_OF_MAZE || inputy>MAX_RANGE_OF_MAZE ||
		inputx<MIN_RANGE_OF_MAZE || inputx>MAX_RANGE_OF_MAZE)
	{
		printf("Naive! 你这样子输入是不行的，I'm Angry!\n");
		printf("请重新输入迷宫的长和宽(默认值：31 × 25)\t【长宽值范围: 3~73   中间请使用空格隔开】\n\t");
		scanf("%d %d", &inputy, &inputx);
	}

	generatex = inputx / 2; generatey = inputy / 2;
	mazerangex = 2 * generatex + 1; mazerangey = 2 * generatey + 1;
	terminalx = 2 * generatex; terminaly = 2 * generatey + 1;

	printf("是否要自定义起点和终点？\t【否 - 0   是 - 1】\n\t");
	scanf("%d", &Customize_points);
	if (Customize_points)
	{
		printf("请输入起点坐标，注意不要超出迷宫的范围\n\t");
		scanf("%d %d", &inputx, &inputy);
		while (inputx<1 || inputx>mazerangex || inputy<1 || inputy>mazerangey)
		{
			printf("Naive! 你这样子输入是不行的，I'm Angry!\n");
			printf("请重新输入起点坐标\n\t");
			scanf("%d %d", &inputx, &inputy);
		}
		outsetx = inputx; outsety = inputy;

		printf("请输入终点坐标，注意不要超出迷宫的范围\n\t");
		scanf("%d %d", &inputx, &inputy);
		while (inputx<1 || inputx>mazerangex || inputy<1 || inputy>mazerangey)
		{
			printf("Naive! 你这样子输入是不行的，I'm Angry!\n");
			printf("请重新输入终点坐标\n\t");
			scanf("%d %d", &inputx, &inputy);
		}
		terminalx = inputx; terminaly = inputy;
	}

	printf("是否需要显示迷宫的形成过程？\t【否 - 0   是 - 1】\n\t");
	scanf("%d", &Show_Generate_Process);

	if (Show_Search_Process)
	{
		printf("您希望怎样显示迷宫搜索时每一步的过程？\t【手动 - 0   自动 - 1】\n\t");
		scanf("%d", &AutoPause);

		if (AutoPause)
		{
			printf("请输入搜索过程中输出每一步的间隔时间，以毫秒为单位(默认值：600ms)\n\t");
			scanf("%d", &Sleep_Time);
		}
	}
}


void printmaze(int presentx, int presenty)
{
	for (int i = 1; i <= mazerangex; i++)
		for (int j = 1; j <= mazerangey; j++)
		{
			if (Print_Mode == 2)
			{
				if (i == presentx && j == presenty)
					printf("00 ");
				else
					switch (maze[i][j])
					{
					case 0: printf("   "); break;
					case 1: printf("苟 "); break;
					case 2: printf("利 "); break;
					case 3: printf("国 "); break;
					case 4: printf("家 "); break;
					case 5: printf("生 "); break;
					case 6: printf("死 "); break;
					case 7: printf("以 "); break;
					case 8: printf("岂 "); break;
					case 9: printf("因 "); break;
					case 10: printf("祸 "); break;
					case 11: printf("福 "); break;
					case 12: printf("避 "); break;
					case 13: printf("趋 "); break;
					case 14: printf("之 "); break;
					case 15: printf("\u00B7 "); break;
					case 16: printf("xx "); break;
					case 17: printf("<> "); break;
					}

				if (j == mazerangey)
					printf("\n");
			}

			if (Print_Mode != 2)
			{
				if (i == presentx && j == presenty)
					printf("00");
				else
					switch (maze[i][j])
					{
					case 0: printf("  "); break;
					case 15:
						if (Print_Mode == 0)
							printf("\u00B7");
						else printf(".."); break;
					case 16: printf("xx"); break;
					case 17: printf("<>"); break;
					default:
						if (Print_Mode == 0)
							printf("\u2588");
						else printf("KK");
						break;
					}

				if (j == mazerangey)
					printf("\n");
			}
		}
}


void GenerateMaze(int generatex, int generatey)
{
	int	doublex = generatex * 2, doubley = generatey * 2;
	int	phase = rand() % 2 ? 1 : 3;

	maze[doublex][doubley] = 0;

	for (int i = 0, step = rand() % 4; i < 4; i++, step = (step + phase) % 4)
		if (doubley + dy[step] - 1 != mazerangey &&
			maze[doublex + 2 * dx[step]][doubley + 2 * dy[step]])
		{
			maze[doublex + dx[step]][doubley + dy[step]] = 0;
			if (Show_Generate_Process)
			{
				system("cls"); printmaze(doublex + dx[step], doubley + dy[step]);
				if (!AutoPause)
				{
					printf("Press Enter To Continue."); getchar();
				}
				else Sleep(Sleep_Time);
			}
			GenerateMaze(generatex + dx[step], generatey + dy[step]);
		}
}


void InitialGenerate()
{
	for (int i = 0; i <= generatex * 2 + 2; i++)
		for (int j = 0; j <= generatey * 2 + 2; ++j)
			maze[i][j] = (i + j - 2) % 14 + 1;

	for (int i = 0, j = 2 * generatex + 2; i <= 2 * generatey + 2; i++)
	{
		maze[0][i] = 0; maze[j][i] = 0;
	}
	for (int i = 0, j = 2 * generatey + 2; i <= 2 * generatex + 2; i++)
	{
		maze[i][0] = 0; maze[i][j] = 0;
	}

	maze[outsetx][outsety] = 15; maze[terminalx][terminaly] = 0;
}


void  Search_Maze_with_BFS(int x, int y)
{
	Q.push(make_pair(x, y));
	while (!Q.empty())
	{
		int xstep, ystep;
		x = Q.front().first; y = Q.front().second; Qupdated = 0;
		for (int step = 0; step < 4; step++)
		{
			if ((x + dx[step] > 0) && (x + dx[step] <= mazerangex) &&
				(y + dy[step] > 0) && (y + dy[step] <= mazerangey) &&
				!maze[x + dx[step]][y + dy[step]])
			{
				xstep = x + dx[step], ystep = y + dy[step];
				maze[xstep][ystep] = 17;
				Q.push(make_pair(xstep, ystep)); Qupdated = 1;

				Qpre[xstep][ystep][0] = x; Qpre[xstep][ystep][1] = y; QnextNum[x][y]++;

				if (Show_Search_Process)
				{
					system("cls"); printmaze(xstep, ystep);
					if (!AutoPause)
					{
						printf("Press Enter To Continue."); getchar();
					}
					else Sleep(Sleep_Time);
				}

				if (xstep == terminalx && ystep == terminaly)
				{
					do
						//Set the path with accessable character instead of trying charcater
					{
						maze[xstep][ystep] = 15;
						int xtemp = Qpre[xstep][ystep][0];
						ystep = Qpre[xstep][ystep][1]; xstep = xtemp;
					} while (xstep && ystep);

					system("cls");  printmaze(terminalx, terminaly);
					PathExist = 1; printf("已到达迷宫的出口位置\n");
					printf("Press Enter To Continue."); getchar(); getchar();
					exit(0);
				}
			}
		}

		if (!Qupdated)
		{
			int deletex = x, deletey = y;
			do
			{
				maze[deletex][deletey] = 16;

				int xtemp = Qpre[deletex][deletey][0];
				deletey = Qpre[deletex][deletey][1]; deletex = xtemp;

				QnextNum[deletex][deletey]--;

				if (Show_Search_Process)
				{
					system("cls"); printmaze(xstep, ystep);
					if (!AutoPause)
					{
						printf("Press Enter To Continue."); getchar();
					}
					else Sleep(Sleep_Time);
				}
			} while (deletex && deletey && !QnextNum[deletex][deletey]);
		}
		Q.pop();
	}
}


void Search_Maze_with_DFS(int x, int y)
{
	if (x == terminalx && y == terminaly)
	{
		system("cls");  printmaze(x, y);
		PathExist = 1;
		printf("已到达迷宫的出口位置\n");
		printf("Press Enter To Continue."); getchar(); getchar();
		exit(0);
	}

	for (int step = 0; step < 4; step++)
		if ((x + dx[step] > 0) && (x + dx[step] <= mazerangex) &&
			(y + dy[step] > 0) && (y + dy[step] <= mazerangey) &&
			!maze[x + dx[step]][y + dy[step]])
		{
			if (Show_Search_Process)
			{
				system("cls"); printmaze(x, y);
				if (!AutoPause)
				{
					printf("Press Enter To Continue."); getchar();
				}
				else Sleep(Sleep_Time);
			}

			x = x + dx[step]; y = y + dy[step]; maze[x][y] = 15;

			Search_Maze_with_DFS(x, y);

			if (Show_Search_Process)
			{
				system("cls"); printmaze(x, y);
				if (!AutoPause)
				{
					printf("Press Enter To Continue."); getchar();
				}
				else Sleep(Sleep_Time);
			}

			maze[x][y] = 16; x = x - dx[step]; y = y - dy[step];
		}
}
