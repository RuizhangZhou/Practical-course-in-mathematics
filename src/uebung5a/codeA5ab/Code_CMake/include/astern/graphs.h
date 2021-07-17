//
// Created by noeske on 17.07.21.
//

#ifndef ASTERN_GRAPHS_H
#define ASTERN_GRAPHS_H

class CoordinateGraph : public DistanceGraph {
public:
    vector<NeighborT> adjacencyList;
    vector<pair<double, double>> coordinates;

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
        //if from and to are not neighbors, cost(from,to)=cost(to,from)=infty
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

    double latLongToDist(VertexT from, VertexT to) const {//coordinates here are Longitude and Latitude
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
        return (latLongToDist(from, to) / 200.0) * 60.0;//estimate by driving a car with 200km/h
    }
};

#endif //ASTERN_GRAPHS_H
