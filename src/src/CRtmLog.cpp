
#include <iostream>
#include <fstream>
#include "common.h"
#include "CRtmLog.h"
#include "CMessage.h"


CRtmLog::CRtmLog(){};

CRtmLog::CRtmLog(const MAP_SS &msgDic, const char mode) : 
    m_msgDic(msgDic),
    m_mode(mode)
{
}

CRtmLog::~CRtmLog(void){}


void CRtmLog::SaveMessageItems(std::string &msgId, std::string &fromId, std::string &toId)
{
    std::string msgName;
    CMessage msg_obj;   
    msgName = msgId = msg_obj.AdjustMsgId(msgId);    

    MAP_SS::iterator it = m_msgDic.find(msgId);
    if(it != m_msgDic.end()){
        msgName = it->second;
    }

    D_PAIR_SS pairItem;
    if((it != m_msgDic.end()) || (m_mode == '0')){
        pairItem.push_back(PAIR_SS("message", msgName));
        pairItem.push_back(PAIR_SS("from", fromId));
        pairItem.push_back(PAIR_SS("to", toId));
        m_extractedLog.push_back(pairItem);                    
    } 
}


bool CRtmLog::ExtractPattern_messageType1(const std::string &logStr)
{
    int pos_start = logStr.find("[0x");
    if (pos_start == 0) 
    {       
        VS items = StrSplit(logStr, "-x:],", 6);
        SaveMessageItems(items[5], items[1], items[3]);      
        return 1;
    }
    return 0;
}


bool CRtmLog::ExtractPattern_messageType2(const std::string &logStr)
{ 
    int pos_start = logStr.find("0x");
    int pos_arrow = logStr.find("-->");
    if ((pos_start == 0) && pos_arrow != logStr.npos)
    {
        VS items = StrSplit(logStr, "x :,", 5);
        SaveMessageItems(items[1], items[2], items[4]);      
        return 1;
    }
    return 0;
}

bool CRtmLog::ExtractPattern_timeType(const std::string &logStr)
{
    int pos_start = logStr.find("RTM   <");
    if (pos_start == 0)
    {
        int pos = logStr.find("######### Prev_Subframe=");       
        if (pos != logStr.npos)
        {            
            VS items = StrSplit(logStr.substr(pos_start), "<:.>", 3);
            D_PAIR_SS pairItem;
            pairItem.push_back(PAIR_SS("time", items[1] + ":" + items[2]));
            m_extractedLog.push_back(pairItem); 
            return 1;
        }        
    }
    return 0;            
}

bool CRtmLog::ExtractPattern_noteType(const std::string &logStr)
{  
    int pos_start = logStr.find("RTM   <");
    if (pos_start == 0)
    {
        int pos = logStr.find(">");
        if (pos != logStr.npos) 
        {
            std::string str(logStr.size()-pos-1, ' ');
            if (logStr.compare(pos+1, logStr.size()-pos-1, str))
            {
                D_PAIR_SS pairItem;
                pairItem.push_back(PAIR_SS("note", logStr.substr(pos+2, logStr.size()-pos-2)));
                m_extractedLog.push_back(pairItem);                               
            }            
            return 1;
        }
    }
    return 0;
}


void CRtmLog::ExtractLogInfo(const std::string &logStr)
{      
    if (ExtractPattern_messageType1(logStr)) return;
    else if (ExtractPattern_messageType2(logStr)) return;
    else if (ExtractPattern_timeType(logStr)) return;
    ExtractPattern_noteType(logStr);
}


void CRtmLog::ExtractLogInfoFromFile(const char* logFile)
{  
    std::ifstream infile(logFile);
    if (!infile){
        perror ("** Error opening file!");
        std::cout<<": "<< logFile<<std::endl;
        return;
    } 
    else
    {       
        std::string currLine;
        while (getline(infile, currLine))
        {
            ExtractLogInfo(currLine);          
        }
    }
    infile.close();
}

MULTIMAP_S_D_PAIR_SS CRtmLog::GetExtractLogMap()
{
    MULTIMAP_S_D_PAIR_SS extractLogMap;
    std::string timeStemp("0");
    for(V_D_PAIR_SS::const_iterator it = m_extractedLog.begin(); it != m_extractedLog.end(); it ++)
    {
        if(it->front().first == "time")
        {
            timeStemp = it->front().second;
        }
        else{
            extractLogMap.insert(PAIR_S_D_PAIR_SS(timeStemp, *it));
        }        
    } 
    m_extractedLog.clear(); 
    return extractLogMap; 
}


void CRtmLog::JosnFormatConversion(VS &josnLog)
{
    MULTIMAP_S_D_PAIR_SS extractLogMap = GetExtractLogMap();    
    for(MULTIMAP_S_D_PAIR_SS::iterator it = extractLogMap.begin(); it != extractLogMap.end(); it++)
    {
        std::string str;
        std::string timestr= "?";
        if(it->first.compare("0")){
            timestr = it->first;
        }
        it->second.push_front(PAIR_SS("time", timestr));
        josnLog.push_back(JosnFormatCombine(it->second)); 
    } 
}


void CRtmLog::LogParseOnFile(const char* fileName)
{
    ExtractLogInfoFromFile(fileName);

    VS josnLog;
    JosnFormatConversion(josnLog); 

    std::string outfile(fileName);
    outfile.replace(outfile.rfind('.'), outfile.size()-outfile.rfind('.')+1, "_extractLog.txt");   
    SaveStringListToFile(josnLog, outfile.c_str()); 
    std::cout<<" *"<<fileName<<" parse finish.. \n"
    <<"     output file: "<<outfile <<"\n ";
}

void CRtmLog::LogParseOnFileList(const VS &logFileList)
{  
    VS::const_iterator it;
    for(it = logFileList.begin(); it != logFileList.end(); it ++)
    {
        LogParseOnFile(it->c_str());    
    }  
}


