#include <hdfs_test.h>
#include "hdfs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int HDFSTestFixture::check_hdfs_io(const char* writePath) {
  hdfsFS fs = hdfsConnect("default", 0);
  printf("OK\n");
  hdfsFile writeFile = hdfsOpenFile(fs, writePath, O_WRONLY|O_CREAT, 0, 0, 0);
  if(!writeFile) {
    fprintf(stderr, "Failed to open %s for writing!\n", writePath);
    exit(-1);
  }
  const char* buffer = "Hello, World!!\n";
  tSize num_written_bytes = hdfsWrite(fs, writeFile, (void*)buffer, strlen(buffer)+1);
  if (hdfsFlush(fs, writeFile)) {
    fprintf(stderr, "Failed to 'flush' %s\n", writePath);
    exit(-1);
  }
  hdfsCloseFile(fs, writeFile);
}

TEST_F(HDFSTestFixture, check_hdfs_io) {
   char *argv;
   check_hdfs_io("/tmp/testFile");
}
