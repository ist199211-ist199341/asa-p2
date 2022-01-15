#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>

#define NUM_CHILD 2

typedef struct
{
    int self;
    std::vector<int> parents;
    int son[NUM_CHILD];
} node;

std::vector<int> order1;
std::vector<int> order2;

void search(int vertice1, node *all_nodes);

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

    search(vertice1, tree);
    search(vertice2, tree);

    for (int num : order1)
    {
        printf("%d ", num);
    }
    printf("\n");

    return 0;
}
void search(int vertice1, node *all_nodes)
{
    order1.push_back(vertice1);

    for (int parent : all_nodes[vertice1].parents)
    {
        search(parent, all_nodes);
    }
}
