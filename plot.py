import graphviz
from graphviz import Digraph
import sys
dot = graphviz.Digraph(comment='Tree')
dot


a = (sys.stdin.readline().split())
vertice1 = int(a[0])
vertice2 = int(a[1])


a = (sys.stdin.readline().split())

num_vertice = int(a[0])
num_edges = int(a[1])

for i in range(1, num_edges):
    if (i == vertice1):
        with dot.subgraph(name="1") as c:
            c.node_attr['fillcolor'] = 'red'
            c.node_attr['style'] = 'filled'
            c.node(str("v" + str(i)))
    else:
        if(i == vertice2):
            with dot.subgraph(name="2") as c:
                c.node_attr['color'] = 'blue'
                c.node_attr['style'] = 'filled'
                c.node(str("v" + str(i)))
        else:
            dot.node(str("v" + str(i)))


for i in range(0, num_edges):
    a = (sys.stdin.readline().split())

    dot.edge(str(str("v" + str(a[0]))), str(str("v" + str(a[1]))))


dot.render('doctest-output/round-table', view=True)
