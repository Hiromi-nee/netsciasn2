import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.log4j.Logger;
import org.apache.giraph.graph.BasicComputation;
import org.apache.giraph.graph.Vertex;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.WritableComparable;
import org.apache.giraph.conf.LongConfOption;
import org.apache.giraph.edge.Edge;
import java.io.IOException;

//compute PR. PREGEL
public class ComputePR extends BasicComputation<
        LongWritable, DoubleWritable, FloatWritable, DoubleWritable> {
    public static final int SUPERSTEPS_MAX = 30;
    
    @Override
    public void compute(Vertex<LongWritable, DoubleWritable, FloatWritable> vertex,
                        Iterable<DoubleWritable> messages) throws IOException {
    if (getSuperstep() >= 1) {
      double sum = 0;
      for (DoubleWritable message : messages) {
        sum += message.get();
      }
      vertex.getValue().set((0.15f / getTotalNumVertices()) + 0.85f * sum);
    }
    if (getSuperstep() < SUPERSTEPS_MAX) {
      sendMessageToAllEdges(vertex,
          new DoubleWritable(vertex.getValue().get() / vertex.getNumEdges()));
    } else {
      vertex.voteToHalt();
    }
  }
}
