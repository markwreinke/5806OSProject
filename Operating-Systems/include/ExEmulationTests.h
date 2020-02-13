//
// Created by csis on 2/8/20.
//

#ifndef INC_5806OSPROJECT_EXEMULATIONTESTS_H
#define INC_5806OSPROJECT_EXEMULATIONTESTS_H

#include <iostream>
#include <iomanip>
#include "VDIfile.h"
#include "StepZDebug.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
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
};


#endif //INC_5806OSPROJECT_EXEMULATIONTESTS_H
