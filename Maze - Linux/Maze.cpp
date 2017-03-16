/*********************************************************************************************
 * Note: This program was written as my homework project due 20170305
 * Program Usage: Generate a maze randomly and find the path automatically
 * Author: Kingsley ( Xie Yimin )
 * Last modified date: 16th March, 2017 && Code Length: 425 Lines in elementary OS 0.4 Loki
 * You can download the code through [https://github.com/KingsleyXie/Maze]
 * Copyright © Kingsley( Xie Yimin )    All rights reserved    HeiHeiHei
 *********************************************************************************************
 */

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
char Instruction[3]; //Convert "Sleep( Sleep_Time )" in <windows.h> to Linux instruction

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

void printmaze( int presentx, int presenty );
/*
	The function of this Function: show real-time status of Maze

	"presentx" and "presenty" are coordinates of the current move
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

void GenerateMaze( int generatex, int generatey );
/*
	The function of this Function: generate Maze

	Inspiration from [http://www.wutianqi.com/?p=1490]
*/

void InitialGenerate();
/*
	The function of this Function: initial some data before the generation of Maze
*/

void Search_Maze_with_BFS( int x, int y );
/*
	The function of this Function: do the Breadth_First_Search Algorithm

	I used Queue to simplify some processes
*/

void Search_Maze_with_DFS( int x, int y );
/*
	The function of this Function: do the Depth_First_Search Algorithm
*/



int main()
{
	printf( "Which algorithm would you like to use to search the path of maze?\t(Depth-First-Search - 0   Breadth-First-Search - 1)\n\t" );
	scanf( "%d", &Algorithm_Mode );

	printf( "Would you like to see the process of searching path?\t(No - 0   Yes - 1)\n\t" );
	scanf( "%d", &Show_Search_Process );

	printf( "Would you like to directly run with default setting?\t(No - 0   Yes - 1)\n\t" );
	int defaultset; scanf( "%d", &defaultset );
	if ( !defaultset )
		UserInput();

	sprintf(Instruction, "sleep %.2f", Sleep_Time/1000.0);

	InitialGenerate();	srand( (unsigned) time( NULL ) );
	GenerateMaze( rand() % generatex + 1, rand() % generatey + 1 );

	if ( Algorithm_Mode == 0 )
		Search_Maze_with_DFS( outsetx, outsety );
	else Search_Maze_with_BFS( outsetx, outsety );

	if ( !PathExist )
	{
		printf( "You are trapped in the maze\nHei Hei Hei\n" );
		printmaze( 0, 0 );
		printf("Press Enter To Continue."); getchar(); getchar();
	}

	return(0);
}



void UserInput()
{
	int Customize_points, inputx, inputy;

	printf( "Which mode would you like to show the maze\t(Default - 0   ASCII - 1   HappyFrog - 2)\n\t" );
	scanf( "%d", &Print_Mode );

	printf( "Please input the length and width of maze(Default Value: 31 × 25)\t(Use space to seperate them and make sure they are in range of 3~73)\n\t" );
	scanf( "%d %d", &inputy, &inputx );

	while ( inputy<MIN_RANGE_OF_MAZE || inputy>MAX_RANGE_OF_MAZE ||
		inputx<MIN_RANGE_OF_MAZE || inputx>MAX_RANGE_OF_MAZE )
	{
		printf( "Naive! Your input are contrary to the Basic Law, I'm Angry!\n" );
		printf( "Please re-input the length and width of maze(Default Value: 31 × 25)\t(Use space to seperate them and make sure they are in range of 3~73)\n\t" );
		scanf( "%d %d", &inputy, &inputx );
	}

	generatex = inputx / 2; generatey = inputy / 2;
	mazerangex = 2 * generatex + 1; mazerangey = 2 * generatey + 1;
	terminalx = 2 * generatex; terminaly = 2 * generatey + 1;

	printf( "Would you like to customize the outset and terminal points?\t(No - 0   Yes - 1)\n\t" );
	scanf( "%d", &Customize_points );
	if ( Customize_points )
	{
		printf( "Please input the coordinates of outset point and make sure it is inside the map of maze\n\t" );
		scanf( "%d %d", &inputx, &inputy );
		while ( inputx<1 || inputx>mazerangex || inputy<1 || inputy>mazerangey )
		{
			printf( "Naive! Your input are contrary to the Basic Law, I'm Angry!\n" );
			printf( "Please re-input the coordinates of outset point\n\t" );
			scanf( "%d %d", &inputx, &inputy );
		}
		outsetx = inputx; outsety = inputy;

		printf( "Please input the coordinates of terminal point and make sure it is inside the map of maze\n\t" );
		scanf( "%d %d", &inputx, &inputy );
		while ( inputx<1 || inputx>mazerangex || inputy<1 || inputy>mazerangey )
		{
			printf( "Naive! Your input are contrary to the Basic Law, I'm Angry!\\n" );
			printf( "Please re-input the coordinates of terminal point\n\t" );
			scanf( "%d %d", &inputx, &inputy );
		}
		terminalx = inputx; terminaly = inputy;
	}

	printf( "Would you like to see the generate process of maze?\t(No - 0   Yes - 1)\n\t" );
	scanf( "%d", &Show_Generate_Process );

	if ( Show_Search_Process )
	{
		printf( "How would you like to show every step while printing maze?\t(ManualControl - 0   AutomaticControl - 1)\n\t" );
		scanf( "%d", &AutoPause );

		if ( AutoPause )
		{
			printf( "Please input the pause time between steps in millisecond(Default Value: 600ms)\n\t" );
			scanf( "%d", &Sleep_Time );
		}
	}
}


