#include <iostream>
using namespace std;

struct Vertex
{
  bool explored;     // true if edge has been explored in bfs
  int in_edge;       // edge leading in to vertex in bfs, allows us to get parent
  int depth;         // depth of vertex in bfs tree
  int num_neighbors; // how many edges coming out 
  int *neighbors;    // list of all edges coming out
  int count;         // neighbors read in so far, only used in reading in
};

struct Edge
{
  int begin;     // incoming vertex
  int end;       // outgoing vertes
};

struct Queue
{
  int *elements; // where the queue elements are stored
  int start;     // first index position in queue
  int stop;      // one past last index position in queue
};

// Breadth first search
bool bfs(Vertex *V, Edge *E, int num_vertices, int num_edges);

// Print path from s to t gotten from bfs
void print_path(Vertex *V, Edge *E, int s, int t);

// Print an edge given edge id
void print_edge(Edge *E, int eid);

// Print all edges in graph
void print_all_edges(Edge *E, int num_edges);

// Print edges in bfs tree
void print_bfs_tree(Edge *E, Vertex *V, int num_vertices);

// Read in value in between bounds
int read_in_between(int low, int high);

// Read in nonnegative number
int read_nonnegative();

// initialize a queue
void initialize_queue(Queue &Q, int size);

// add an elemement to a queue
void add_to_queue(Queue &Q, int n);

// take an element from a queue
int take_from_queue(Queue &Q);

// true if queue is empty
bool empty(Queue Q);

int main()
{
  int num_vertices;     // how many vertices in graph
  Vertex *V;            // list of vertices in graph
  Edge *E;              // list of edges in graph
  int num_edges;        // how many edges in graph
  int u,v;              // vertices used in reading in

  cout << "How many vertices? ";
  num_vertices = read_nonnegative();

  cout << "How many edges ";
  num_edges = read_in_between(0,(num_vertices*(num_vertices-1))/2);
  num_edges = 2 * num_edges;  // edges go in both directions

  // Vertex ids will start at 0
  // Edge ids will start at 1
  V = new Vertex[num_vertices];
  E = new Edge[num_edges+1];

  for (int vid=0; vid<num_vertices; vid++)
    {
      V[vid].num_neighbors = 0;
      V[vid].count = 0;
    }

  cout << "List all the edges" << endl;

  // Read in edges and calculuate how many neighbors aeach vertex has
  // Need to account for edgss in both directions
  for (int eid=1; eid<=num_edges; eid++)
    {
      E[eid].begin = read_in_between(0,num_vertices-1);
      E[eid].end = read_in_between(0,num_vertices-1);
      E[eid+1].begin = E[eid].end;
      E[eid+1].end = E[eid].begin;
      u = E[eid].begin;
      V[u].num_neighbors++;
      v = E[eid].end;
      V[v].num_neighbors++;
      eid++;
    }

  // Now I know how many neighbors each vertex has
  for (int vid=0; vid<num_vertices; vid++)
    {
      V[vid].neighbors = new int[V[vid].num_neighbors];
    }

  // Add edges to vertex neighbor list
  for (int eid=1; eid<=num_edges; eid++)
    {
      u = E[eid].begin;
      V[u].neighbors[V[u].count] = eid;
      V[u].count++;
    }

  print_all_edges(E,num_edges);
      		    
  bool cycle = bfs(V, E, num_vertices, num_edges);

  if (!cycle)
    {
      cout << "There is no cycle" << endl;
    }

  return 0;
}

