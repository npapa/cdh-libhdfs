# libhdfs with Docker

Docker image for single libdfs node.

**Only for development purpose.**

License: **MIT**

## Local build
```sh
$ sudo docker build -t cdh-libhdfs .
```

## Running HDFS container

```sh
# Running container
$ sudo docker run --hostname=quickstart.cloudera --privileged=true -ti cdh-libhdfs
```
