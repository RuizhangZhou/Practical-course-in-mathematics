
#include "astern/text_visualizer.h"
#include "astern/unit.h"
#include <vector>
#include <algorithm>//make_heap
#include <functional>//greater

using namespace std;

// Ein Graph, der Koordinaten von Knoten speichert.
class CoordinateGraph : public DistanceGraph {
public:
    const NeighborT getNeighbors( VertexT v) const override;
    
    CostT estimatedCost( VertexT from, VertexT to) const override;
    
    CostT cost( VertexT from, VertexT to) const override;
};

class DisCoordGraph : public CoordinateGraph {
    public:
        CostT estimatedCost( VertexT from, VertexT to) const override{

        }
};

class TimeCoordGraph : public CoordinateGraph{
    public:
        CostT estimatedCost( VertexT from, VertexT to) const override{

        }
};

class MazeGraph : public DistanceGraph {
    const NeighborT getNeighbors( VertexT v) const override{

    }
    
    CostT estimatedCost( VertexT from, VertexT to) const override{

    }
    
    CostT cost( VertexT from, VertexT to) const override{

    }
};

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
    
    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg
    
    return 0;
}
 