bool bfs(Vertex *V, Edge *E, int num_vertices, int num_edges)
{
  Queue Q;         // queue of edges in graph
  int u,v;        // nodes used in algorithm
  int eid;        // to store an edge taken from queue
  int e;          // edge used in algorithms
  int s = 0;      // start search at node 0

  // Create queue to store edges
  initialize_queue(Q,num_edges);

  // Set all edges as unexplored
  for (int vid=0; vid<num_vertices; vid++)
    {
      V[vid].explored = false;
    }

  // Put 0 in queue indicating no edge into root
  add_to_queue(Q,0);

  // while Q is not empty
  while(!empty(Q))
    {
      // take an edge (u,v) from Q
      eid = take_from_queue(Q);
      if (eid == 0) // if root
	{
	  u = -1;
	  v = s;
	}
      else // not root
	{
	  u = E[eid].begin;
	  v = E[eid].end;
	}

      if (!V[v].explored) // first time here
	{
	  V[v].explored = true;
	  V[v].in_edge = eid;
	  if (eid == 0) // if root
	    {
	      V[v].depth = 0;
	    }
	  else // not root
	    {
	      V[v].depth = V[u].depth + 1;
	    }
	  // for all neigbors w of v
	  for (int j=0; j<V[v].num_neighbors; j++)
	    {
	      // add (v,w) to Q if w is not already parent of v
	      e = V[v].neighbors[j];
	      if (E[e].end != u)
		{
		  add_to_queue(Q,e);
		}
	    }
	}
      else // explored already
	{
	  cout<<"There is a cycle using the following edges: \n";
	  if (V[u].depth == V[v].depth) // same level
	    {
		print_edge(E,eid);
		do{
			print_edge(E, V[u].in_edge);
			print_edge(E, V[v].in_edge);
			u = E[V[u].in_edge].begin;
			v = E[V[v].in_edge].begin;
		}while(u != v); 
	    }
	  else // adjacent levels
	    {
		print_edge(E, eid);
		print_edge(E, V[v].in_edge);
		v = E[V[v].in_edge].begin;	
		do{
			print_edge(E, V[u].in_edge);
			print_edge(E, V[v].in_edge);
			u = E[V[u].in_edge].begin;
			v = E[V[v].in_edge].begin;
		}while(u != v); 
	    }
	  cout << '\n';
	  return true;
	}
    }

  print_bfs_tree(E,V,num_vertices);

  return false;

}

void print_path(Vertex *V, Edge *E, int s, int t)
{

  int v; // store next vertex up the tree

  if (s != t) // if not at root
    {
      v = E[V[t].in_edge].begin; // get parent vertex
      print_path(V,E,s,v);
      cout << t << " ";
    }
  else // at root of tree
    {
      cout << s << " ";
    }
}

void print_edge(Edge *E, int eid)
{
  cout << "(" << E[eid].begin << "," << E[eid].end << ") ";
}

void print_all_edges(Edge *E, int num_edges)
{
  cout << "Here are your edges again" << endl;
  for (int eid=1; eid<=num_edges; eid++)
    {
      print_edge(E,eid);
    }
  cout << endl;
}

void print_bfs_tree(Edge *E, Vertex *V, int num_vertices)
{
  cout << "Here are the edges in the bfs tree:" << endl;
  for (int vid=0; vid<num_vertices; vid++)
    {
      if(V[vid].in_edge > 0 && V[vid].explored) // if real edge \
and not explored
  {
    print_edge(E,V[vid].in_edge);
  }
    }
  cout << endl;
}

int read_in_between(int low, int high)
{
  int n;
  cin >> n;
  while (low > n || n > high)
    {
      cout << "Illegal input, enter that one again: ";
      cin >> n;
    }
  return n;
}

int read_nonnegative()
{
  int n;
  cin >> n;
  while (n < 0)
    {
      cout << "Illegal input, enter that one again: ";
      cin >> n;
    }
  return n;
}

void initialize_queue(Queue &Q, int size)
{
  Q.elements = new int[size];
  Q.start = 0;
  Q.stop = 0;
}

void add_to_queue(Queue &Q, int n)
{
  Q.elements[Q.stop] = n;
  Q.stop++;
}

int take_from_queue(Queue &Q)
{
  int n = Q.elements[Q.start];
  Q.start++;
  return n;
}

bool empty(Queue Q)
{
  return Q.start >= Q.stop;
}

