#ifndef __HDFS_POSIX_TEST_H__
#define __HDFS_POSIX_TEST_H__

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <gtest/gtest.h>



class POSIXTestFixture: public testing::Test {
 public:
  /* ********************************* */
  /*             CONSTANTS             */
  /* ********************************* */




  /* ********************************* */
  /*          GTEST FUNCTIONS          */
  /* ********************************* */

  /** Test initialization. */
  virtual int check_posix_write(const char *writeFileName, ssize_t fileTotalSize, ssize_t bufferSize);
  virtual int check_posix_read(const char *rfile, ssize_t bufferSize);
};

#endif
