#include "mymainframe.h"

int main(int argc, char *argv[])
{
    TApplication theApp("App",&argc,argv);

    MyMainFrame *my_mframe = new MyMainFrame(gClient->GetRoot(),1500,900);

    theApp.Run();
    return 0;
}
