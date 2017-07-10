FROM cloudera/quickstart:latest
MAINTAINER npapa

RUN yum install -y cmake wget zlib-devel openssl-devel epel-release lz4-devel
RUN yum install -y openmpi-devel

RUN yum install -y git

WORKDIR /opt
RUN wget https://github.com/facebook/zstd/archive/v1.1.0.tar.gz
RUN tar xf v1.1.0.tar.gz
WORKDIR /opt/zstd-1.1.0
RUN sudo make install PREFIX='/usr'

WORKDIR /opt
RUN wget https://github.com/google/googletest/archive/release-1.7.0.tar.gz
RUN tar xf release-1.7.0.tar.gz
WORKDIR /opt/googletest-release-1.7.0
RUN cmake .
RUN make
RUN mv include/gtest /usr/include/gtest
RUN mv libgtest_main.a libgtest.a /usr/lib/

RUN yum install -y doxygen

WORKDIR /opt
RUN git clone https://github.com/Blosc/c-blosc
WORKDIR /opt/c-blosc
RUN mkdir build
WORKDIR /opt/c-blosc/build
RUN cmake -DCMAKE_INSTALL_PREFIX=/usr/local/ ..
RUN cmake --build . --target install


RUN mkdir -p /opt/libhdfs_test
COPY src/* /opt/libhdfs_test/
RUN mkdir -p /opt/TileDB
COPY TileDB /opt/TileDB/

WORKDIR /opt
RUN wget -O /etc/yum.repos.d/slc6-devtoolset.repo http://linuxsoft.cern.ch/cern/devtoolset/slc6-devtoolset.repo
RUN wget -O /etc/pki/rpm-gpg/RPM-GPG-KEY-cern http://linuxsoft.cern.ch/cern/slc68/x86_64/RPM-GPG-KEY-cern

RUN yum install devtoolset-2 -y
RUN scl enable devtoolset-2 bash
RUN gcc --version

CMD /usr/bin/docker-quickstart
