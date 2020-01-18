#include <iostream>
#include <iomanip>
#include <VDIfile.h>
#include "step_0.h"
//#include "src/step_0.cpp"
#include <fcntl.h>
using namespace std;


int main()
{
    char* filename = "\VirtualBoxCSIS\CSIS Virtual Box\CSIS Virtual Box Home";
    step_0 *X = new step_0(filename);
   int f = X->open2(filename);
   cout << f << endl;
}
