
import org.apache.giraph.graph.BasicComputation;
import org.apache.giraph.edge.Edge;
import org.apache.giraph.graph.Vertex;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.giraph.Algorithm;

import java.io.IOException;


//with Giraph

public class ConnectedComponentsVertex extends
        BasicComputation<IntWritable, IntWritable, NullWritable, IntWritable> {

    /**
     *
     * @param vertex Vertex
     * @param messages Iterator of messages from the previous superstep.
     * @throws IOException
     */
    @Override
    public void compute(
            Vertex<IntWritable, IntWritable, NullWritable> vertex,
            Iterable<IntWritable> messages) throws IOException {
        int currentComponent = vertex.getValue().get();

        if (getSuperstep() == 0) {
            for (Edge<IntWritable, NullWritable> edge : vertex.getEdges()) {
                int neighbor = edge.getTargetVertexId().get();
                if (neighbor < currentComponent) {
                    currentComponent = neighbor;
                }
            }
            // Only need to send value if it is not the own id
            if (currentComponent != vertex.getValue().get()) {
                vertex.setValue(new IntWritable(currentComponent));
                for (Edge<IntWritable, NullWritable> edge : vertex.getEdges()) {
                    IntWritable neighbor = edge.getTargetVertexId();
                    if (neighbor.get() > currentComponent) {
                        sendMessage(neighbor, vertex.getValue());
                    }
                }
            }

            vertex.voteToHalt();
            return;
        }

        boolean changed = false;
        //  a smaller id ?
        for (IntWritable message : messages) {
            int candidateComponent = message.get();
            if (candidateComponent < currentComponent) {
                currentComponent = candidateComponent;
                changed = true;
            }
        }

        // propagate new component id to the neighbors
        if (changed) {
            vertex.setValue(new IntWritable(currentComponent));
            sendMessageToAllEdges(vertex, vertex.getValue());
        }
        vertex.voteToHalt();
    }
}