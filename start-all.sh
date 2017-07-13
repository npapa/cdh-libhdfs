#!/bin/bash
source /opt/rh/devtoolset-2/enable 
DAEMONS="zookeeper-server \
        hadoop-hdfs-datanode \
        hadoop-hdfs-journalnode \
        hadoop-hdfs-namenode \
        hadoop-hdfs-secondarynamenode \
        hadoop-httpfs"

for daemon in ${DAEMONS}; do
    sudo service ${daemon} start
done

cd /opt/TileDB/build/
gcc --version
cmake ..
export JAVA_HOME="/usr/java/jdk1.7.0_67-cloudera"
export HADOOP_LIB="/usr/lib64/:/usr/lib/hadoop/lib/native/"
export LD_LIBRARY_PATH="$HADOOP_LIB:$JAVA_HOME/jre/lib/amd64/server/"
export CLASSPATH=`hadoop classpath --glob`

make check -j 4

exec bash
