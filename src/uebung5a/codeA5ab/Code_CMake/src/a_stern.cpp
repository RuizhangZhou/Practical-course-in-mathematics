
#include "astern/text_visualizer.h"
#include "astern/unit.h"
#include "astern/mazegraph.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>//make_heap
#include <map>
#include "astern/graphs.h"
#include "astern/visualiser.h"

using namespace std;

// Ein Graph, der Koordinaten von Knoten speichert.

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


vector<CostT> f_v;//f(v)=g(v)+h(v)//global variable than can be got from any function

bool comp(const VertexT &a, const VertexT &b) {
    return f_v[a] > f_v[b];
}


bool A_star(const DistanceGraph &g, GraphVisualizer &v, VertexT start, VertexT ziel, std::list<VertexT> &weg) {
    // ...
    //vector<CostT> f_v;//f(v)=g(v)+h(v)
    vector<CostT> g_v;
    vector<VertexT> vorgaenger(g.numVertices());
    vector<VertexT> bekannteKnoten;
    vector<VertexStatus> statuses(g.numVertices());
    bekannteKnoten.push_back(start);

    f_v.clear();
    for (size_t i = 0; i < g.numVertices(); i++) {
        g_v.push_back(infty);
        f_v.push_back(infty);
        vorgaenger.push_back(undefinedVertex);
        statuses.push_back(VertexStatus::UnknownVertex);
        if (i != ziel) {
            v.markVertex(i, VertexStatus::UnknownVertex);
        }
    }
    g_v[start] = 0;
    f_v[start] = g.estimatedCost(start, ziel);
    statuses[start] = VertexStatus::InQueue;
    v.markVertex(start, VertexStatus::InQueue);

    while (!bekannteKnoten.empty()) {
        //use the priority queue(make_heap) which mentioned in the script
        //here should compare the VertexT by f_v

        //make_heap(bekannteKnoten.begin(), bekannteKnoten.end(),comp);//smallest value at the front,but how to rewrite the comparator for VertexT here?
        //pop_heap is enough, because it first make a heap, then move the top element to the end
        pop_heap(bekannteKnoten.begin(), bekannteKnoten.end(), comp);//smallest value move to the back
        VertexT minVertexT = bekannteKnoten.back();//now curVertexT is the smallest one
        bekannteKnoten.pop_back();//remove the minVertexT from the bekannteKnoten
        v.markVertex(minVertexT, VertexStatus::Active);
        v.draw();
        /*
        VertexT minVertexT=undefinedVertex;
        CostT minCost=infty;
        for(auto vertex : bekannteKnoten){
            if(f_v[vertex] < minCost){
                minCost=f_v[vertex];
                minVertexT=vertex;
            }
        }
        remove(bekannteKnoten.begin(),bekannteKnoten.end(),minVertexT);
        */

        if (minVertexT == ziel) {
            VertexT curV = ziel;
            weg.clear();
            weg.push_front(ziel);
            while (curV != start) {
                v.markEdge(make_pair(vorgaenger[curV], curV), EdgeStatus::Optimal);
                curV = vorgaenger[curV];
                weg.push_front(curV);
            }
            v.draw();
            v.finish();
            return true;
        }
        //here I just split all the Vertex to 3 Status:UnknownVertex,InQueue,Done
        
        for (auto curE : g.getNeighbors(minVertexT)) {
            v.markEdge(make_pair(minVertexT, curE.first), EdgeStatus::Active);
            if (statuses[curE.first] != VertexStatus::Done) {
                CostT newg_v = g_v[minVertexT] + g.cost(minVertexT, curE.first);
                if (newg_v < g_v[curE.first]) {
                    vorgaenger[curE.first] = minVertexT;
                    v.updateVertex(curE.first, newg_v, g.estimatedCost(curE.first, ziel), minVertexT,
                                   VertexStatus::InQueue);
                    g_v[curE.first] = newg_v;
                    f_v[curE.first] = newg_v + g.estimatedCost(curE.first, ziel);
                }
                if (statuses[curE.first] == VertexStatus::UnknownVertex) {
                    statuses[curE.first] = VertexStatus::InQueue;
                    v.markVertex(curE.first, VertexStatus::InQueue);
                    bekannteKnoten.push_back(curE.first);
                }
            }
            v.draw();
            v.markEdge(make_pair(minVertexT, curE.first), EdgeStatus::Visited);
        }

        statuses[minVertexT] = VertexStatus::Done;
        v.markVertex(minVertexT, VertexStatus::Done);
        //v.draw();//do we have to draw here in the CoordinateVisiuliser? 
        //Cuz here we don't have active nodes, so that I can't show the active routes to the active nodes in Labyrinthe
    }
    v.draw();//if no weg has been found, then need to draw() the last situation here
    v.finish();
    return false; // Kein Weg gefunden.
}

