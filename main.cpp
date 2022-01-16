#include <iostream>
#include <set>
#include <string.h>
#include <bits/stdc++.h>

#define NUM_PARENT 2

typedef struct
{
    int self;
    std::set<int> parents;
    std::set<int> children;
} node;

std::set<int> all_possible;
/*
call all the parents of the node
*/
void go_up(int current, int target, node *all_nodes);
/*
goes to the children of the node and check if they are equal to the target
*/
bool go_down(int original, int current, int target, node *all_nodes);

int main()
{
    int node1_i, node2_i, num_vertices, num_edges;
    std::cin >> node1_i >> node2_i >> num_vertices >> num_edges;

    node tree[num_vertices + 1];

    for (int i = 1; i < num_vertices + 1; ++i)
    {
        node new_node;
        new_node.self = i;
        new_node.parents = std::set<int>();
        new_node.children = std::set<int>();
        tree[i] = new_node;
    }

    for (int i = 0; i < num_edges; ++i)
    {
        int start, stop;
        std::cin >> start >> stop;

        tree[start].children.insert(stop);
        tree[stop].parents.insert(start);

        if (tree[stop].parents.size() > NUM_PARENT)
        {
            // a node cannot have more than 2 parents, invalid tree
            std::cout << 0 << std::endl;
            return 0;
        }
    }

    // needs to make to both vertices
    go_up(node1_i, node2_i, tree);
    go_up(node2_i, node1_i, tree);

    if (all_possible.empty())
    {
        std::cout << "-";
    }
    else
    {
        for (int value : all_possible)
        {
            std::cout << value << ' ';
        }
    }
    std::cout << std::endl;

    return 0;
}

void go_up(int current, int target, node *all_nodes)
{

    for (int above : all_nodes[current].parents)
    {
        // add number to vector
        if (go_down(current, above, target, all_nodes))
        {
            all_possible.insert(above);
            // return because the above are not the closest
            return;
        }
        else
        {
            go_up(above, target, all_nodes);
        }
    }
}

bool go_down(int original, int current, int target, node *all_nodes)
{
    // it means that it went up and got down to the same node, it would give a bad output if not taken into consideration
    if (original == current)
        return false;

    // if they are equal, that means that we can reach the other vertice by the same node
    if (current == target)
    {
        return true;
    }

    // visit sons
    for (int child : all_nodes[current].children)
    {
        if (go_down(original, child, target, all_nodes))
            return true;
    }
    return false;
}