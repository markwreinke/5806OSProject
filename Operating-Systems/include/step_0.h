#ifndef STEP_0_H
#define STEP_0_H
#include <stdint.h>
#include <sys/types.h>
#include <iostream>
#include <iomanip>
#include <fcntl.h>
#include <fstream>
using namespace std;

class step_0
{
    public:
        step_0(char* fn);

        int open2(char *fn);
        void closeFile(int fd);
        ssize_t read(int fd, void *buf, size_t count);
        ssize_t write(int fd, void *buf, size_t count);
        off_t lseek(int fd, off_t offset, int anchor);
        void displayBufferPage(uint8_t *buf,uint32_t count,uint32_t start,uint64_t offset);
        void displayBuffer(uint8_t *buf,uint32_t count,uint64_t offset);


    protected:

    private:
        char *FileName;
};

#endif // STEP_0_H
