
#include "astern/text_visualizer.h"
#include "astern/unit.h"
#include "astern/mazegraph.h"
#include <vector>
#include <algorithm>//make_heap
#include <functional>//greater
#include <map>

using namespace std;

// Ein Graph, der Koordinaten von Knoten speichert.
class CoordinateGraph : public DistanceGraph {
protected:
    vector<NeighborT> adjacencyList;
    vector<pair<double,double>> coordinates;

public:
    CoordinateGraph(int num_verts = 0)
        : DistanceGraph(num_verts), adjacencyList(num_verts), coordinates(num_verts) {}
    
    virtual ~CoordinateGraph() {}
    
    const NeighborT getNeighbors(VertexT v) const override {
        return adjacencyList[v];
    }

    CostT cost(VertexT from, VertexT to) const override {
        if (from == to) {
            return 0;
        }
        for (LocalEdgeT edge : adjacencyList[from]) {
            if (edge.first == to) {
                return edge.second;
            }
        }
        return infty;
    }

    void resisze(int num_verts) {
        vertexCount = num_verts;
        adjacencyList.resize(num_verts);
        coordinates.resize(num_verts);
    }
    
    void addEdge(VertexT vertex, LocalEdgeT edge) {
        adjacencyList[vertex].push_back(edge);
    }

    void addCoordinates(int vertex, pair<double, double> coord) {
        coordinates[vertex] = coord;
    }

    void updateCost(VertexT from, VertexT to, CostT newCost) const{
        EdgeT curEdge(from,to);//key
        costsMap[curEdge]=newCost;//how to update the key-value pair in <map>?
    }

    friend std::istream& operator>>(std::istream&, CoordinateGraph&);
};

class EucliGraph : public CoordinateGraph {
public:
    EucliGraph(int num_verts = 0)
        : CoordinateGraph(num_verts) {}

    virtual ~EucliGraph() {}

    double euclideanDistance(VertexT from, VertexT to) const {
        double diffLat = coordinates[to].first - coordinates[from].first;
        double diffLon = coordinates[to].second - coordinates[from].second;

        double dist = sqrt(pow(diffLat, 2) + pow(diffLon, 2));

        return dist;
    }
};

class CircEucliGraph : public EucliGraph {
public:
    CircEucliGraph(int num_verts = 0)
        : EucliGraph(num_verts) {}
    
    ~CircEucliGraph() {
        for (size_t i = 0; i < adjacencyList.size(); i++) {
            adjacencyList[i].clear();
            adjacencyList[i].shrink_to_fit();
        }
        adjacencyList.clear();
        adjacencyList.shrink_to_fit();

        coordinates.clear();
        coordinates.shrink_to_fit();
    }
    
    CostT estimatedCost(VertexT from, VertexT to) const override {
        double distance = min(euclideanDistance(from, to), min(cost(from, to), cost(to, from)));
        return distance;
    }
};

class ShortDistEucliGraph : public EucliGraph {
public:
    ShortDistEucliGraph(int num_verts = 0)
        : EucliGraph(num_verts) {}
    
    ~ShortDistEucliGraph() {
        for (size_t i = 0; i < adjacencyList.size(); i++) {
            adjacencyList[i].clear();
            adjacencyList[i].shrink_to_fit();
        }
        adjacencyList.clear();
        adjacencyList.shrink_to_fit();

        coordinates.clear();
        coordinates.shrink_to_fit();
    }
    
    CostT estimatedCost(VertexT from, VertexT to) const override {
        return euclideanDistance(from, to);
    }
};

class LonLatCoordGraph : public CoordinateGraph {
public:
    LonLatCoordGraph(int num_verts = 0)
        : CoordinateGraph(num_verts) {}
    
    virtual ~LonLatCoordGraph() {}

    double latLongToDist(VertexT from, VertexT to) const {
        double earthRadius = 6378.388;
        double lon1 = coordinates[from].second * M_PI/180;
        double lon2 = coordinates[to].second * M_PI/180;
        double lat1 = coordinates[from].first * M_PI/180;
        double lat2 = coordinates[to].first * M_PI/180;
        double dist = earthRadius * acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lon2 - lon1));

        return dist;
    }
};

class LongDistCoordGraph : public LonLatCoordGraph {
public:
    LongDistCoordGraph(int num_verts = 0)
        : LonLatCoordGraph(num_verts) {}
    
    ~LongDistCoordGraph() {
        for (size_t i = 0; i < adjacencyList.size(); i++) {
            adjacencyList[i].clear();
            adjacencyList[i].shrink_to_fit();
        }
        adjacencyList.clear();
        adjacencyList.shrink_to_fit();

        coordinates.clear();
        coordinates.shrink_to_fit();
    }
    
    CostT estimatedCost(VertexT from, VertexT to) const override {
        return latLongToDist(from, to);
    }
};

class TimeCoordGraph : public LonLatCoordGraph {
public:
    TimeCoordGraph(int num_verts = 0)
        : LonLatCoordGraph(num_verts) {}
    
