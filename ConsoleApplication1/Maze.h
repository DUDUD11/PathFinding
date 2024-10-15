#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>


class MazeGenerator
{
    struct Node
    {
        int x;
        int y;

        Node(int _y, int _x)
        {
            y = _y;
            x = _x;

        }


    };




private:
    std::stack<Node> _stack;
    std::vector<Node> _list;
    int _rand;
    int* _maze;
    int _width;
    int _height;

    bool validNextNode(Node node)
    {
        int numNeighboringOnes = 0;

        for (int y = node.y - 1; y < node.y + 2; y++)
        {
            for (int x = node.x - 1; x < node.x + 2; x++)
            {


                if (pointOnGrid(x, y) && pointNotNode(node, x, y) && (_maze + y * x)[x] == 1)
                {
                    numNeighboringOnes++;
                }
            }
        }

        return (numNeighboringOnes < 3) && (_maze + node.y * node.x)[node.x] != 1;

    };

    bool pointOnGrid(int x, int y)
    {
        return x >= 0 && y >= 0 && x < _width&& y < _height;
    };

    bool pointNotCorner(Node node, int x, int y)
    {
        return (x == node.x || y == node.y);
    };

    bool pointNotNode(Node node, int x, int y)
    {
        return !(x == node.x && y == node.y);
    };

public:

    ~MazeGenerator()
    {
        delete[] _maze;

    }

    int* GetMaze()
    {
        return _maze;
    }


    MazeGenerator(int height, int width)
    {
        _width = width;
        _height = height;

        _maze = new int[width * height];
        memset(_maze, 0, width * height * sizeof(int));
        srand((unsigned int)time(NULL));
    }

    void generateMaze()
    {
        _stack.push(Node(0, 0));
        _list.clear();

        while (!_stack.empty())
        {
            Node next = _stack.top();
            _stack.pop();

            if (validNextNode(next))
            {
                (_maze + next.y * next.x)[next.x] = 1;
                findNeighbors(next, _list);
                randomlyAddNodesToStack(_list);


            }


        }

    }

    void findNeighbors(Node node, std::vector<Node>& neighbors) {

        for (int y = node.y - 1; y < node.y + 2; y++) {
            for (int x = node.x - 1; x < node.x + 2; x++) {
                if (pointOnGrid(x, y) && pointNotCorner(node, x, y)
                    && pointNotNode(node, x, y)) {
                    neighbors.push_back(Node(y, x));
                }
            }
        }
    }

    void randomlyAddNodesToStack(std::vector<Node>& nodes) {
        int targetIndex;
        while (!nodes.empty()) {

            targetIndex = rand() % nodes.size();

            _stack.push(nodes[targetIndex]);
            nodes.erase(nodes.begin() + targetIndex);
        }
    }

};


/*
import java.util.ArrayList;
import java.util.Stack;
import java.util.Random;
import java.util.Arrays;

class MazeGenerator {

    private Stack<Node> stack = new Stack<>();
    private Random rand = new Random();
    private int[][] maze;
    private int dimension;

    MazeGenerator(int dim) {
        maze = new int[dim][dim];
        dimension = dim;
    }

    public void generateMaze() {
        stack.push(new Node(0,0));
        while (!stack.empty()) {
            Node next = stack.pop();
            if (validNextNode(next)) {
                maze[next.y][next.x] = 1;
                ArrayList<Node> neighbors = findNeighbors(next);
                randomlyAddNodesToStack(neighbors);
            }
        }
    }

    public String getRawMaze() {
        StringBuilder sb = new StringBuilder();
        for (int[] row : maze) {
            sb.append(Arrays.toString(row) + "\n");
        }
        return sb.toString();
    }

    public String getSymbolicMaze() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                sb.append(maze[i][j] == 1 ? "*" : " ");
                sb.append("  ");
            }
            sb.append("\n");
        }
        return sb.toString();
    }

    private boolean validNextNode(Node node) {
        int numNeighboringOnes = 0;
        for (int y = node.y-1; y < node.y+2; y++) {
            for (int x = node.x-1; x < node.x+2; x++) {
                if (pointOnGrid(x, y) && pointNotNode(node, x, y) && maze[y][x] == 1) {
                    numNeighboringOnes++;
                }
            }
        }
        return (numNeighboringOnes < 3) && maze[node.y][node.x] != 1;
    }

    private void randomlyAddNodesToStack(ArrayList<Node> nodes) {
        int targetIndex;
        while (!nodes.isEmpty()) {
            targetIndex = rand.nextInt(nodes.size());
            stack.push(nodes.remove(targetIndex));
        }
    }

    private ArrayList<Node> findNeighbors(Node node) {
        ArrayList<Node> neighbors = new ArrayList<>();
        for (int y = node.y-1; y < node.y+2; y++) {
            for (int x = node.x-1; x < node.x+2; x++) {
                if (pointOnGrid(x, y) && pointNotCorner(node, x, y)
                    && pointNotNode(node, x, y)) {
                    neighbors.add(new Node(x, y));
                }
            }
        }
        return neighbors;
    }

    private Boolean pointOnGrid(int x, int y) {
        return x >= 0 && y >= 0 && x < dimension && y < dimension;
    }

    private Boolean pointNotCorner(Node node, int x, int y) {
        return (x == node.x || y == node.y);
    }

    private Boolean pointNotNode(Node node, int x, int y) {
        return !(x == node.x && y == node.y);
    }
}
*/
