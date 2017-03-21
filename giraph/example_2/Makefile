all: myjar.jar

ComputePR.class: ComputePR.java
	javac -cp /data/grpNT01s/giraph/giraph-examples/target/giraph-examples-1.1.0-SNAPSHOT-for-hadoop-2.4.1-jar-with-dependencies.jar:$(shell ${HADOOP_HOME}/bin/hadoop classpath) ComputePR.java

myjar.jar: ComputePR.class
	cp /data/grpNT01s/giraph/giraph-examples/target/giraph-examples-1.1.0-SNAPSHOT-for-hadoop-2.4.1-jar-with-dependencies.jar ./myjar.jar
	jar uf myjar.jar ComputePR*class


tidy:
	rm -f *class

clean: tidy
	rm -f *.jar
