#pragma once
#pragma once
#include <queue>
#include <list>


using namespace std;

#define path_cost_infinite 100000000
#define direction 8

//https://velog.io/@smadoros/JPS-%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98 
class JPS
{

public:

	enum class Direction
	{
		UP = 0,
		RIGHT_UP = 1,
		RIGHT = 2,
		RIGHT_DOWN = 3,
		DOWN = 4,
		LEFT_DOWN = 5,
		LEFT = 6,
		LEFT_UP = 7
	};

	struct Node
	{
		Node* prev;
		int Dir;
		int Y;
		int X;
		int Moved_distance;
		int Destination_distance;
		int sum;

		Node(Node* pr, int dir, int y, int x, int mo, int de, int su)
		{
			prev = pr;
			Dir = dir;
			Y = y;
			X = x;
			Moved_distance = mo;
			Destination_distance = de;
			sum = su;

		}

	};

	struct compare
	{
		bool operator()(const JPS::Node* A, const JPS::Node* B) const
		{

			if (A->sum != B->sum)
			{
				return A->sum > B->sum;
			}

			return A->Moved_distance > B->Moved_distance;

		}

	};


	~JPS()
	{
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

		if (map != nullptr)
		{
			delete[] map;
		}

		if (path_cost != nullptr)
		{
			delete[] path_cost;
		}



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
		for (int i = 0; i < map_height * map_width; i++)
		{
			path_cost[i] = path_cost_infinite;
		}

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


		for (int i = 0; i < 8; i++)
		{
			pq.push(new Node(nullptr, i, start.first, start.second, 0, distance_to_end(start), distance_to_end(start)));
		}

		(path_cost + start.first * map_width)[start.second] = 0;

		while (!pq.empty())
		{
			// obj compare 추가부터

			Node* tmp = pq.top();
			pq.pop();

			Close_list.push_back(tmp);

	

			if ((path_cost + tmp->Y * map_width)[tmp->X] < tmp->sum && (path_cost + tmp->Y * map_width)[tmp->X] != 0) continue;

			(path_cost + tmp->Y * map_width)[tmp->X] = tmp->sum;

			if (tmp->Y == end.first && tmp->X == end.second)
			{
				Set_Route(tmp);
			
				return;
			}

			if (tmp->Dir % 2 == 0) straight_direction(tmp, tmp->Dir);
			else diagonal_direction(tmp, tmp->Dir);
		}
	}

	list<Node*> Route()
	{
		return route;

	}

private:

	int W[8] = { 0,1,1,1,0,-1,-1,-1 };
	int H[8] = { -1,-1,0,1,1,1,0,-1 };
	int C_W1[8] = { -1,-1,1,1,1,1,-1,-1 };
	int C_W2[8] = { 1,1,1,-1,-1,-1,-1,1 };
	int C_H1[8] = { -1,-1,-1,-1,1,1,1,1 };
	int C_H2[8] = { -1,1,1,1,1,-1,-1,-1 };
	int C_WL1[8] = { -1,-1,0,0,1,1,0,0 };
	int C_WL2[8] = { 1,0,0,-1,-1,0,0,1 };
	int C_HL1[8] = { 0,0,-1,-1,0,0,1,1 };
	int C_HL2[8] = { 0,1,1,0,0,-1,-1,0 };
	int DIR1[8] = { 7,7,1,1,3,3,5,5 };
	int DIR2[8] = { 1,3,3,5,5,7,7,1 };

	priority_queue<Node*, vector<Node*>, compare> pq;
	list<Node*> route;
	list<Node*> Close_list;
	int* map; // 0과 1만 체크함
	int* path_cost;
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

	bool Valid_Node(int height, int width)
	{
		return height >= 0 && width >= 0 && height < map_height && width < map_width;
	}

