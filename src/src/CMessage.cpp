
#include <iostream>
#include <fstream>
#include "common.h"
#include "CMessage.h"

const char* MESSAGE_FILE("message_info/messages.properties");
const char* WHITE_LIST_FILE("message_info/white_list.txt");
const char* BLACK_LIST_FILE("message_info/black_list.txt");

CMessage::CMessage(){};

CMessage::CMessage(char mode):
    m_mode(mode)
{
}

CMessage::~CMessage(void){}


bool CMessage::MsgIdCheck(const std::string &MsgIdStr)
{
    int validCharNum = 0;
    for(int i = 0; i < MsgIdStr.size(); i ++)
    {
        if(isdigit(MsgIdStr[i]) || isspace(MsgIdStr[i]) 
            || ((MsgIdStr[i] >= 'A' && MsgIdStr[i] <= 'F') 
            || (MsgIdStr[i] >= 'a' && MsgIdStr[i] <= 'f')))
        {
            if(isgraph(MsgIdStr[i]))
            {
                validCharNum ++;
            }            
        }
        else
        {
            return 0;
        }
    }

    return ((validCharNum > 0) && (validCharNum <= 4)) ? 1 : 0;
}


std::string CMessage::AdjustMsgId(const std::string &msgId)
{
    const int msgIdSize = 4;
    std::string outMsgId(msgIdSize, '0');
    int pos = 0;
    for(int i = msgId.size() - 1; i >= 0; i --)
    {
        if(isgraph(msgId[i]))
        {
            pos ++;
            outMsgId[msgIdSize - pos] = tolower(msgId[i]);           
        }
    }
    return outMsgId;
}


std::string CMessage::AdjustMsgName(const std::string &msgName)
{
    std::string outMsgName(msgName.size(), '0');
    int pos = 0;
    for(int i = 0; i < msgName.size(); i ++)
    {
        if(isgraph(msgName[i]))
        {           
            outMsgName[pos] = msgName[i]; 
            pos ++;          
        }
    }
    if(pos < msgName.size())
    {
        outMsgName.erase(pos, msgName.size() - pos);
    }   
    return outMsgName;
}


void CMessage::MessagePropertyExtract(const std::string &msgPropertyStr)
{
    std::string msgId, msgName;
    VS msgData = StrSplit(msgPropertyStr, "=", 2);
    if((!msgData.empty()) && (MsgIdCheck(msgData[0])))
    {               
        msgId = AdjustMsgId(msgData[0]);
        msgName = AdjustMsgName(msgData[1]);                   
        m_msgDic.insert(PAIR_SS(msgId, msgName));
        m_msgDic.insert(PAIR_SS(msgName, msgId));                 
    }         
}


void CMessage::FilterMessageExtract(const std::string &filterMsgStr)
{
    VS msgData = StrSplit(filterMsgStr, "=", 2);
    if(!msgData.empty())
    {
        std::string msgId, msgName;  
        MAP_SS::iterator it;                
        if (MsgIdCheck(msgData[0])){
            msgId = AdjustMsgId(msgData[0]);
            it = m_msgDic.find(msgId);
            if(it != m_msgDic.end()){
                m_filterMsgList.insert(PAIR_SS(msgId, m_msgDic[msgId]));
            }
        } 
        else{
            msgName = AdjustMsgName(msgData[0]);
            it = m_msgDic.find(msgName);
            if(it != m_msgDic.end()){
                m_filterMsgList.insert(PAIR_SS(m_msgDic[msgName], msgName)); 
            } 
        }                
    }        
}


void CMessage::LoadMessageProperty(const char* fileName)
{   
    std::ifstream infile(fileName);
    if (!infile)
    {      
        perror ("** Error opening file!");
        std::cout<<": "<< fileName<<std::endl;
    }
    else
    {             
        std::string currLine;
        while (getline(infile, currLine))
        {
            MessagePropertyExtract(currLine);        
        } 
    }
    infile.close();
}

void CMessage::LoadFilterMessage(const char* fileName)
{   
    std::ifstream infile(fileName);
    if (!infile)
    {      
        perror ("** Error opening file!");
        std::cout<<": "<< fileName<<std::endl;
    }
    else
    {             
        std::string currLine;
        while (getline(infile, currLine))
        {
            FilterMessageExtract(currLine);        
        } 
    }
    infile.close();
}


MAP_SS CMessage::GetFinalMsgDic()
{
    LoadMessageProperty(MESSAGE_FILE);  
    
    if (m_mode == '1'){
        LoadFilterMessage(WHITE_LIST_FILE);
        m_msgDic = m_filterMsgList;
        m_filterMsgList.clear();
    }
    else if(m_mode == '2'){
        LoadFilterMessage(BLACK_LIST_FILE);      
        MAP_SS::iterator it1, it2;
        for(it1 = m_filterMsgList.begin(); it1 != m_filterMsgList.end(); it1 ++)
        {
            it2 = m_msgDic.find(it1->first);
            if(it2 != m_msgDic.end()){
                m_msgDic.erase(it2);
            }          
        }
    } 
    return  m_msgDic; 
}