    ~TimeCoordGraph() {
        for (size_t i = 0; i < adjacencyList.size(); i++) {
            adjacencyList[i].clear();
            adjacencyList[i].shrink_to_fit();
        }
        adjacencyList.clear();
        adjacencyList.shrink_to_fit();

        coordinates.clear();
        coordinates.shrink_to_fit();
    }
    
    CostT estimatedCost( VertexT from, VertexT to) const override {
        return (latLongToDist(from, to)/200.0)*60.0;
    }
};

istream& operator>>(istream &s, CoordinateGraph &graph) {
    int num_verts;
    int num_edges;
    VertexT vertex;
    pair<VertexT, CostT> edge;
    pair<double, double> coord;

    s >> num_verts;

    graph.resisze(num_verts);

    s >> num_edges;

    for (int i = 0; i < num_edges; i++){
        s >> vertex;
        s >> edge.first;
        s >> edge.second;
        graph.addEdge(vertex, edge);
    }

    for (int i = 0; i < num_verts; i++) {
        s >> coord.first;
        s >> coord.second;
        graph.addCoordinates(i, coord);
    }

    return s;
}

void Dijkstra(const DistanceGraph& g, GraphVisualizer& v, VertexT start, std::vector<CostT>& kostenZumStart) {
    // ...
}

bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
    // ...
    vector<VertexT> bekannteKnoten;
    vector<VertexT> untersuchterKnoten;
    bekannteKnoten.push_back(start);
    while(!bekannteKnoten.empty()){
        /*use the priority queue(make_heap) which mentioned in the script
        //here should compare the VertexT by g.cost(start,this)+g.estimateCost(this,ziel)

        make_heap(bekannteKnoten.begin(),bekannteKnoten.end(),greater<>{});//smallest value at the front,but how to rewrite the comparator for VertexT here?
        pop_heap(bekannteKnoten.begin(),bekannteKnoten.end(),greater<>{});//smallest value move to the back
        VertexT curVertexT=bekannteKnoten.back();//now curVertexT is the smallest one
        weg.push_back(curVertexT);
        bekannteKnoten.pop_back();
        */
        VertexT minVertexT=undefinedVertex;
        CostT minCost=infty;
        for (VertexT v:bekannteKnoten){//loop to find the minVertexT
            CostT curCost=g.cost(start,v)+g.estimatedCost(v,ziel);//f(v)=g(v)+h(v)
            if(curCost<minCost){
                minVertexT=v;
                minCost=curCost;
            }
        }
        weg.push_back(minVertexT);
        if(minVertexT==ziel){
            return true;
        }
        remove(bekannteKnoten.begin(),bekannteKnoten.end(),minVertexT);
        untersuchterKnoten.push_back(minVertexT);
        auto curNeighborTs=g.getNeighbors(minVertexT);
        for (auto localEdgeT : curNeighborTs){
            // if this neighbor isn't untersucht, then add it into the bekannteKnoten
            if(find(untersuchterKnoten.begin(),untersuchterKnoten.end(),localEdgeT.first)==untersuchterKnoten.end()){
                bekannteKnoten.push_back(localEdgeT.first);
                CostT newCost=g.cost(start,minVertexT)+localEdgeT.second;
                if(newCost < g.cost(start,localEdgeT.first)){
                    g.updateCost(start,localEdgeT.first,newCost);//g is a DistanceGraph, but the updateCost I defined in the Herachie classes, how to debug it?
                }
            }
        }
    }
    return false; // Kein Weg gefunden.
}

int main()
{
    // Frage Beispielnummer vom User ab
    // Lade die zugehoerige Textdatei in einen Graphen
    // PruefeHeuristik
    int example;
    cout << "Geben Sie eine Beispielnummer zwischen 1 und 10 ein." << endl;
    cin >> example;

    if (example == 1) {
        ShortDistEucliGraph graph;
        ifstream s("Graph1.dat");
        s >> graph;
        PruefeHeuristik(graph);
    } else if (example == 2) {
        CircEucliGraph graph;
        ifstream s("Graph2.dat");
        s >> graph;
        PruefeHeuristik(graph);
    } else if (example == 3) {
        LongDistCoordGraph graph;
        ifstream s("Graph3.dat");
        s >> graph;
        PruefeHeuristik(graph);
    } else if (example == 4) {
        TimeCoordGraph graph;
        ifstream s("Graph4.dat");
        s >> graph;
        PruefeHeuristik(graph);
    } else if (example >= 5 && example <= 9) {
        MazeGraph graph;
        int num = example - 4;
        string file = "Maze" + to_string(num) + ".dat";
        ifstream s(file);
        s >> graph;
        PruefeHeuristik(graph);
    } else if (example == 10) {
        unsigned int seed;
        cout << "Geben Sie einen seed ein." << endl;
        cin >> seed;
        vector<CellType> maze;
        maze = ErzeugeLabyrinth(256, 256, seed);
        MazeGraph graph(maze, 256, 256);
    } else {
        cout << "Ungültige Beispielnummer." << endl;
        return -1;
    }
    
    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg
    
    return 0;
}
 
