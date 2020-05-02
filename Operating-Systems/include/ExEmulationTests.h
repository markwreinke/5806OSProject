//
// Created by csis on 2/8/20.
//

#ifndef INC_5806OSPROJECT_EXEMULATIONTESTS_H
#define INC_5806OSPROJECT_EXEMULATIONTESTS_H

#include <iostream>
#include <iomanip>
#include "VDIFile.h"
#include "StepZDebug.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "Partition.h"
#include "Ext2File.h"
#include "Inode.h"
#include "FileAccess.h"
#include "Directories.h"
#include "FilePaths.h"
#include "copyFile.h"
using namespace std;

class ExEmulationTests {
public:
    static void runEmTest(int step, int example);
    static void step0Ex1();
    static void step0Ex2();
    static void step0Ex3();
    static void step1Ex1();
    static void step1Ex2();
    static void step1Ex3();
    static void step1Ex4();
    static void step2Ex1();
    static void step2Ex2();
    static void step2Ex3();
    static void step3Ex1();
    static void step3Ex2();
    static void step3Ex3();
    static void step4Ex1();
    static void step4Ex2();
    static void step4Ex3();
    static void step5Ex1();
    static void step5Ex2();
    static void step6Ex1();
    static void step7Ex1();
    static void step8Ex1();
    static void step8Ex2();
    static void step8Ex3();
    static void step8Ex4();
};


#endif //INC_5806OSPROJECT_EXEMULATIONTESTS_H