void dijkstra_test(const DistanceGraph &graph, int example) {
    for (size_t v = 0; v < graph.numVertices(); v++) {
        vector<CostT> cost(graph.numVertices());
        Dijkstra(graph, v, cost);
        PruefeDijkstra(example, v, cost);
    }
}

void a_star_test(const CoordinateGraph &g, int example) {
    for (size_t v1 = 0; v1 < g.numVertices(); v1++) {
        for (size_t v2 = 0; v2 < g.numVertices(); v2++) {
            if (v1 != v2) {
                cout << "Sollen die Texte angezeigt werden? (y/n)" << endl;
                char s;
                cin >> s;
                bool show_texts = s == 'y';
                list<VertexT> weg(g.numVertices());
                CoordinateGraphVisualiser v(g, v1, v2, show_texts);
                if (A_star(g, v, v1, v2, weg)) {
                    PruefeWeg(example, weg);
                }
            }
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
        a_star_test(graph, 1);
    } else if (example == 2) {
        CircEucliGraph graph;
        ifstream s("daten/Graph2.dat");
        s >> graph;
        PruefeHeuristik(graph);
        dijkstra_test(graph, 2);
        a_star_test(graph, 2);
    } else if (example == 3) {
        LongDistCoordGraph graph;
        ifstream s("daten/Graph3.dat");
        s >> graph;
        PruefeHeuristik(graph);
        dijkstra_test(graph, 3);
        a_star_test(graph, 3);
    } else if (example == 4) {
        TimeCoordGraph graph;
        ifstream s("daten/Graph4.dat");
        s >> graph;
        PruefeHeuristik(graph);
        dijkstra_test(graph, 4);
        a_star_test(graph, 4);
    } else if (example >= 5 && example <= 9) {
        MazeGraph graph;
        int num = example - 4;
        string file = "daten/Maze" + to_string(num) + ".dat";
        ifstream s(file);
        s >> graph;
        PruefeHeuristik(graph);

        for (auto pair : StartZielPaare(example)) {
            auto start = pair.first;
            auto goal = pair.second;
            list<VertexT> weg(graph.numVertices());
            MazeGraphVisualiser v(graph, start, goal);
            if (A_star(graph, v, start, goal, weg)) {
                PruefeWeg(example, weg);
            }
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
        VertexT start = undefinedVertex;
        VertexT goal = undefinedVertex;
        for (size_t i = 0; i < graph.numVertices(); i++) {
            if (maze[i] == CellType::Start) {
                start = i;
            }
            if (maze[i] == CellType::Destination) {
                goal = i;
            }
        }
        list<VertexT> weg(graph.numVertices());
        MazeGraphVisualiser v(graph, start, goal);
        if (A_star(graph, v, start, goal, weg)) {
            PruefeWeg(example, weg);
        }

    } else {
        cout << "Ungültige Beispielnummer." << endl;
        return -1;
    }


    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg

    return 0;
}
 
