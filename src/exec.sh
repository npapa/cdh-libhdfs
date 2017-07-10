export JAVA_HOME="/usr/java/jdk1.7.0_67-cloudera"
export HADOOP_LIB="/usr/lib64/"
export LD_LIBRARY_PATH="$HADOOP_LIB:$JAVA_HOME/jre/lib/amd64/server/"

export CLASSPATH=`hadoop classpath --glob`

./$1
