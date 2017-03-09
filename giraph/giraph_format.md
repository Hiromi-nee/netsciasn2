public abstract class AdjacencyListTextVertexInputFormat<I extends org.apache.hadoop.io.WritableComparable,V extends org.apache.hadoop.io.Writable,E extends org.apache.hadoop.io.Writable>
extends TextVertexInputFormat<I,V,E>


VertexReader that readers lines of text with vertices encoded as adjacency lists and converts each token to the correct type.

For example, a graph with vertices as integers and values as doubles could be encoded as: 1 0.1 2 0.2 3 0.3 to represent a vertex named 1, with 0.1 as its value and two edges, to vertices 2 and 3, with edge values of 0.2 and 0.3, respectively.

src_vertex src_vertex_val vertex_2 edge_2 vertex_3 edge_3