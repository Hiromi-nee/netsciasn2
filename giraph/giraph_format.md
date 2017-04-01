public abstract class AdjacencyListTextVertexInputFormat<I extends org.apache.hadoop.io.WritableComparable,V extends org.apache.hadoop.io.Writable,E extends org.apache.hadoop.io.Writable>
extends TextVertexInputFormat<I,V,E>


VertexReader that readers lines of text with vertices encoded as adjacency lists and converts each token to the correct type.

For example, a graph with vertices as integers and values as doubles could be encoded as: 1 0.1 2 0.2 3 0.3 to represent a vertex named 1, with 0.1 as its value and two edges, to vertices 2 and 3, with edge values of 0.2 and 0.3, respectively.

src_vertex src_vertex_val vertex_2 edge_2 vertex_3 edge_3

<hr>


 VertexReader that features <code>double</code> vertex
 values and <code>float</code> out-edge weights. The
 files should be in the following JSON format:
 JSONArray(<vertex id>, <vertex value>,
 JSONArray(JSONArray(<dest vertex id>, <edge value>), ...))
 Here is an example with vertex id 1, vertex value 4.3, and two edges.
 First edge has a destination vertex 2, edge value 2.1.
 Second edge has a destination vertex 3, edge value 0.7.
 [1,4.3,[[2,2.1],[3,0.7]]]
