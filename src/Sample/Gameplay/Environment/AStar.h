#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <algorithm>
#include <SFML/System/Vector2.hpp>

struct Vec2iHash {
    std::size_t operator()(const sf::Vector2i& v) const {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
    }
};

class Pathfinder {
    struct Node {
        sf::Vector2i Pos;
        int G = 0;
        int H = 0;
        int F = 0;

        bool operator>(const Node& other) const { return F > other.F; }
    };

public:
    static std::vector<sf::Vector2i> FindPath(
        sf::Vector2i start,
        sf::Vector2i target,
        const std::unordered_set<sf::Vector2i, Vec2iHash>& solidBlocks)
    {
        std::vector<sf::Vector2i> path;
        if (start == target) return path;

        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
        std::unordered_set<sf::Vector2i, Vec2iHash> closedSet;
        std::unordered_map<sf::Vector2i, sf::Vector2i, Vec2iHash> cameFrom;
        std::unordered_map<sf::Vector2i, int, Vec2iHash> gScore;

        auto isSolid = [&](int x, int y) { return solidBlocks.contains({x, y}); };

        openSet.push({start, 0, Heuristic(start, target), Heuristic(start, target)});
        gScore[start] = 0;

        while (!openSet.empty()) {
            sf::Vector2i current = openSet.top().Pos;
            openSet.pop();

            if (current == target) {
                sf::Vector2i curr = target;
                while (curr != start) {
                    path.push_back(curr);
                    curr = cameFrom[curr];
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            closedSet.insert(current);
            std::vector<sf::Vector2i> neighbors = GetPlatformerNeighbors(current, isSolid);

            for (const auto& next : neighbors) {
                if (closedSet.contains(next)) continue;

                int tentativeG = gScore[current] + 10;
                if (!gScore.contains(next) || tentativeG < gScore[next]) {
                    cameFrom[next] = current;
                    gScore[next] = tentativeG;
                    int fScore = tentativeG + Heuristic(next, target);
                    openSet.push({next, tentativeG, Heuristic(next, target), fScore});
                }
            }
        }
        return path;
    }

private:
    static int Heuristic(sf::Vector2i a, sf::Vector2i b) {
        return (std::abs(a.x - b.x) + std::abs(a.y - b.y)) * 10;
    }

    template<typename Func>
    static std::vector<sf::Vector2i> GetPlatformerNeighbors(sf::Vector2i pos, Func isSolid) {
        std::vector<sf::Vector2i> neighbors;
        int x = pos.x;
        int y = pos.y;

        if (!isSolid(x - 1, y) && isSolid(x - 1, y - 1)) neighbors.push_back({x - 1, y});
        if (!isSolid(x + 1, y) && isSolid(x + 1, y - 1)) neighbors.push_back({x + 1, y});

        if (!isSolid(x, y + 1) && !isSolid(x - 1, y + 1) && isSolid(x - 1, y)) neighbors.push_back({x - 1, y + 1});
        if (!isSolid(x, y + 1) && !isSolid(x + 1, y + 1) && isSolid(x + 1, y)) neighbors.push_back({x + 1, y + 1});

        if (!isSolid(x - 1, y) && !isSolid(x - 1, y - 1) && isSolid(x - 1, y - 2)) neighbors.push_back({x - 1, y - 1});
        if (!isSolid(x + 1, y) && !isSolid(x + 1, y - 1) && isSolid(x + 1, y - 2)) neighbors.push_back({x + 1, y - 1});

        return neighbors;
    }
};

#endif