#include "astern/unit.h"
#include "astern/graphs.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <cmath>


using namespace std;

const sf::Color GREY = sf::Color(0x90a4aeff);
const sf::Color RED = sf::Color(0xf44336ff);
const sf::Color ORANGE = sf::Color(0xff9800ff);
const sf::Color YELLOW = sf::Color(0xffeb3bff);
const sf::Color GREEN = sf::Color(0x4caf50ff);
const sf::Color BLUE = sf::Color(0x2196f3ff);
const sf::Color DARK_GREY = sf::Color(0x546e7aff);
const sf::Color DARK_GREEN = sf::Color(0x388e3cff);
const sf::Color DARK_RED = sf::Color(0xd32f2fff);

class CoordinateGraphVisualiser : public GraphVisualizer {
    struct VertexData {
        VertexStatus status;
        double cost;
        double estimate;
        VertexT parent;
    };

    vector<VertexData> vertex_data;

    struct EdgeData {
        EdgeStatus status;
    };

    map<EdgeT, EdgeData> edge_data;

    sf::RenderWindow window;

    VertexT start;
    VertexT end;

    const CoordinateGraph &graph;

public:
    CoordinateGraphVisualiser(const CoordinateGraph &graph, VertexT start, VertexT end) :
            vertex_data(graph.numVertices()),
            window(sf::VideoMode(1800, 1800), "My window"),
            start(start), end(end),
            graph(graph) {
        for (size_t i = 0; i < vertex_data.size(); i++) {
            vertex_data[i].status = VertexStatus::UnknownVertex;
            vertex_data[i].cost = infty;
            vertex_data[i].estimate = graph.estimatedCost(i, end);
            vertex_data[i].parent = SIZE_MAX;
        }
        vertex_data[start].status = VertexStatus::Active;
        vertex_data[end].status = VertexStatus::Destination;

        for (size_t i = 0; i < graph.numVertices(); i++) {
            for (LocalEdgeT t : graph.adjacencyList[i]) {
                edge_data[make_pair(i, t.first)].status = EdgeStatus::UnknownEdge;
            }
        }
    }

    ~CoordinateGraphVisualiser() override {

    }

    void markVertex(VertexT vertex, VertexStatus status) override {
        vertex_data[vertex].status = vertex == end ? VertexStatus::Destination : status;;
    }

    void markEdge(EdgeT edge, EdgeStatus status) override {
        edge_data[edge].status = status;
    }

    void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status) override {
        vertex_data[vertex].status = vertex == end ? VertexStatus::Destination : status;
        vertex_data[vertex].cost = cost;
        //vertex_data[vertex].estimate = estimate; //Not needed because estimates are calculated in the constructor.
        vertex_data[vertex].parent = parent;
    }

    void draw() override {
        if (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) // event loop
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::Resized) {
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                }
            }

            auto window_size = window.getSize();
            unsigned int size = max(min(window_size.x, window_size.y), 40u) - 40;
            unsigned int offset_x = (window_size.x - size) / 2;
            unsigned int offset_y = (window_size.y - size) / 2;

            double coord_min_x = 1000;
            double coord_max_x = -1000;
            double coord_min_y = 1000;
            double coord_max_y = -1000;
            for (pair<double, double> coord : graph.coordinates) {
                coord_min_x = min(coord.second, coord_min_x);
                coord_max_x = max(coord.second, coord_max_x);
                coord_min_y = min(-coord.first, coord_min_y);
                coord_max_y = max(-coord.first, coord_max_y);
            }
            auto get_x = [=](double x) {
                return (float) (((x - coord_min_x) / (coord_max_x - coord_min_x)) * size + offset_x);
            };
            auto get_y = [=](double y) {
                return (float) ((((-y) - coord_min_y) / (coord_max_y - coord_min_y)) * size + offset_y);
            };

            window.clear(sf::Color::White);

            sf::CircleShape vertex_shape(20);
            vertex_shape.setFillColor(GREY);
            vertex_shape.setOrigin(vertex_shape.getRadius(), vertex_shape.getRadius());
            for (size_t i = 0; i < vertex_data.size(); i++) {
                if (i == start) {
                    vertex_shape.setFillColor(RED);
                } else if (vertex_data[i].status == VertexStatus::Destination) {
                    vertex_shape.setFillColor(BLUE);
                } else if (vertex_data[i].status == VertexStatus::Done) {
                    vertex_shape.setFillColor(GREEN);
                } else if (vertex_data[i].status == VertexStatus::Active) {
                    vertex_shape.setFillColor(YELLOW);
                } else if (vertex_data[i].status == VertexStatus::InQueue) {
                    vertex_shape.setFillColor(ORANGE);
                } else {
                    vertex_shape.setFillColor(GREY);
                }
                vertex_shape.setPosition(get_x(graph.coordinates[i].second), get_y(graph.coordinates[i].first));
                window.draw(vertex_shape);
            }

            for (pair<EdgeT, EdgeData> t : edge_data) {
                sf::Color line_color;
                if (t.second.status == EdgeStatus::Active) {
                    line_color = DARK_GREEN;
                } else if (t.second.status == EdgeStatus::Optimal) {
                    line_color = DARK_RED;
                } else if (t.second.status == EdgeStatus::Visited) {
                    line_color = DARK_GREY;
                } else {
                    line_color = GREY;
                }
                sf::Vertex line[] = {
                        sf::Vertex(sf::Vector2f(
                                get_x(graph.coordinates[t.first.first].second),
                                get_y(graph.coordinates[t.first.first].first)
                        ), line_color),
                        sf::Vertex(sf::Vector2f(
                                get_x(graph.coordinates[t.first.second].second),
                                get_y(graph.coordinates[t.first.second].first)
                        ), line_color)
                };
                window.draw(line, 2, sf::Lines);

                double rad;
                if (line[1].position.x - line[0].position.x != 0) {
                    rad = atan((line[1].position.y - line[0].position.y) / (line[1].position.x - line[0].position.x));
                } else {
                    rad = line[0].position.y < line[1].position.y ? -acos(0) : acos(0);
                }

                double invert = line[0].position.x < line[1].position.x ? 1 : -1;

                double tip_x = line[0].position.x + 0.7 * (line[1].position.x - line[0].position.x);
                double tip_y = line[0].position.y + 0.7 * (line[1].position.y - line[0].position.y);
                double add_x = sin(rad) * 20;
                double add_y = cos(rad) * 20;
                double left_x = tip_x + invert * ( + add_x - add_y);
                double left_y = tip_y + invert * (- add_y - add_x);
                double right_x = tip_x + invert * ( - add_x - add_y);
                double right_y = tip_y + invert * ( + add_y - add_x);

                sf::Vertex triangle[] = {
                        sf::Vertex(sf::Vector2f(
                                tip_x,
                                tip_y
                        ), line_color),
                        sf::Vertex(sf::Vector2f(
                                left_x,
                                left_y
                        ), line_color),
                        sf::Vertex(sf::Vector2f(
                                right_x,
                                right_y
                        ), line_color)
                };
                window.draw(triangle, 3, sf::Triangles);
            }


            window.display();
            sf::sleep(sf::milliseconds(1000));
        }
    }
};
