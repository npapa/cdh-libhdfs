#include <test_posix_read.h>

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


int POSIXTestFixture::check_posix_write(const char *writeFileName, ssize_t fileTotalSize, ssize_t bufferSize) {
    int output_fd;    
    char* buffer;
    int i;
    ssize_t nrRemaining;
    ssize_t curSize;
    ssize_t written;

    // sanity check
    if(fileTotalSize == ULONG_MAX && errno == ERANGE) {
      fprintf(stderr, "invalid file size - must be <= %lu\n", ULONG_MAX);
      exit(-3);
    }
    
    output_fd = open(writeFileName, O_WRONLY | O_CREAT, 0644);
    if(output_fd == -1){
        perror("open");
        return 3;
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

    printf("Test POSIX write total bytes: %d with buffer size: %d\n", fileTotalSize, bufferSize);
    start();    
    // write to the file
    for (nrRemaining = fileTotalSize; nrRemaining > 0; nrRemaining -= bufferSize ) {
      curSize = ( bufferSize < nrRemaining ) ? bufferSize : nrRemaining;
      if ((written = write (output_fd, (void*)buffer, (ssize_t) curSize)) != curSize) {
        perror("write");
        exit(-3);
      }
    }
    stop();

    free(buffer);
    close (output_fd);
    return 0;
}

int POSIXTestFixture::check_posix_read(const char *rfile, ssize_t bufferSize) {
    char* buffer;
    ssize_t curSize;
    int input_fd;

    input_fd = open (rfile, O_RDONLY);
    if (input_fd == -1) {
            perror ("open");
            return 2;
    }

    // data to be written to the file
    buffer = (char *) malloc(sizeof(char) * bufferSize);
    if(buffer == NULL) {
        return -2;
    }

    printf("Test POSIX read with buffer size: %d\n",bufferSize);
    start();
    // read from the file
    curSize = bufferSize;
    for (; curSize == bufferSize;) {
        curSize = read (input_fd, (void*)buffer, curSize);
    }
    stop();

    free(buffer);
    close (input_fd);
    return 0;
}


TEST_F(POSIXTestFixture, check_posix_read_write) {
   totalSize=1000000000;
   check_posix_write("/tmp/testFile_rw",totalSize,4096);
   check_posix_read("/tmp/testFile_rw",4096);

   check_posix_write("/tmp/testFile_rw",totalSize,1048576);
   check_posix_read("/tmp/testFile_rw",1048576);

   check_posix_write("/tmp/testFile_rw",totalSize,10485760);
   check_posix_read("/tmp/testFile_rw",10485760);

   check_posix_write("/tmp/testFile_rw",totalSize,104857600);
   check_posix_read("/tmp/testFile_rw",104857600);
}

