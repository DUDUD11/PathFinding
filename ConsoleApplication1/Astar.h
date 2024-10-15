#pragma once
#include <queue>
#include <list>


using namespace std;

#define path_cost_infinite 1000000

class Astar
{

public:
	int* path_cost;
	int* path_start_cost;

	struct Node
	{
		Node* prev;
		int Y;
		int X;
		int Moved_distance;
		int Destination_distance;
		int sum;

		Node(Node* pr, int y, int x, int mo, int de, int su)
		{
			prev = pr;
			Y = y;
			X = x;
			Moved_distance = mo;
			Destination_distance = de;
			sum = su;

		}

	};

	struct compare
	{
		bool operator()(const Astar::Node* A, const Astar::Node* B) const
		{

			if (A->sum != B->sum)
			{
				return A->sum > B->sum;
			}

			return A->Moved_distance > B->Moved_distance;

		}

	};


	~Astar()
	{
		if (path_cost != nullptr)
		{
			delete[] path_cost;
		}

		if (path_start_cost != nullptr)
		{
			delete[] path_start_cost;
		}

		if (map != nullptr)
		{
			delete[] map;
		}

		while (!pq.empty())
		{
			Node* tmp = pq.top();
			pq.pop();
			delete(tmp);
		}

		for (list<Node*>::iterator iter = Close_list.begin(); iter != Close_list.end();)
		{
			Node* tmp = *iter;

			iter = Close_list.erase(iter);
			delete(tmp);
		}
	}



	int* get_checker()
	{
		return path_cost;
	}

	void set_start(pair<int, int> st)
	{
		start = st;
	}

	void set_end(pair<int, int> ed)
	{
		end = ed;
	}


	void set_map(int* mp, int ha, int wd)
	{
	


		map_height = ha;
		map_width = wd;
		map = new int[ha * wd];

		path_cost = new int[map_height * map_width];
		path_start_cost = new int[map_height * map_width];

		for (int i = 0; i < ha; i++)
		{
			for (int j = 0; j < wd; j++)
			{
				(map + wd * i)[j] = (mp + wd * i)[j];
			}
		}
	}

	void run()
	{

		for (int i = 0; i < map_height; i++)
		{
			for (int j = 0; j < map_width; j++)
			{
				(path_cost + i * map_width)[j] = path_cost_infinite;
				(path_start_cost + i * map_width)[j] = path_cost_infinite;
			}
		}

		Node* _start = new Node(nullptr, start.first, start.second, 0, distance_to_end(start), distance_to_end(start));
		pq.push(_start);

		(path_cost + start.first)[start.second] = 0;
		(path_start_cost + start.first)[start.second] = 0;

		int Y_i[] = { -1,1,0,0,-1,-1,1,1 };
		int X_i[] = { 0,0,-1,1,-1,1,-1,1 };

		int recur = 0;

		while (!pq.empty())
		{
			// obj compare 추가부터


			Node* tmp = pq.top();
			pq.pop();

			Close_list.push_back(tmp);



			if (tmp->Y == end.first && tmp->X == end.second)
			{
				Set_Route(tmp);
			
				return;

			}

			for (int i = 0; i < 8; i++)
			{
				int Y = tmp->Y + Y_i[i];
				int X = tmp->X + X_i[i];


				// y가 더클때 목적지보다 더클때테스트

				if (Y < 0 || Y >= map_height || X < 0 || X >= map_width)
				{
					continue;
				}

				if ((map + Y * map_width)[X] == 1)
				{
					continue;
				}


				int de_st = tmp->Moved_distance;
				int de_ed = distance_to_end(pair<int, int>(Y, X));



				if (Y_i[i] * X_i[i] != 0)
				{
					de_st += 14;
				}

				else
				{
					de_st += 10;
				}



				if (de_ed + de_st < (path_cost + Y * map_width)[X])
				{
					(path_cost + Y * map_width)[X] = de_ed + de_st;
					(path_start_cost + Y * map_width)[X] = de_st;

					Node* t = new Node(tmp, Y, X, de_st, de_ed, de_st + de_ed);

					pq.push(t);



				}

				else if (de_ed + de_st == (path_cost + Y * map_width)[X])
				{
					if (de_st < (path_start_cost + Y * map_width)[X])
					{
						(path_start_cost + Y * map_width)[X] = de_st;

						Node* t = new Node(tmp, Y, X, de_st, de_ed, de_st + de_ed);
						pq.push(t);
					}
				}

			}


		}


	}

	list<Node*> Route()
	{
		return route;
	}

private:

	priority_queue<Node*, vector<Node*>, compare> pq;
	list<Node*> Close_list;
	list<Node*> route;

	int* map; // 0과 1만 체크함
	int map_height;
	int map_width;
	pair<int, int> start; // y가 앞부터
	pair<int, int> end; // y가 앞부터

	void Set_Route(Node* val)
	{


		while (val != nullptr)
		{
			route.push_front(val);
			val = val->prev;
		}


	}


	int distance_to_end(pair<int, int> val)
	{
		int d = (int)sqrt((val.first - end.first) * (val.first - end.first) + (val.second - end.second) * (val.second - end.second)) * 10;
		return d;
	}





};