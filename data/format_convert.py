from __future__ import print_function
import pprint
import sys


#src_vertex src_vertex_val vertex_2 edge_2 vertex_3 edge_3
def format_giraph(unf_net):
    edge_val = 0
    vertex_val = 0
    out_line = ""
    for k in unf_net.keys():
        edges = unf_net[k]
        out_line = out_line+ str(k)+" "+str(vertex_val)
        for edge in edges:
            out_line = out_line + " " + str(edge) + " " + str(edge_val)
        out_line = out_line + "\n"
    return(out_line)


#[vertex ID]  [number of target vertices] [target ID 1] [target ID 2] [target ID 3]
def format_glab(unf_net):
    out_line = ""
    for k in unf_net.keys():
        edges = unf_net[k]
        edges_size = len(edges)
        out_line = out_line + str(k) + " " + str(edges_size)
        for edge in edges:
            out_line = out_line + " " + str(edge)
        out_line = out_line + "\n"
    return out_line

def read_net(fname):
    list_net = {}
    with open(fname) as f:
        for line in f:
            tline = line.rstrip().split(" ")
            if list_net.has_key(tline[0]):
                list_net[tline[0]].extend(tline[1:])
            else:
                list_net[tline[0]] = tline[1:]
    return list_net

def write_net(f_net, fname):
    f = open(fname, 'w')
    f.write(f_net)
    f.close()

def main(argv):
    in_file = ""
    if argv[1]:
        in_file = argv[1]
    else:
        print("Specify input file.")
    unf_net = read_net(in_file)
    #pp = pprint.PrettyPrinter(width=41)
    #pp.pprint(unf_net)

    f1_net = format_giraph(unf_net)
    f2_net = format_glab(unf_net)
    print("Writing Giraph format.")
    write_net(f1_net, "erdos_giraph.net")
    print("Writing Graphlab format.")
    write_net(f2_net, "erdos_graphlab.net")
    
if __name__ == '__main__':
    main(sys.argv)

