#include <hdfs_test.h>

static struct timeval tm1;
static off_t totalSize;

static inline void start()
{
    gettimeofday(&tm1, NULL);
}

static inline void stop()
{
    struct timeval tm2;
    gettimeofday(&tm2, NULL);

    unsigned long long t = 1000000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec);
    printf("Time: %llu us, MB/s:%f\n", t, (double)totalSize/(double)t);
}


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

    printf("Test write total bytes: %d with buffer size: %d\n", fileTotalSize, bufferSize);
    start();    
    // write to the file
    for (nrRemaining = fileTotalSize; nrRemaining > 0; nrRemaining -= bufferSize ) {
      curSize = ( bufferSize < nrRemaining ) ? bufferSize : (tSize)nrRemaining;
      if ((written = hdfsWrite(fs, writeFile, (void*)buffer, curSize)) != curSize) {
        fprintf(stderr, "ERROR: hdfsWrite returned an error on write: %d\n", written);
        exit(-3);
      }
    }
    stop();

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

    printf("Test read with buffer size: %d\n",bufferSize);
    start();
    // read from the file
    curSize = bufferSize;
    for (; curSize == bufferSize;) {
        curSize = hdfsRead(fs, readFile, (void*)buffer, curSize);
    }
    stop();

    free(buffer);
    hdfsCloseFile(fs, readFile);
    hdfsDisconnect(fs);

    return 0;
}

int HDFSTestFixture::get_hosts(const char *fileName) {
    hdfsFS fs;
    hdfsFile readFile;
    char* buffer;
    tSize curSize;

    fs = hdfsConnect("default", 0);
    if (!fs) {
        fprintf(stderr, "Oops! Failed to connect to hdfs!\n");
        exit(-1);
    }

    char ***hosts = hdfsGetHosts(fs, fileName, 0, 1);
    int i,j;
    if(hosts) {
       fprintf(stdout, "hdfsGetHosts - SUCCESS! ... \n");
       i=0;
       while(hosts[i]) {
          j = 0;
          while(hosts[i][j]) {
             fprintf(stdout,"\thosts[%d][%d] - %s\n", i, j, hosts[i][j]);
             ++j;
          }
          ++i;
       }
    } else {
       fprintf(stdout, "waah! hdfsGetHosts - FAILED!\n");
    }

    hdfsFreeHosts(hosts);
    hdfsDisconnect(fs);

    return 0;
}

TEST_F(HDFSTestFixture, check_hdfs_read_write) {
   totalSize=1000000000;
   check_hdfs_write("/tmp/testFile_rw",totalSize,4096);
   check_hdfs_read("/tmp/testFile_rw",4096);

   check_hdfs_write("/tmp/testFile_rw",totalSize,1048576);
   check_hdfs_read("/tmp/testFile_rw",1048576);

   check_hdfs_write("/tmp/testFile_rw",totalSize,10485760);
   check_hdfs_read("/tmp/testFile_rw",10485760);

   check_hdfs_write("/tmp/testFile_rw",totalSize,104857600);
   check_hdfs_read("/tmp/testFile_rw",104857600);

   get_hosts("/tmp/testFile_rw");
}