	bool Destination_Check(Node* node, int _y, int _x, int _dir)
	{
		if (_y == end.first && _x == end.second)
		{
			if (_dir % 2 == 0)
			{
				int de_st = node->Moved_distance + (abs(node->X - _x) + abs(node->Y - _y)) * 10;

				Node* add = new Node(node, _dir, _y, _x, de_st, 0, de_st);

				pq.push(add);
			}
			else
			{
				int de_st = node->Moved_distance + (abs(node->X - _x)) * 14;
				Node* add = new Node(node, _dir, _y, _x, de_st, 0, de_st);
				pq.push(add);
			}
			return true;
		}

		return false;
	}


	bool NOT_Wall(int height, int width)
	{
		return (map + height * map_width)[width] != 1;
	}


	// 코너확인 및 queue에 들어온 노드가 바로 코너확인시 노드 생성(두개가 될수도 있음), 간접노드에서 코너확인시 queue에 넣고 return
	bool Corner_Procedure(Node* node, int _dir, int _y, int _x, bool parent)
	{
		int temp_x1 = _x + C_W1[_dir];
		int temp_x2 = _x + C_W2[_dir];
		int temp_y1 = _y + C_H1[_dir];
		int temp_y2 = _y + C_H2[_dir];
		int temp_xl1 = _x + C_WL1[_dir];
		int temp_xl2 = _x + C_WL2[_dir];
		int temp_yl1 = _y + C_HL1[_dir];
		int temp_yl2 = _y + C_HL2[_dir];


		if (parent)
		{
			// 체크안해도 되긴함
			if (Valid_Node(temp_y1, temp_x1) && NOT_Wall(temp_y1, temp_x1) && Valid_Node(temp_yl1, temp_xl1) && !NOT_Wall(temp_yl1, temp_xl1))
			{
				int de_ed = distance_to_end(pair<int, int>(temp_y1, temp_x1));
				int de_st = node->Moved_distance + 14;

				if (de_ed + de_st < (path_cost + temp_y1 * map_width)[temp_x1])
				{
					(path_cost + temp_y1 * map_width)[temp_x1] = de_ed + de_st;

				
					diagonal_direction(node, DIR1[_dir]);
				}
			}

			if (Valid_Node(temp_y2, temp_x2) && NOT_Wall(temp_y2, temp_x2) && Valid_Node(temp_yl2, temp_xl2) && !NOT_Wall(temp_yl2, temp_xl2))
			{
				int de_ed = distance_to_end(pair<int, int>(temp_y2, temp_x2));
				int de_st = node->Moved_distance + 14;


				if (de_ed + de_st < (path_cost + temp_y2 * map_width)[temp_x2])
				{
					(path_cost + temp_y2 * map_width)[temp_x2] = de_ed + de_st;

					diagonal_direction(node, DIR2[_dir]);
				}
			}

			return true; // parent는 안씀
		}

		else
		{
			if (
				(Valid_Node(temp_y1, temp_x1) && NOT_Wall(temp_y1, temp_x1) && Valid_Node(temp_yl1, temp_xl1) && !NOT_Wall(temp_yl1, temp_xl1)) ||
				(Valid_Node(temp_y2, temp_x2) && NOT_Wall(temp_y2, temp_x2) && Valid_Node(temp_yl2, temp_xl2) && !NOT_Wall(temp_yl2, temp_xl2))
				)
			{
				int de_ed = distance_to_end(pair<int, int>(_y, _x));
				int de_st = node->Moved_distance;

				if (_dir % 2 == 0)
				{
					de_st += (abs(node->X - _x) + abs(node->Y - _y)) * 10;
				}

				else
				{
					de_st += (abs(node->X - _x)) * 14;
				}

				Node* add = new Node(node, _dir, _y, _x, de_st, de_ed, de_ed + de_st);
				pq.push(add);
				return true;

			}

		}


		return false;

	}

