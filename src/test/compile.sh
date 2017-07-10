export JAVA_HOME="/usr/java/jdk1.7.0_67-cloudera"
export HADOOP_INCLUDE="/usr/include/"
export HADOOP_LIB="/usr/lib64/"
export CXXFLAGS="-O2 -g -Wall -I$HADOOP_INCLUDE -I$JAVA_HOME/include/ -I$JAVA_HOME/include/linux/"
export LDFLAGS="-L$HADOOP_LIB -lhdfs -L$JAVA_HOME/jre/lib/amd64/server/ -ljvm"
export LD_LIBRARY_PATH="$HADOOP_LIB:$JAVA_HOME/jre/lib/amd64/server/"

gcc $1.c -o $1 $CXXFLAGS $LDFLAGS
