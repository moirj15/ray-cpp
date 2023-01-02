#pragma once

#include <vector>

namespace geometry
{

class OctTree
{
public:
    struct Node {
    };
private:
    std::vector<Node> m_nodes;
};

} // namespace geometry