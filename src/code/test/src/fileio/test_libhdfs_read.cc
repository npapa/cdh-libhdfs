#include <hdfs_test.h>


int HDFSTestFixture::check_hdfs_write(const char *writeFileName, off_t fileTotalSize, tSize bufferSize) {
    hdfsFS fs;
    hdfsFile writeFile;
    char* buffer;
    int i;
    off_t nrRemaining;
    tSize curSize;
    tSize written;


    fs = hdfsConnect("default", 0);
    if (!fs) {
        fprintf(stderr, "Oops! Failed to connect to hdfs!\n");
        exit(-1);
    }

    // sanity check
    if(fileTotalSize == ULONG_MAX && errno == ERANGE) {
      fprintf(stderr, "invalid file size - must be <= %lu\n", ULONG_MAX);
      exit(-3);
    }

    writeFile = hdfsOpenFile(fs, writeFileName, O_WRONLY, bufferSize, 0, 0);
    if (!writeFile) {
        fprintf(stderr, "Failed to open %s for writing!\n", writeFileName);
        exit(-2);
    }

    // data to be written to the file
    buffer = (char *) malloc(sizeof(char) * bufferSize);
    if(buffer == NULL) {
        fprintf(stderr, "Could not allocate buffer of size %d\n", bufferSize);
        return -2;
    }
    for (i=0; i < bufferSize; ++i) {
        buffer[i] = 'a' + (i%26);
    }

    // write to the file
    for (nrRemaining = fileTotalSize; nrRemaining > 0; nrRemaining -= bufferSize ) {
      curSize = ( bufferSize < nrRemaining ) ? bufferSize : (tSize)nrRemaining;
      if ((written = hdfsWrite(fs, writeFile, (void*)buffer, curSize)) != curSize) {
        fprintf(stderr, "ERROR: hdfsWrite returned an error on write: %d\n", written);
        exit(-3);
      }
    }

    free(buffer);
    hdfsCloseFile(fs, writeFile);
    hdfsDisconnect(fs);

    return 0;
}

int HDFSTestFixture::check_hdfs_read(const char *rfile, tSize bufferSize) {
    hdfsFS fs;
    hdfsFile readFile;
    char* buffer;
    tSize curSize;

    fs = hdfsConnect("default", 0);
    if (!fs) {
        fprintf(stderr, "Oops! Failed to connect to hdfs!\n");
        exit(-1);
    }

    readFile = hdfsOpenFile(fs, rfile, O_RDONLY, bufferSize, 0, 0);
    if (!readFile) {
        fprintf(stderr, "Failed to open %s for writing!\n", rfile);
        exit(-2);
    }

    // data to be written to the file
    buffer = (char *) malloc(sizeof(char) * bufferSize);
    if(buffer == NULL) {
        return -2;
    }

    // read from the file
    curSize = bufferSize;
    for (; curSize == bufferSize;) {
        curSize = hdfsRead(fs, readFile, (void*)buffer, curSize);
    }


    free(buffer);
    hdfsCloseFile(fs, readFile);
    hdfsDisconnect(fs);

    return 0;
}

TEST_F(HDFSTestFixture, check_hdfs_read_write) {
   check_hdfs_write("/tmp/testFile_rw",1000,1000000);
   check_hdfs_read("/tmp/testFile_rw",1000);
}

