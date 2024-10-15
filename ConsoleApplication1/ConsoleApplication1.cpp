// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "Astar.h"
#include "JumpPointSearch.h"
#include "Maze.h"
#include "Profiler.h"
#define maze_size 1000
#define space 5
#define GRID_WIDTH (maze_size+10)
#define GRID_HEIGHT (maze_size+10)
#define TestCase 1000

LARGE_INTEGER Profiler_Manager::Profile_Freq;
INT64 Profiler_Manager::Profile_total_Counter;
FILE* Profiler_Manager::Profile_fp;
DWORD Profiler_Manager::Profile_dwTlsIndex;
Profiler_Manager::st_TLS* Profiler_Manager::Profile_Tlspool[ThreadNum];
SRWLOCK Profiler_Manager::tlspool_lock;


int main()
{
    Profiler_Manager profileManager;
    profileManager.Init();


    for (int i = 0; i < TestCase; i++)
    {

        MazeGenerator Maze_Gen(maze_size, maze_size);
        Maze_Gen.generateMaze();

        int* g_Tile = new int[GRID_WIDTH * GRID_HEIGHT];
        memset(g_Tile, 0, GRID_WIDTH * GRID_HEIGHT * sizeof(int));
        int* c = Maze_Gen.GetMaze();
        for (int i = space + 1; i < maze_size + space; i++)
        {
            for (int j = space + 1; j < maze_size + space; j++)
            {
                int x = rand();
                if(x%10==0)
                g_Tile[i* maze_size +j] = (c + (i - space) * (j - space))[j - space];
            }
        }

        JPS jps;

        {
            Profiler_Manager::Profile p(L"JPS", TestCase);

      
            jps.set_start(pair<int, int>{0, 0});
            jps.set_end(pair<int, int>{space + maze_size + 1, space + maze_size + 1});
            jps.set_map(g_Tile, GRID_HEIGHT, GRID_WIDTH);
            jps.run();
            jps.Route();

        }

        delete[] g_Tile;
        

    }
    
    for (int i = 0; i < TestCase; i++)
    {

        MazeGenerator Maze_Gen(maze_size, maze_size);
        Maze_Gen.generateMaze();

        int* g_Tile = new int[GRID_WIDTH * GRID_HEIGHT];
        memset(g_Tile, 0, GRID_WIDTH * GRID_HEIGHT * sizeof(int));
        int* c = Maze_Gen.GetMaze();
        for (int i = space + 1; i < maze_size + space; i++)
        {
            for (int j = space + 1; j < maze_size + space; j++)
            {
                int x = rand();
                if (x % 10 == 0)
                g_Tile[i * maze_size + j] = (c + (i - space) * (j - space))[j - space];
            }
        }

        Astar astar;

        {
            Profiler_Manager::Profile p(L"ASTAR", TestCase);

            
            astar.set_start(pair<int, int>{0, 0});
            astar.set_end(pair<int, int>{space + maze_size + 1, space + maze_size + 1});
            astar.set_map(g_Tile, GRID_HEIGHT, GRID_WIDTH);
            astar.run();
            astar.Route();

        }

        delete[] g_Tile;


    }

    






}
