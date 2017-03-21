#include <vector>
#include <string>
#include <fstream>
#include <graphlab.hpp>


float RESET_PROB = 0.15;
float TOLERANCE = 1.0E-2;
// vertex data is pagerank value
typedef float vertex_data_type;
// no edge data
typedef graphlab::empty edge_data_type;
// graph type will be determined by the vertex and edge data types
typedef graphlab::distributed_graph<vertex_data_type, edge_data_type> graph_type;
// to init vertices
void init_vertex(graph_type::vertex_type& vertex) { vertex.data() = 1; }

//calc pagerank
class pagerank :
  public graphlab::ivertex_program<graph_type, float>,
  public graphlab::IS_POD_TYPE {
  float last_change;
public:
  /* Gather weighted rank of the adjacent page   */
  float gather(icontext_type& context, const vertex_type& vertex,
               edge_type& edge) const {
    return ((1.0 - RESET_PROB) / edge.source().num_out_edges()) *
      edge.source().data();
  }

  /* Update vertex rank with sum of adjacent vertices rank */
  void apply(icontext_type& context, vertex_type& vertex,
             const gather_type& total) {
    const double newval = total + RESET_PROB;
    last_change = std::fabs(newval - vertex.data());
    vertex.data() = newval;
  }

  /* The scatter edges depend on whether the pagerank has converged */
  edge_dir_type scatter_edges(icontext_type& context,
                              const vertex_type& vertex) const {
    if (last_change > TOLERANCE) return graphlab::OUT_EDGES;
    else return graphlab::NO_EDGES;
  }

  //signal adjacent vertices
  void scatter(icontext_type& context, const vertex_type& vertex,
               edge_type& edge) const {
    context.signal(edge.target());
  }
}; // pagerank calc


//save results
struct pagerank_writer {
  std::string save_vertex(graph_type::vertex_type v) {
    std::stringstream strm;
    strm << v.id() << "\t" << v.data() << "\n";
    return strm.str();
  }
  std::string save_edge(graph_type::edge_type e) { return ""; }
}; 


//main func
int main(int argc, char** argv) {
  // mpi
  graphlab::mpi_tools::init(argc, argv);
  graphlab::distributed_control dc;
  global_logger().set_log_level(LOG_INFO);

  // Command line opts
  graphlab::command_line_options clopts("PageRank");
  std::string graph_dir;
  std::string format = "adj"; //default graph format adjacency list
  std::string exec_type = "synchronous"; //synchronous execution
  clopts.attach_option("graph", graph_dir,
                       "The graph file");
  clopts.add_positional("graph");
  clopts.attach_option("format", format,
                       "graph file format");
  clopts.attach_option("engine", exec_type, 
                       "Synchronous or asynchronous");
  clopts.attach_option("tol", TOLERANCE,
                       "The permissible change at convergence.");
  std::string saveprefix;
  clopts.attach_option("saveprefix", saveprefix,
                       "prefix of outfile");

  if(!clopts.parse(argc, argv)) {
    dc.cout() << "Error in parsing command line arguments." << std::endl;
    return EXIT_FAILURE;
  }

  if (graph_dir == "") {
    dc.cout() << "Graph not specified.";
    return EXIT_FAILURE;
  }

  // Gen graph obj
  graph_type graph(dc, clopts);
  dc.cout() << "Loading graph of format: "<< format << std::endl;
  graph.load_format(graph_dir, format);
  // must call finalize before querying the graph
  graph.finalize();
  dc.cout() << "#vertices: " << graph.num_vertices()
            << " #edges:" << graph.num_edges() << std::endl;

  // Init vertex data
  graph.transform_vertices(init_vertex);

  //calc pagerank
  graphlab::omni_engine<pagerank> engine(dc, graph, exec_type, clopts);
  engine.signal_all();
  engine.start();
  const float runtime = engine.elapsed_seconds();
  dc.cout() << "Finished Running engine in " << runtime
            << " seconds." << std::endl;

  //save graph
  if (saveprefix != "") {
    graph.save(saveprefix, pagerank_writer(),
               false,    // do not gzip
               true,     // save vertices
               false);   // do not save edges
  }

  //tie up lose ends
  graphlab::mpi_tools::finalize();
  return EXIT_SUCCESS;
} //main end
