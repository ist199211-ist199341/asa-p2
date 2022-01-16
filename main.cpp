#include <iostream>
#include <set>
#include <string.h>
#include <bits/stdc++.h>

#define NUM_CHILD 2

typedef struct
{
    int self;
    std::set<int> parents;
    std::set<int> children;
} node_t;

typedef enum color
{
    WHITE,
    GRAY,
    BLACK
} color_t;

typedef struct
{
    color_t color;
    int distance;
    int pi;
    node_t *node;
} bfs_node;

std::set<int> *get_reachable_nodes(int start_node, int node_count, node_t *tree);
void strip_parents(int start_node, std::set<int> *set, node_t *tree);

int main()
{
    int node1_i, node2_i, num_vertices, num_edges;
    std::cin >> node1_i >> node2_i >> num_vertices >> num_edges;

    node_t tree[num_vertices];

    for (int i = 0; i < num_vertices; ++i)
    {
        tree[i].self = i;
        tree[i].parents = std::set<int>();
        tree[i].children = std::set<int>();
    }

    for (int i = 0; i < num_edges; ++i)
    {
        int start, stop;
        std::cin >> start >> stop;

        tree[start - 1].children.insert(stop - 1);
        tree[stop - 1].parents.insert(start - 1);

        if (tree[stop - 1].parents.size() > 2)
        {
            // a node cannot have more than 2 parents, invalid tree
            std::cout << 0 << std::endl;
            return 0;
        }
    }

    // execute "reverse" BFS
    std::set<int> *v1_parents = get_reachable_nodes(node1_i - 1, num_vertices, tree);
    std::set<int> *v2_parents = get_reachable_nodes(node2_i - 1, num_vertices, tree);
    std::set<int> common_parents;

    // find the common parents
    for (int el : *v1_parents)
    {
        if (v2_parents->count(el) > 0)
        {
            common_parents.insert(el);
        }
    }
    for (int el : common_parents)
    {
        strip_parents(el, &common_parents, tree);
    }

    if (common_parents.empty())
    {
        std::cout << "-";
    }
    else
    {
        for (int value : common_parents)
        {
            std::cout << value + 1 << ' ';
        }
    }
    std::cout << std::endl;

    return 0;
}

std::set<int> *get_reachable_nodes(int start_node, int node_count, node_t *tree)
{
    bfs_node nodes[node_count];

    // init bfs_nodes
    for (int i = 0; i < node_count; ++i)
    {
        nodes[i].color = WHITE;
        nodes[i].distance = -1;
        nodes[i].pi = -1;
        nodes[i].node = tree + i;
    }
    nodes[start_node].color = GRAY;
    nodes[start_node].distance = 0;

    // apply bfs_algorithm
    std::queue<int> queue;
    queue.push(start_node);

    while (!queue.empty())
    {
        int node_i = queue.front();
        queue.pop();

        for (int parent : nodes[node_i].node->parents)
        {
            if (nodes[parent].color == WHITE)
            {
                nodes[parent].color = GRAY;
                nodes[parent].distance = nodes[node_i].distance + 1;
                nodes[parent].pi = node_i;
                queue.push(parent);
            }
        }
        nodes[node_i].color = BLACK;
    }

    std::set<int> *reached_nodes = new std::set<int>;

    for (int i = 0; i < node_count; ++i)
    {
        if (nodes[i].distance >= 0)
        {
            reached_nodes->insert(i);
        }
    }

    return reached_nodes;
}

void strip_parents(int start_node, std::set<int> *set, node_t *tree)
{
    for (int parent : tree[start_node].parents)
    {
        auto set_it = set->find(parent);
        if (set_it != set->end())
        {
            set->erase(set_it);
        }
        strip_parents(parent, set, tree);
    }
}