#ifndef __HDFS_TEST_H__
#define __HDFS_TEST_H__

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
};

#endif
