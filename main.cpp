#include <iostream>
#include <vector>
#include <string.h>
#include <bits/stdc++.h>

#define NUM_CHILD 2

typedef struct
{
    int self;
    std::vector<int> parents;
    int son[NUM_CHILD];
} node;

std::vector<int> all_possible;

void go_up(int current, int target, node *all_nodes);
bool go_down(int original, int current, int target, node *all_nodes);

int main()
{
    int vertice1, vertice2;
    scanf("%d %d", &vertice1, &vertice2);

    int num_vertices, num_edges;
    scanf("%d %d", &num_vertices, &num_edges);

    node tree[num_vertices + 1];

    for (int i = 0; i < num_vertices + 1; i++)
    {
        node new_node;
        new_node.self = i;
        new_node.parents = std::vector<int>();
        new_node.son[0] = -1;
        new_node.son[1] = -1;
        tree[i] = new_node;
    }

    int c = 0;

    int start, stop;
    while (c != EOF)
    {
        scanf("%d %d", &start, &stop);
        // more than two children
        if (tree[start].son[0] != -1 && tree[start].son[1] != -1)
        {
            printf("0\n");
            return 0;
        }
        if (tree[start].son[0] != -1)
        {
            tree[start].son[1] = stop;
        }
        else
        {
            tree[start].son[0] = stop;
        }
        tree[stop].parents.push_back(start);

        c = getchar();
    }

    go_up(vertice1, vertice2, tree);
    go_up(vertice2, vertice1, tree);

    if (all_possible.empty())
        printf("-");
    else
    {
        sort(all_possible.begin(), all_possible.end());

        for (int value : all_possible)
        {
            printf("%d ", value);
        }
    }
    printf("\n");

    return 0;
}

void go_up(int current, int target, node *all_nodes)
{

    for (int above : all_nodes[current].parents)
    {
        if (go_down(current, above, target, all_nodes) == true)
        {
            all_possible.push_back(above);
            return;
        }
        go_up(above, target, all_nodes);
    }
}
bool go_down(int original, int current, int target, node *all_nodes)
{
    if (original == current)
        return false;

    if (current == target)
    {
        return true;
    }
    for (int i = 0; i < NUM_CHILD; i++)
    {
        if (all_nodes[current].son[i] != -1)
            if (go_down(original, all_nodes[current].son[i], target, all_nodes) == true)
                return true;
    }
    return false;
}