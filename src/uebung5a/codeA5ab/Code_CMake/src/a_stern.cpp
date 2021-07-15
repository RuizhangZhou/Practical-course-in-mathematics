
#include "astern/text_visualizer.h"
#include "astern/unit.h"
#include "astern/mazegraph.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>//make_heap
#include <functional>//greater
#include <map>

using namespace std;

// Ein Graph, der Koordinaten von Knoten speichert.
class CoordinateGraph : public DistanceGraph {
protected:
    vector<NeighborT> adjacencyList;
    vector<pair<double, double>> coordinates;

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

    friend std::istream &operator>>(std::istream &, CoordinateGraph &);
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
        double lon1 = coordinates[from].second * M_PI / 180;
        double lon2 = coordinates[to].second * M_PI / 180;
        double lat1 = coordinates[from].first * M_PI / 180;
        double lat2 = coordinates[to].first * M_PI / 180;
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

    CostT estimatedCost(VertexT from, VertexT to) const override {
        return (latLongToDist(from, to) / 200.0) * 60.0;
    }
};

istream &operator>>(istream &s, CoordinateGraph &graph) {
    int num_verts;
    int num_edges;
    VertexT vertex;
    pair<VertexT, CostT> edge;
    pair<double, double> coord;

    s >> num_verts;

    graph.resisze(num_verts);

    s >> num_edges;

    for (int i = 0; i < num_edges; i++) {
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

void Dijkstra(const DistanceGraph &g, VertexT start, std::vector<CostT> &kostenZumStart) {
    map<VertexT, CostT> remaining;
    for (size_t i = 0; i < g.numVertices(); i++) {
        if (i == start) {
            kostenZumStart[i] = 0;
        } else {
            kostenZumStart[i] = infty;
            remaining[i] = infty;
        }
    }

    for (pair<VertexT, CostT> t : g.getNeighbors(start)) {
        remaining[t.first] = t.second;
    }

    bool all_infinite = false;
    while (not remaining.empty() and not all_infinite) {
        pair<VertexT, CostT> minimal = make_pair(0, infty);
        for (pair<VertexT, CostT> p : remaining) {
            if (p.second < minimal.second) {
                minimal = p;
            }
        }
        if (minimal.second != infty) {
            remaining.erase(minimal.first);
            kostenZumStart[minimal.first] = minimal.second;

            for (pair<VertexT, CostT> t : g.getNeighbors(minimal.first)) {
                if (remaining.find(t.first) !=
                    remaining.end()) { //There is no contains method in a map, this is a common workaround.
                    remaining[t.first] = min(minimal.second + t.second, remaining[t.first]);
                }
            }
        } else {
            all_infinite = true;
        }
    }
}

vector<CostT> f_v;//f(v)=g(v)+h(v)

bool comp(const VertexT &a,const VertexT &b){
    return f_v[a] > f_v[b];
}

bool A_star(const DistanceGraph &g, /*GraphVisualizer &v,*/ VertexT start, VertexT ziel, std::list<VertexT> &weg) {
    // ...
    vector<CostT> g_v;
    vector<VertexT> vorgaenger(g.numVertices());
    vector<VertexT> bekannteKnoten;
    vector<VertexStatus> statuses(g.numVertices());

    bekannteKnoten.push_back(start);
    f_v.clear();
    for (size_t i = 0; i < g.numVertices(); i++) {
        g_v.push_back(infty);
        
        f_v.push_back(infty);
        statuses.push_back(VertexStatus::UnknownVertex);
    }
    g_v[start] = 0;
    f_v[start] = g.estimatedCost(start, ziel);
    statuses[start]=VertexStatus::InQueue;

    while (!bekannteKnoten.empty()) {
        //use the priority queue(make_heap) which mentioned in the script
        //here should compare the VertexT by f_v

        make_heap(bekannteKnoten.begin(), bekannteKnoten.end(),
                  comp);//smallest value at the front,but how to rewrite the comparator for VertexT here?
        pop_heap(bekannteKnoten.begin(), bekannteKnoten.end(), comp);//smallest value move to the back
        VertexT minVertexT = bekannteKnoten.back();//now curVertexT is the smallest one
        if (minVertexT == ziel) {
            VertexT curV = ziel;
            weg.clear();
            weg.push_front(ziel);
            while (curV != start) {
                curV = vorgaenger[curV];
                weg.push_front(curV);
            }
            return true;
        }
        bekannteKnoten.pop_back();//remove the minVertexT from the bekannteKnoten
        statuses[minVertexT]=VertexStatus::Done;
        for (auto curE : g.getNeighbors(minVertexT)) {
            if (statuses[curE.first]!=VertexStatus::Done) {
                CostT newg_v = g_v[minVertexT] + g.cost(minVertexT, curE.first);
                if (newg_v < g_v[curE.first]) {
                    vorgaenger[curE.first] = minVertexT;
                    g_v[curE.first] = newg_v;
                    f_v[curE.first] = newg_v + g.estimatedCost(curE.first, ziel);
                }
                statuses[curE.first]=VertexStatus::InQueue;//as long as it's not "Done", all set as "InQueue"(no matter it's "Unknown" or "inQueue");
            }
        }
    }
    return false; // Kein Weg gefunden.
}

void dijkstra_test(const DistanceGraph &graph, int example) {
    for (size_t v = 0; v < graph.numVertices(); v++) {
        vector<CostT> cost(graph.numVertices());
        Dijkstra(graph, v, cost);
        PruefeDijkstra(example, v, cost);
    }
}

void a_star_test(const DistanceGraph &g, int example){
    for(size_t v1=0; v1<g.numVertices();v1++){
        for(size_t v2=0;v2<g.numVertices();v2++){
            list<VertexT> weg(g.numVertices());
            A_star(g,v1,v2,weg);
            PruefeWeg(example,weg);
        }
    }
}

int main() {
    // Frage Beispielnummer vom User ab
    // Lade die zugehoerige Textdatei in einen Graphen
    // PruefeHeuristik
    int example;
    cout << "Geben Sie eine Beispielnummer zwischen 1 und 10 ein." << endl;
    cin >> example;

    if (example == 1) {
        ShortDistEucliGraph graph;
        ifstream s("daten/Graph1.dat");
        s >> graph;
        PruefeHeuristik(graph);
        dijkstra_test(graph, 1);
        a_star_test(graph,1);
    } else if (example == 2) {
        CircEucliGraph graph;
        ifstream s("daten/Graph2.dat");
        s >> graph;
        PruefeHeuristik(graph);
        dijkstra_test(graph, 2);
        a_star_test(graph,2);
    } else if (example == 3) {
        LongDistCoordGraph graph;
        ifstream s("daten/Graph3.dat");
        s >> graph;
        PruefeHeuristik(graph);
        dijkstra_test(graph, 3);
        a_star_test(graph,3);
    } else if (example == 4) {
        TimeCoordGraph graph;
        ifstream s("daten/Graph4.dat");
        s >> graph;
        PruefeHeuristik(graph);
        dijkstra_test(graph, 4);
        a_star_test(graph,4);
    } else if (example >= 5 && example <= 9) {
        MazeGraph graph;
        int num = example - 4;
        string file = "daten/Maze" + to_string(num) + ".dat";
        ifstream s(file);
        s >> graph;
        PruefeHeuristik(graph);
        for ( auto pair : StartZielPaare(example)) {
            auto start = pair.first;
            auto goal  = pair.second;
            list<VertexT> weg(graph.numVertices());
            A_star(graph,start,goal,weg);
            PruefeWeg(example,weg);
            //(Berechne den kuerzesten Weg von start zu goal)
        }
    } else if (example == 10) {
        unsigned int seed;
        cout << "Geben Sie einen seed ein." << endl;
        cin >> seed;
        vector<CellType> maze;
        maze = ErzeugeLabyrinth(256, 256, seed);
        MazeGraph graph(maze, 256, 256);
        //which start and goal should we choose here?
    } else {
        cout << "Ungültige Beispielnummer." << endl;
        return -1;
    }

    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg

    return 0;
}
 
