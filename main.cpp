#include <iostream>
#include <queue>
#include <set>
#include <string.h>

#define NUM_PARENTS 2

typedef struct
{
    int parents[NUM_PARENTS];
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

bool try_insert_array(int *arr, int element, size_t arr_size);

std::set<int> *get_reachable_nodes(int start_node, int node_count, node_t *tree);
void strip_parents(int start_node, std::set<int> *set, node_t *tree);
bool has_loops(int node_count, node_t *tree);
bool dfs_visit_loop_check(color_t *node_colors, node_t *tree, int current_node);

int main()
{
    int node1_i, node2_i, num_vertices, num_edges;
    std::cin >> node1_i >> node2_i >> num_vertices >> num_edges;

    node_t *tree = (node_t *)malloc(num_vertices * sizeof(node_t));

    for (int i = 0; i < num_vertices; ++i)
    {
        for (int j = 0; j < NUM_PARENTS; ++j)
        {
            tree[i].parents[j] = -1;
        }
    }

    for (int i = 0; i < num_edges; ++i)
    {
        int start, stop;
        std::cin >> start >> stop;

        if (!try_insert_array(tree[stop - 1].parents, start - 1, NUM_PARENTS))
        {
            // a node cannot have more than 2 parents, invalid tree
            std::cout << 0 << std::endl;
            return 0;
        }
    }

    if (has_loops(num_vertices, tree))
    {
        std::cout << 0 << std::endl;
        return 0;
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

std::set<int> *get_reachable_nodes(int start_node, int node_count, node_t *tree)
{
    bfs_node *nodes = (bfs_node *)malloc(node_count * sizeof(bfs_node));

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
            if (parent == -1)
                break;
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
        if (parent == -1)
            break;
        auto set_it = set->find(parent);
        if (set_it != set->end())
        {
            set->erase(set_it);
        }
        strip_parents(parent, set, tree);
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