void printmaze( int presentx, int presenty )
{
	for ( int i = 1; i <= mazerangex; i++ )
		for ( int j = 1; j <= mazerangey; j++ )
		{
			if ( Print_Mode == 2 )
			{
				if ( i == presentx && j == presenty )
					printf( "00 " );
				else
					switch ( maze[i][j] )
					{
					case 0: printf( "   " ); break;
					case 1: printf( "苟 " ); break;
					case 2: printf( "利 " ); break;
					case 3: printf( "国 " ); break;
					case 4: printf( "家 " ); break;
					case 5: printf( "生 " ); break;
					case 6: printf( "死 " ); break;
					case 7: printf( "以 " ); break;
					case 8: printf( "岂 " ); break;
					case 9: printf( "因 " ); break;
					case 10: printf( "祸 " ); break;
					case 11: printf( "福 " ); break;
					case 12: printf( "避 " ); break;
					case 13: printf( "趋 " ); break;
					case 14: printf( "之 " ); break;
					case 15: printf( "\u00B7\u00B7 " ); break;
					case 16: printf( "xx " ); break;
					case 17: printf( "<> " ); break;
					}

				if ( j == mazerangey )
					printf( "\n" );
			}

			if ( Print_Mode != 2 )
			{
				if ( i == presentx && j == presenty )
					printf( "00" );
				else
					switch ( maze[i][j] )
					{
					case 0: printf( "  " ); break;
					case 15:
						if ( Print_Mode == 0 )
							printf( "\u00B7\u00B7" );
						else printf( ".." ); break;
					case 16: printf( "xx" ); break;
					case 17: printf( "<>" ); break;
					default:
						if ( Print_Mode == 0 )
							printf( "\u2588\u2588" );
						else printf( "KK" );
						break;
					}

				if ( j == mazerangey )
					printf( "\n" );
			}
		}
}


void GenerateMaze( int generatex, int generatey )
{
	int	doublex = generatex * 2, doubley = generatey * 2;
	int	phase = rand() % 2 ? 1 : 3;

	maze[doublex][doubley] = 0;

	for ( int i = 0, step = rand() % 4; i < 4; i++, step = (step + phase) % 4 )
		if ( doubley + dy[step] - 1 != mazerangey &&
			maze[doublex + 2 * dx[step]][doubley + 2 * dy[step]] )
		{
			maze[doublex + dx[step]][doubley + dy[step]] = 0;
			if ( Show_Generate_Process )
			{
				system( "clear" ); printmaze( doublex + dx[step], doubley + dy[step] );
				if ( !AutoPause )
				{
					printf("Press Enter To Continue."); getchar();
				}
				else system(Instruction);
			}
			GenerateMaze( generatex + dx[step], generatey + dy[step] );
		}
}