	// 간접탐색 대각선에서 직선을 탐색할때 사용
	bool Sub_Search(Node* node, int _dir, int _y, int _x, bool parent)
	{

		int Dir_1 = (_dir - 1 + direction) % direction;
		int Dir_2 = (_dir + 1) % direction;

		if (parent)
		{

			Sub_straight_direction(node, Dir_1, _y, _x, true);
			Sub_straight_direction(node, Dir_2, _y, _x, true);
		}

		else
		{
			if (Sub_straight_direction(node, Dir_1, _y, _x, false) || Sub_straight_direction(node, Dir_2, _y, _x, false))
			{
				int de_ed = distance_to_end(pair<int, int>(_y, _x));
				int de_st = node->Moved_distance;

				de_st += (abs(node->X - _x)) * 14;

				Node* add = new Node(node, _dir, _y, _x, de_st, de_ed, de_ed + de_st);
				pq.push(add);

				return true;
			}

		}


		return false;
	}

	//대각선에서 나온 직선 탐색 노드
	//이해안됨
	bool Sub_straight_direction(Node* node, int _dir, int _y, int _x, bool parent)
	{
		if (parent)
		{
			while (Valid_Node(_y, _x) && NOT_Wall(_y, _x))
			{
				if (Destination_Check(node, _y, _x, _dir))
				{
					break;
				}

				if (Corner_Procedure(node, _dir, _y, _x, false))
				{
					break;
				}

				_y += H[_dir];
				_x += W[_dir];
			}

			return true;
		}

		else
		{
			while (Valid_Node(_y, _x) && NOT_Wall(_y, _x))
			{
				int temp_x1 = _x + C_W1[_dir];
				int temp_x2 = _x + C_W2[_dir];
				int temp_y1 = _y + C_H1[_dir];
				int temp_y2 = _y + C_H2[_dir];
				int temp_xl1 = _x + C_WL1[_dir];
				int temp_xl2 = _x + C_WL2[_dir];
				int temp_yl1 = _y + C_HL1[_dir];
				int temp_yl2 = _y + C_HL2[_dir];

				if (_y == end.first && _x == end.second)
				{
					return true;
				}


				if (
					(Valid_Node(temp_y1, temp_x1) && NOT_Wall(temp_y1, temp_x1) && Valid_Node(temp_yl1, temp_xl1) && !NOT_Wall(temp_yl1, temp_xl1)) ||
					(Valid_Node(temp_y2, temp_x2) && NOT_Wall(temp_y2, temp_x2) && Valid_Node(temp_yl2, temp_xl2) && !NOT_Wall(temp_yl2, temp_xl2))
					)
				{
					return true;
				}

				_y += H[_dir];
				_x += W[_dir];
			}

		}

		return false;

	}

	// 직선 탐색 노드
	void straight_direction(Node* input, int _dir)
	{

		int Dir = _dir;
		int Y = input->Y;
		int X = input->X;
		

		Corner_Procedure(input, Dir, Y, X, true);
	

		// 코너가아니므로 한칸ㅇ 
		Y += H[Dir];
		X += W[Dir];

		while (Valid_Node(Y, X) && NOT_Wall(Y, X))
		{
			if (Destination_Check(input, Y, X, Dir))
			{
				break;
			}

			if (Corner_Procedure(input, Dir, Y, X, false))
			{
				break;
			}

			Y += H[Dir];
			X += W[Dir];
		}

	}

	// 대각선 탐색 노드
	void diagonal_direction(Node* input, int _dir)
	{

		int Dir = _dir;
		int Y = input->Y;
		int X = input->X;

		Corner_Procedure(input, Dir, Y, X, true);
		Sub_Search(input, Dir, Y, X, true);



		Y += H[Dir];
		X += W[Dir];

		while (Valid_Node(Y, X) && NOT_Wall(Y, X))
		{
			if (Destination_Check(input, Y, X, Dir))
			{
				break;
			}

			if (Corner_Procedure(input, Dir, Y, X, false))
			{
				break;
			}

			if (Sub_Search(input, Dir, Y, X, false))
			{
				break;
			}

			Y += H[Dir];
			X += W[Dir];
		}

	}

};