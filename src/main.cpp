#include <iostream>
#include <queue>
#include <string.h>

#define NUM_PARENTS 2

typedef struct
{
    int parents[NUM_PARENTS];
    int children_count;
    short visited;
} node_t;

typedef enum color
{
    WHITE,
    GRAY,
    BLACK
} color_t;

bool try_insert_array(int *arr, int element, size_t arr_size);

void calculate_reachable_nodes(int start_node, int node_count, node_t *tree);
bool has_loops(int node_count, node_t *tree);
bool dfs_visit_loop_check(color_t *node_colors, node_t *tree, int current_node);

int main()
{
    std::ios::sync_with_stdio(false);
    int node1_i, node2_i, num_vertices, num_edges;
    std::cin >> node1_i >> node2_i >> num_vertices >> num_edges;

    node_t *tree = (node_t *)malloc(num_vertices * sizeof(node_t));

    for (int i = 0; i < num_vertices; ++i)
    {
        for (int j = 0; j < NUM_PARENTS; ++j)
        {
            tree[i].parents[j] = -1;
            tree[i].children_count = 0;
            tree[i].visited = 0;
        }
    }

    for (int i = 0; i < num_edges; ++i)
    {
        int start = -1, stop = -1;
        std::cin >> start >> stop;

        if (start == -1 || stop == -1 || !try_insert_array(tree[stop - 1].parents, start - 1, NUM_PARENTS))
        {
            // a node cannot have more than 2 parents, invalid tree
            std::cout << 0 << std::endl;
            return 0;
        }
        tree[start - 1].children_count += 1;
    }

    if (has_loops(num_vertices, tree))
    {
        // tree cannot have loops, invalid tree
        std::cout << 0 << std::endl;
        return 0;
    }

    // execute "reverse" BFS
    calculate_reachable_nodes(node1_i - 1, num_vertices, tree);
    calculate_reachable_nodes(node2_i - 1, num_vertices, tree);

    // chop off non-common nodes from the tree
    for (int el = 0; el < num_vertices; ++el)
    {
        if (tree[el].visited < 2)
        {
            for (int i = 0; i < NUM_PARENTS; ++i)
            {
                if (tree[el].parents[i] == -1)
                    break;
                tree[tree[el].parents[i]].children_count -= 1;
            }
        }
    }

    // print result
    bool empty = true;
    for (int i = 0; i < num_vertices; ++i)
    {
        if (tree[i].visited >= 2 && tree[i].children_count == 0)
        {
            std::cout << i + 1 << ' ';
            empty = false;
        }
    }
    if (empty)
    {
        std::cout << '-';
    }
    std::cout << std::endl;

    return 0;
}

bool try_insert_array(int *arr, int element, size_t arr_size)
{
    for (size_t i = 0; i < arr_size; ++i)
    {
        if (arr[i] == -1)
        {
            arr[i] = element;
            return true;
        }
    }
    return false;
}

void calculate_reachable_nodes(int start_node, int node_count, node_t *tree)
{
    color_t *node_colors = (color_t *)malloc(node_count * sizeof(color_t));

    // init bfs_nodes
    for (int i = 0; i < node_count; ++i)
    {
        node_colors[i] = WHITE;
    }
    node_colors[start_node] = GRAY;

    // apply bfs_algorithm
    std::queue<int> queue;
    queue.push(start_node);

    while (!queue.empty())
    {
        int node_i = queue.front();
        queue.pop();

        for (int parent : tree[node_i].parents)
        {
            if (parent == -1)
                break;
            if (node_colors[parent] == WHITE)
            {
                node_colors[parent] = GRAY;
                queue.push(parent);
            }
        }
        node_colors[node_i] = BLACK;
        tree[node_i].visited += 1;
    }
}

bool has_loops(int node_count, node_t *tree)
{
    color_t *node_colors = (color_t *)malloc(node_count * sizeof(color_t));

    // initialize DFS nodes
    for (int i = 0; i < node_count; ++i)
    {
        node_colors[i] = WHITE;
    }

    for (int i = 0; i < node_count; ++i)
    {
        if (node_colors[i] == WHITE)
            if (dfs_visit_loop_check(node_colors, tree, i))
            {
                return true;
            }
    }
    return false;
}

bool dfs_visit_loop_check(color_t *node_colors, node_t *tree, int current_node)
{
    node_colors[current_node] = GRAY;
    for (int adj_i : tree[current_node].parents)
    {
        if (adj_i == -1)
            break;
        if (node_colors[adj_i] == WHITE)
        {
            if (dfs_visit_loop_check(node_colors, tree, adj_i))
            {
                // propagate result
                return true;
            }
        }
        else if (node_colors[adj_i] == GRAY)
        {
            // detected a loop!
            return true;
        }
    }
    node_colors[current_node] = BLACK;
    return false;
}
