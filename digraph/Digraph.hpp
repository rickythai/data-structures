// Digraph.hpp


#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <queue>



// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}



// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:
    // setAllFalse() sets all of the vertecies visit status to false initially
    void setAllFalse(std::map<int, bool>& visit) const;

    // traverse() is the algorithm that traverses through the graph
    void traverse(std::map<int, bool>& visit, int vertex) const;

    // setUp() sets up the info needed to run Dijkstra's algorithm
    void setUp(std::map<int, bool>& kv, std::map<int, int>& dv, 
               std::map<int, int>& pv, int startVertex) const;

    // analyzePaths() runs Dijkstra's algoritm to find the shortest paths
    void analyzePaths(std::map<int, bool>& kv, std::map<int, int>& dv, 
        std::map<int, int>& pv, std::priority_queue<std::pair<int,int>>& pq,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;
    
    // map is a private member variable that represents the graph
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> map;
};



template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
{
    map = d.map;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
{
    map = d.map;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    map = d.map;
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
    map = d.map;
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
    std::vector<int> vertex_nums;

    for (auto m : map)
    {
        vertex_nums.push_back(m.first);
    }

    return vertex_nums;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    std::vector<std::pair<int,int>> edge_pairs;

    for (auto m : map) // for each pair in map
    {
        for (auto e : m.second.edges) // for each edge in the list of edges
        {
            edge_pairs.push_back(std::pair<int,int>(e.fromVertex, e.toVertex));
        }
    }

    return edge_pairs;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
    std::vector<std::pair<int,int>> edge_pairs;

    if (map.find(vertex) == map.end())
    {
        throw DigraphException("Vertex does not exist");
    }

    for (auto e : map.at(vertex).edges) // for each edge in list of edges
    {
        edge_pairs.push_back(std::pair<int,int>(e.fromVertex, e.toVertex));
    }
       
    return edge_pairs;
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    if (map.find(vertex) == map.end())
    {
        throw DigraphException("Vertex does not exist");
    }

    return map.at(vertex).vinfo;
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
    if (map.find(fromVertex) == map.end() || map.find(toVertex) == map.end())
    {
        throw DigraphException("One of the vertices does not exist");
    }

    for (auto e : map.at(fromVertex).edges)
    {
        if (e.fromVertex == fromVertex && e.toVertex == toVertex)
        {
             return e.einfo;
        }
    }

    throw DigraphException("Edge does not exist");
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    if (map.find(vertex) != map.end())
    {
        throw DigraphException("Vertex is already in the graph");
    }

    DigraphVertex<VertexInfo, EdgeInfo> vobject;
    vobject.vinfo = vinfo;

    std::pair<int, DigraphVertex<VertexInfo, EdgeInfo>> newVertex;
    newVertex.first = vertex;
    newVertex.second = vobject;

    map.insert(newVertex);
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
    if (map.find(fromVertex) == map.end() || map.find(toVertex) == map.end())
    {
        throw DigraphException("One of the verticies does not exist");
    }

    std::pair<int,int> tempEdge(fromVertex, toVertex);
    std::vector<std::pair<int,int>> e = edges(fromVertex);

    if (std::find(e.begin(), e.end(), tempEdge) != e.end())
    {
        throw DigraphException("Edge already exists");
    }

    DigraphEdge<EdgeInfo> newEdge{fromVertex, toVertex, einfo};
    map.at(fromVertex).edges.push_back(newEdge);
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    if (map.find(vertex) == map.end())
    {
        throw DigraphException("Vertex does not exist");
    }

    for (auto m : map)
    {
        for (auto e = m.second.edges.begin(); e != m.second.edges.end(); e++)
        {
            if (e->toVertex == vertex)
            {
                m.second.edges.erase(e);
            }
        }
    }

    map.erase(vertex);
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    if (map.find(fromVertex) == map.end() || map.find(toVertex) == map.end())
    {
        throw DigraphException("One of the vertices does not exist");
    }

    std::pair<int, int> tempEdge(fromVertex, toVertex);
    std::vector<std::pair<int, int>> tempEdges= edges(fromVertex);

    if (std::find(tempEdges.begin(), tempEdges.end(), tempEdge) == 
        tempEdges.end())
    {
        throw DigraphException("Edge does not exist");
    }

    for (auto e =  map.at(fromVertex).edges.begin(); 
         e != map.at(fromVertex).edges.end(); e++)
    {
        if (e->toVertex == toVertex)
        {
            map.at(fromVertex).edges.erase(e);
        }
    }
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return map.size();
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    int counter = 0;

    for (auto m : map)
    {
        counter += m.second.edges.size();
    }

    return counter;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    return map.at(vertex).edges.size();
}


template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    std::map<int, bool> visit;

    for (auto m : map)
    {
        setAllFalse(visit);
        traverse(visit, m.first);
        
        for (auto v : visit)
        {
            if (v.second == false)
            {
                return false;
            }
        }
    }

    return true;
}


template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{ 
    std::map<int, bool> kv;
    std::map<int, int> dv;
    std::map<int, int> pv;

    setUp(kv, dv, pv, startVertex);

    std::priority_queue<std::pair<int,int>> pq;
    std::pair<int,int> vorder(0, startVertex);
    pq.push(vorder);

    analyzePaths(kv, dv, pv, pq, edgeWeightFunc);
    
    return pv;
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::analyzePaths(
    std::map<int, bool>& kv, std::map<int, int>& dv, std::map<int, int>& pv,
    std::priority_queue<std::pair<int,int>>& pq,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
    while (!pq.empty())
    {
        int v = pq.top().second;
        pq.pop();

        if (kv[v] == false)
        {
            kv[v] = true;
        }
        
        for (auto e : map.at(v).edges)
        {
            if (dv.at(e.toVertex)>dv[v]+edgeWeightFunc(edgeInfo(v, e.toVertex)))
            {
                dv[e.toVertex] = dv[v]+edgeWeightFunc(edgeInfo(v, e.toVertex));
                pv[e.toVertex] = v;

                std::pair<int,int> newv(dv[e.toVertex], e.toVertex);
                pq.push(newv);
            }
        }
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::setAllFalse(std::map<int, bool>& visit)const
{
    for (auto m : map)
    {
        visit[m.first] = false;
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::traverse(std::map<int, bool>& visit, 
    int vertex) const
{
    visit[vertex] = true;

    for (auto e : this->edges())
    {
        if (visit.at(e.second) != true)
        {
            traverse(visit, e.second);
        }
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::setUp(std::map<int, bool>& kv, 
    std::map<int, int>& dv, std::map<int, int>& pv, int startVertex) const
{
    for (auto m : map)
    {
        if (m.first == startVertex)
        {
            kv[m.first] = false;
            dv[m.first] = 0;
            pv[m.first] = m.first;
        }
        else
        {
            kv[m.first] = false;
            dv[m.first] = 999;
            pv[m.first] = m.first;
        }
    }
}



#endif // DIGRAPH_HPP

