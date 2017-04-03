all: myjar.jar

concomp.class: concomp.java
	javac -cp /data/grpNT01s/giraph/giraph-examples/target/giraph-examples-1.1.0-SNAPSHOT-for-hadoop-2.4.1-jar-with-dependencies.jar:$(shell ${HADOOP_HOME}/bin/hadoop classpath) concomp.java

myjar.jar: concomp.class
	cp /data/grpNT01s/giraph/giraph-examples/target/giraph-examples-1.1.0-SNAPSHOT-for-hadoop-2.4.1-jar-with-dependencies.jar ./myjar.jar
	jar uf myjar.jar concomp*class


tidy:
	rm -f *class

clean: tidy
	rm -f *.jar
