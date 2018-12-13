/*  unionsets.cpp */

/* 
    Union-Find of Disjoint Sets
    By Euxhen Hasanaj
    04.03.2018
*/

//  Given  a  number of nodes and edges  between them
//  construct a forest where the connected components
//  correspond to disjoint sets.

#include <vector>

using namespace std;

/*
    The node represents an element of the  set  with
    its value specified as  an  integer in val.  The
    node contains  a reference to its parent  and  a
    size which refers to the size of  the  component
    containing the node. The size is  only  relevant
    in the context of the root of the component.  It
    will still be present in any other node, but its
    value will be ignored.
*/
struct node {
    int parent;
    int val;
    int sz = 1; // The node itself has size of 1

    //Constructors
    node() {}

    node(int p, int v) {
        parent = p;
        val = v;
    }
};

/*
    We construct a graph class which will handle the
    union of the sets. V corresponds to the  maximum
    number of elements in the  tree.   The  tree  is
    represented as an array of nodes, indexed at  0.
*/
class Graph {
private:
    int V;
    node* tree;
public:
    Graph(int V);
    int find_parent(int u);
    void add_edge(int u, int v);
    vector<vector<int>> get_sets();
};

// Constructor
Graph::Graph(int v) {
    V = v;
    tree = new node[V]; // Allocate memory for the tree
    
    // Initialize every node in the tree
    for (int i = 0; i < V; ++i) {
        node myNode(i, i); // Parent is the same as the node value
        tree[i] = myNode;
    }
}

/*
    find_parent(int u) returns the parent of u which
    is simply the node that we have chosen to  be  a
    representative of the set that contains u.  This
    parent node's parent will have a value equal  to
    the parent node itself.

    The time complexity for this search is O(log(n))
    which is achieved by the way we union  two sets,
    where we attach the set with a smaller  size  to
    the other one. In order to further  improve  the
    time complexity of the algorithm, we make use of
    path compression. When finding the parent p of a
    node v, set the parent of all the other nodes in
    the path from v to p to be equal to p. This  way
    we achieve a shorter but wider tree, and  parent
    search is found much faster in  amortized  time.
*/
int Graph::find_parent(int u) {
    if (tree[u].parent != u)
        tree[u].parent = find_parent(tree[u].parent);
    return tree[u].parent;
}

/*
    add_edge(int u, int v) unions the two sets that
    contain u and v. If u and v are already present
    in the same set, then  do  nothing.  Otherwise,
    consider the set with the smaller size and add
    a reference to its root element to point to the
    other  root,  while  increasing  the  set  size
    appropriately.

    Time complexity is O(1), assuming that searching
    for the parent is done in constant time.
*/
void Graph::add_edge(int u, int v) {
    int p1 = find_parent(u);
    int p2 = find_parent(v);

    if (p1 == p2)
        return;

    if (tree[p1].sz > tree[p2].sz) {
        tree[p2].parent = tree[p1].val; // Change parent reference
        tree[p1].sz += tree[p2].sz;
    } else {
        tree[p1].parent = tree[p2].val; // Change parent reference
        tree[p2].sz += tree[p1].sz;
    }
}

/*
    get_sets()  returns  a vector of vectors which
    represent the disjoint sets obtained after the
    union. We attempt to make the  process  faster
    by avoiding push_backs and allocating all  the
    required memory beforehand. For  this  purpose
    we maintain a setNumber  integer  which  keeps
    count of the final number of sets.  We use two
    vectors when reading the sets. The  first  one
    is needed to group the elements into sets, and
    the second one gets rid of the empty  sets  by
    iterating through the sets in the first vector
    and saving only the nonzero sets.
*/
vector<vector<int>> Graph::get_sets() {
    int setNumber = 0; // Set count
    // Initialize a vector of size V
    vector<vector<int>> setsBuff(V, vector<int>(0, 0));
    // Check all elements and place  it  in  the
    // appropriate set based on its parent value
    for (int i = 0; i < V; ++i) {
        int parent = tree[i].parent;
        if (setsBuff[parent].size() == 0) // Parent non-seen before
            ++setNumber;
        setsBuff[parent].push_back(i); // Tradeoff between memory and speed
    }

    // Initialize our second vector with size setNumber
    vector<vector<int>> sets(setNumber, vector<int>(0, 0));
    setNumber = 0; // Position counter
    for (int i = 0; i < V; ++i) {
        if (setsBuff[i].size() != 0) { // Take only nonzero sets
            sets[setNumber] = setsBuff[i];
            setNumber++;
        }
    }
    return sets;
}
