from treelib import Node, Tree

tree = Tree()
with open("./tree.txt") as f:
    input = [[i for i in l.split()] for l in f.read().splitlines()]

for lst in input:
    if lst[1] == '-1':
        tree.create_node(lst[0],lst[0])
    else:
        tree.create_node(lst[0],lst[0], parent=lst[1])

tree.show()
