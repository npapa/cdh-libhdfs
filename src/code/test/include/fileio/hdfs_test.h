#ifndef __HDFS_TEST_H__
#define __HDFS_TEST_H__

#include "hdfs.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>

#include <gtest/gtest.h>



/** Test fixture for dense array operations. */
class HDFSTestFixture: public testing::Test {
 public:
  /* ********************************* */
  /*             CONSTANTS             */
  /* ********************************* */




  /* ********************************* */
  /*          GTEST FUNCTIONS          */
  /* ********************************* */

  /** Test initialization. */
  virtual int check_hdfs_io(const char* writePath);
  virtual int check_hdfs_write(const char *writeFileName, off_t fileTotalSize, tSize bufferSize);
  virtual int check_hdfs_read(const char *rfile, tSize bufferSize);
  virtual int get_hosts(const char *fileName);
};

#endif
