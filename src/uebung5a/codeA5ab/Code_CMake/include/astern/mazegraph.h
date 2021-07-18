#include <iostream>
#include <fstream>
#include <math.h>

#include "unit.h"

using namespace std;
using LocalEdgeT = std::pair<VertexT, CostT>;

class MazeGraph : public DistanceGraph {
public:
    size_t height;
    size_t width;
    vector<CellType> nodes;

protected:
    /*
    size_t height;
    size_t width;
    vector<CellType> nodes;
    */
    vector<NeighborT> adjacencyList;

public:
    MazeGraph(int num_verts = 0)
            : DistanceGraph(num_verts), nodes(num_verts), adjacencyList(num_verts) {}

    MazeGraph(vector<CellType> maze, int h, int w) {
        width = w;
        height = h;
        vertexCount = width * height;
        resisze(numVertices());

        for (size_t i = 0; i < numVertices(); i++) {
            if (maze[i] == CellType::Start || maze[i] == CellType::Destination) {
                maze[i] = CellType::Ground;
            }
        }

        nodes = maze;

        calcAdjacencyList();
    }

    ~MazeGraph() {
        for (size_t i = 0; i < adjacencyList.size(); i++) {
            adjacencyList[i].clear();
            adjacencyList[i].shrink_to_fit();
        }
        adjacencyList.clear();
        adjacencyList.shrink_to_fit();

        nodes.clear();
        nodes.shrink_to_fit();
    }

    /*
    size_t getHeight(){
        return height;
    }

    size_t getWidth(){
        return width;
    }

    const CellType getNodesType(VertexT v){
        return nodes[v];
    }
    */

    const NeighborT getNeighbors(VertexT v) const override {
        return adjacencyList[v];
    }

    CostT cost(VertexT from, VertexT to) const override {
        if (from == to) {
            return 0;
        }

        for (LocalEdgeT edge : adjacencyList[from]) {
            if (edge.first == to) {
                return 1;
            }
        }
        
        /*
        CostT estimatedCost(VertexT from, VertexT to) const override {//norm0: zulässig
            return 0;
        }//result: just search it all the direction with the same opportunity
        */

        /*
        CostT estimatedCost(VertexT from, VertexT to) const override {//norm1: zulässig
            int heightFrom = from/width;
            int heightTo = to/width;
            int heightDiff = abs(heightTo - heightFrom);

            int widthFrom = from % width;
            int widthTo = to % width;
            int widthDiff = abs(widthTo - widthFrom);

            double dist = heightDiff+widthDiff;

            return dist;
        }
        */
        
        
        CostT estimatedCost(VertexT from, VertexT to) const override {//norm2: zulässig
            int heightFrom = from/width;
            int heightTo = to/width;
            int heightDiff = heightTo - heightFrom;

            int heightFrom = from / width;
            int heightTo = to / width;
            int heightDiff = heightTo - heightFrom;

            int widthFrom = from % width;
            int widthTo = to % width;
            int widthDiff = widthTo - widthFrom;

                return dist;
        }
        

        /*
        CostT estimatedCost(VertexT from, VertexT to) const override {//2*norm2: nicht zulässig
            int heightFrom = from/width;
            int heightTo = to/width;
            int heightDiff = heightTo - heightFrom;

            int widthFrom = from % width;
            int widthTo = to % width;
            int widthDiff = widthTo - widthFrom;

            double dist = 2* sqrt(pow(heightDiff, 2) + pow(widthDiff, 2));

            return dist;
        }
        */
        
        



        return dist;
    }

    void resisze(int num_verts) {
        vertexCount = num_verts;
        nodes.resize(num_verts);
        adjacencyList.resize(num_verts);
    }

    void addEdge(VertexT vertex, LocalEdgeT edge) {
        adjacencyList[vertex].push_back(edge);
    }

    void calcAdjacencyList() {
        pair<VertexT, CostT> edge;

        for (size_t i = 0; i < numVertices(); i++) {
            if (nodes[i] == CellType::Ground) {
                if (i % width > 0 && nodes[i - 1] == CellType::Ground) {
                    edge.first = i - 1;
                    edge.second = 1;
                    addEdge(i, edge);
                }

                if (i % width < width - 1 && nodes[i + 1] == CellType::Ground) {
                    edge.first = i + 1;
                    edge.second = 1;
                    addEdge(i, edge);
                }

                if (i >= width && nodes[i - width] == CellType::Ground) {
                    edge.first = i - width;
                    edge.second = 1;
                    addEdge(i, edge);
                }

                if (i < numVertices() - width && nodes[i + width] == CellType::Ground) {
                    edge.first = i + width;
                    edge.second = 1;
                    addEdge(i, edge);
                }
            }
        }
    }

    friend std::istream &operator>>(std::istream &, MazeGraph &);
};

istream &operator>>(istream &s, MazeGraph &graph) {
    char currNode;

    s >> graph.height;
    s >> graph.width;

    graph.resisze(graph.height * graph.width);

    for (size_t i = 0; i < graph.height * graph.width; i++) {
        s >> currNode;

        if (currNode == '#') {
            graph.nodes[i] = CellType::Wall;
        } else if (currNode == '.') {
            graph.nodes[i] = CellType::Ground;
        }
    }

    graph.calcAdjacencyList();

    return s;

}