void InitialGenerate()
{
	for ( int i = 0; i <= generatex * 2 + 2; i++ )
		for ( int j = 0; j <= generatey * 2 + 2; ++j )
			maze[i][j] = (i + j - 2) % 14 + 1;

	for ( int i = 0, j = 2 * generatex + 2; i <= 2 * generatey + 2; i++ )
	{
		maze[0][i] = 0; maze[j][i] = 0;
	}
	for ( int i = 0, j = 2 * generatey + 2; i <= 2 * generatex + 2; i++ )
	{
		maze[i][0] = 0; maze[i][j] = 0;
	}

	maze[outsetx][outsety] = 15; maze[terminalx][terminaly] = 0;
}


void  Search_Maze_with_BFS( int x, int y )
{
	Q.push( make_pair( x, y ) );
	while ( !Q.empty() )
	{
		int xstep, ystep;
		x = Q.front().first; y = Q.front().second; Qupdated = 0;
		for ( int step = 0; step < 4; step++ )
		{
			if ( (x + dx[step] > 0) && (x + dx[step] <= mazerangex) &&
				(y + dy[step] > 0) && (y + dy[step] <= mazerangey) &&
				!maze[x + dx[step]][y + dy[step]] )
			{
				xstep = x + dx[step], ystep = y + dy[step];
				maze[xstep][ystep] = 17;
				Q.push( make_pair( xstep, ystep ) ); Qupdated = 1;

				Qpre[xstep][ystep][0] = x; Qpre[xstep][ystep][1] = y; QnextNum[x][y]++;

				if ( Show_Search_Process )
				{
					system( "clear" ); printmaze( xstep, ystep );
					if ( !AutoPause )
					{
						printf("Press Enter To Continue."); getchar();
					}
					else system(Instruction);
				}

				if ( xstep == terminalx && ystep == terminaly )
				{
					do
					//Set the path with accessable character instead of trying charcater
					{
						maze[xstep][ystep] = 15;
						int xtemp = Qpre[xstep][ystep][0];
						ystep = Qpre[xstep][ystep][1]; xstep = xtemp;
					} while ( xstep && ystep );

					system( "clear" );  printmaze( terminalx, terminaly );
					PathExist = 1; printf( "We have reached the terminal position\n" );
					printf("Press Enter To Continue."); getchar(); getchar();
					exit( 0 );
				}
			}
		}

		if ( !Qupdated )
		{
			int deletex = x, deletey = y;
			do
			{
				maze[deletex][deletey] = 16;

				int xtemp = Qpre[deletex][deletey][0];
				deletey = Qpre[deletex][deletey][1]; deletex = xtemp;

				QnextNum[deletex][deletey]--;

				if ( Show_Search_Process )
				{
					system( "clear" ); printmaze( xstep, ystep );
					if ( !AutoPause )
					{
						printf("Press Enter To Continue."); getchar();
					}
					else system(Instruction);
				}
			} while ( deletex && deletey && !QnextNum[deletex][deletey] );
		}
		Q.pop();
	}
}


void Search_Maze_with_DFS( int x, int y )
{
	if ( x == terminalx && y == terminaly )
	{
		system( "clear" );  printmaze( x, y );
		PathExist = 1;
		printf( "We have reached the terminal position\n" );
		printf("Press Enter To Continue."); getchar(); getchar();
		exit( 0 );
	}

	for ( int step = 0; step < 4; step++ )
		if ( (x + dx[step] > 0) && (x + dx[step] <= mazerangex) &&
			(y + dy[step] > 0) && (y + dy[step] <= mazerangey) &&
			!maze[x + dx[step]][y + dy[step]] )
		{
			if ( Show_Search_Process )
			{
				system( "clear" ); printmaze( x, y );
				if ( !AutoPause )
				{
					printf("Press Enter To Continue."); getchar();
				}
				else system(Instruction);
			}

			x = x + dx[step]; y = y + dy[step]; maze[x][y] = 15;

			Search_Maze_with_DFS( x, y );

			if ( Show_Search_Process )
			{
				system( "clear" ); printmaze( x, y );
				if ( !AutoPause )
				{
					printf("Press Enter To Continue."); getchar();
				}
				else system(Instruction);
			}

			maze[x][y] = 16; x = x - dx[step]; y = y - dy[step];
		}
}

