 
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <time.h>
#include "common.h"
#include "CMessage.h"
#include "CRtmLog.h"   

const clock_t CLOCK_PER_SEC = 1000000;


/* ******************************************************************
 Function:  show the usage of the run time log parser
    
******************************************************************** */
void UsageDisplay(char* exeFilePath)
{
    std::cout
    <<"\n----------------- Usage: ------------------\n"
    <<" * [Function:] run time log parser.\n"
    <<" * Input:\n"
    <<"    [1] run time log file path or directory\n"
    <<"    [2] filter mode: \n"
    <<"         0: no mode seclete (default)\n"
    <<"         1: white list\n"
    <<"         2: black list\n"
    <<" * @Example: "<<exeFilePath<<" drx_log/drx.log"<<std::endl
    <<" * @Example: "<<exeFilePath<<" drx_log/drx.log 1"<<std::endl
    <<" * @Example: "<<exeFilePath<<" drx_log 1"<<std::endl
    <<" * Output: \n"
    <<"       file(s) named *_extractLog.txt at the same path as log file.\n"
    <<"-------------------------------------------\n";
}

bool InputValidationCheck(int argc, char* argv[])
{
    if((argc<2) || (argc>3))
    {
        UsageDisplay(argv[0]);
        return 0;
    }
    else if((argc == 3) && strcmp(argv[2], "0") && strcmp(argv[2], "1") && strcmp(argv[2], "2"))
    {
        std::cout<<"\n** Input error: The second argument should be '0', '1' or '2' \n";
        UsageDisplay(argv[0]);
        return 0;
    }
    return 1;    
}

/* ******************************************************************
    Main Function of run time log parser
    
******************************************************************** */
int main(int argc, char* argv[])
{
    if(InputValidationCheck(argc, argv))
    {
        clock_t start = clock();
        char mode = '0';
        if (argc == 3){
            mode = *argv[2];
        }
        std::vector<std::string> logFileList = TraverseDir(argv[1]);
        if (!logFileList.empty())
        {
            CMessage msg_obj(mode);
            MAP_SS msgDic = msg_obj.GetFinalMsgDic();

            CRtmLog rtmLogParse(msgDic, mode);
            rtmLogParse.LogParseOnFileList(logFileList);
        } 
        double duration = (double)(clock()-start)/CLOCK_PER_SEC;
        std::cout<<" * duration: "<<duration<<" seconds.\n";        
    }  
    return 0;
}


