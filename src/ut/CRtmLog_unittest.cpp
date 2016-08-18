#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include "../src/CMessage.h"
#include "ForcePublic_CRtmLog.h"	//Force public on "../src/CRtmLog.h"
#include "../gtest-1.7.0/include/gtest/gtest.h"


class CRtmLogTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		test_file0 = "test_files/drx.log";
		test_file1 = "test_files/drx1.log";
		test_file2 = "test_files/drx2.log";
		test_file_nonexistent = "test_files/abc.log";

		test_fileList_1.push_back(test_file0);
		test_fileList_2.push_back(test_file1);
		test_fileList_2.push_back(test_file2);
		test_fileList_2.push_back(test_file_nonexistent);

		test_str1 = "[0x1011110b->0x1011127f] msg_type:42e1, flag:0, payload length:40"; // ExtractPattern_messageType1
		test_str2 = "RTM   <24:02.00> ######### Prev_Subframe=1, ";	// ExtractPattern_timeType
		test_str3 = "RTM   <24:05.00> User Delete: crnti (54135)";	// ExtractPattern_noteType
		test_str4 = "RTM   <23:06.00> ######### Prev_Subframe=1, "; // ExtractPattern_timeType
		test_str5 = "0x0970: 1011127f --> 101107d1"; // ExtractPattern_messageType2
		test_str6 = "AddSlave/ slave 1 nid=0x2011"; // no pattern matched

		testStrList.push_back(test_str1);
		testStrList.push_back(test_str2);
		testStrList.push_back(test_str3);
		testStrList.push_back(test_str4);
		testStrList.push_back(test_str5);
		testStrList.push_back(test_str6);

		CMessage msg_obj('0');
		MAP_SS msgDic = msg_obj.GetFinalMsgDic();

		r1 = CRtmLog(msgDic, '0');
		r2 = CRtmLog(msgDic, '1');
		r3 = CRtmLog(msgDic, '2');

	}
	// virtual void TearDown() {}

	CRtmLog r1;
	CRtmLog r2;
	CRtmLog r3;

	std::string test_file0;
	std::string test_file1;
	std::string test_file2;
	std::string test_file_nonexistent;

	VS test_fileList_1;
	VS test_fileList_2;

	std::string test_str1;	
	std::string test_str2;	
	std::string test_str3;
	std::string test_str4;
	std::string test_str5;	
	std::string test_str6;	

	VS testStrList;
};

/*test for  CRtmLog::CRtmLog()*/
TEST_F(CRtmLogTest, CRtmLogTest_constructor_ok)
{
	EXPECT_EQ('0', r1.m_mode) ;
	EXPECT_LT(0, r1.m_msgDic.size());
}


/*test for CRtmLog::SaveMessageItems */
TEST_F(CRtmLogTest, CRtmLogTest_SaveMessageItems_mode_1)
{
	std::string msgId("0970");
	std::string fromId("1011127f");
	std::string toId("101107d1");
	r1.SaveMessageItems(msgId, fromId, toId);
	
	EXPECT_EQ(3, r1.m_extractedLog[0].size());
	ASSERT_TRUE("message" == r1.m_extractedLog[0].front().first);
	ASSERT_TRUE("RTM_NodeId_CpidReq" == r1.m_extractedLog[0].front().second);
	ASSERT_TRUE("from" == r1.m_extractedLog[0].at(1).first);
	ASSERT_TRUE("1011127f" == r1.m_extractedLog[0].at(1).second);
	ASSERT_TRUE("to" == r1.m_extractedLog[0].at(2).first);
	ASSERT_TRUE("101107d1" == r1.m_extractedLog[0].at(2).second);	
	r1.m_extractedLog.clear();
}


/*test for CRtmLog::ExtractPattern_messageType1 */
TEST_F(CRtmLogTest, CRtmLogTest_ExtractPattern_messageType1_ok)
{
	EXPECT_EQ(1, r1.ExtractPattern_messageType1(test_str1));
	EXPECT_EQ(3, r1.m_extractedLog[0].size());
	ASSERT_TRUE("message" == r1.m_extractedLog[0].front().first);
	ASSERT_TRUE("PS_DlPrbsAvailInd" == r1.m_extractedLog[0].front().second);
	ASSERT_TRUE("1011110b" == r1.m_extractedLog[0].at(1).second);
	ASSERT_TRUE("1011127f" == r1.m_extractedLog[0].at(2).second);
	r1.m_extractedLog.clear();
}

/*test for CRtmLog::ExtractPattern_messageType2 */
TEST_F(CRtmLogTest, CRtmLogTest_ExtractPattern_messageType2_ok)
{
	EXPECT_EQ(1, r1.ExtractPattern_messageType2(test_str5));
	EXPECT_EQ(3, r1.m_extractedLog[0].size());
	ASSERT_TRUE("message" == r1.m_extractedLog[0].front().first);
	ASSERT_TRUE("RTM_NodeId_CpidReq" == r1.m_extractedLog[0].front().second);
	ASSERT_TRUE("from" == r1.m_extractedLog[0].at(1).first);
	ASSERT_TRUE("1011127f" == r1.m_extractedLog[0].at(1).second);
	ASSERT_TRUE("to" == r1.m_extractedLog[0].at(2).first);
	ASSERT_TRUE("101107d1" == r1.m_extractedLog[0].at(2).second);	
	r1.m_extractedLog.clear();
}


/*test for CRtmLog::ExtractPattern_timeType */
TEST_F(CRtmLogTest, CRtmLogTest_ExtractPattern_timeType_ok)
{
	EXPECT_EQ(1, r1.ExtractPattern_timeType(test_str2));
	EXPECT_EQ(1, r1.m_extractedLog[0].size());
	ASSERT_TRUE("time" == r1.m_extractedLog[0].front().first);
	ASSERT_TRUE("24:02" == r1.m_extractedLog[0].front().second);	
	r1.m_extractedLog.clear();
}


/*test for CRtmLog::ExtractPattern_noteType */
TEST_F(CRtmLogTest, CRtmLogTest_ExtractPattern_noteType_ok)
{
	EXPECT_EQ(1, r1.ExtractPattern_noteType(test_str3));
	EXPECT_EQ(1, r1.m_extractedLog[0].size());
	ASSERT_TRUE("note" == r1.m_extractedLog[0].front().first);
	ASSERT_TRUE("User Delete: crnti (54135)" == r1.m_extractedLog[0].front().second);		
	r1.m_extractedLog.clear();
}


/*test for CRtmLog::ExtractLogInfo */
TEST_F(CRtmLogTest, CRtmLogTest_ExtractLogInfo_ok)
{
    for(VS::const_iterator it = testStrList.begin(); it != testStrList.end(); it++)
    {
 		r1.ExtractLogInfo(*it);      
    } 	
	EXPECT_EQ(5, r1.m_extractedLog.size());
	r1.m_extractedLog.clear();
}

TEST_F(CRtmLogTest, CRtmLogTest_ExtractLogInfo_getNothing)
{
	r1.ExtractLogInfo(test_str6);	
	EXPECT_EQ(0, r1.m_extractedLog.size());
	r1.m_extractedLog.clear();
}

/*test for CRtmLog::ExtractLogInfoFromFile */
TEST_F(CRtmLogTest, CRtmLogTest_ExtractLogInfoFromFile_ok)
{
	r1.ExtractLogInfoFromFile(test_file0.c_str());	
	EXPECT_EQ(962, r1.m_extractedLog.size());
	r1.m_extractedLog.clear();
}

TEST_F(CRtmLogTest, CRtmLogTest_ExtractLogInfoFromFile_fileOpenError)
{
	r1.ExtractLogInfoFromFile(test_file_nonexistent.c_str());	
	EXPECT_EQ(0, r1.m_extractedLog.size());
	r1.m_extractedLog.clear();
}


/*test for CRtmLog::GetExtractLogMap */
TEST_F(CRtmLogTest, CRtmLogTest_GetExtractLogMap_ok)
{
	r1.ExtractLogInfoFromFile(test_file0.c_str());
	MULTIMAP_S_D_PAIR_SS extractLogMap;
	extractLogMap = r1.GetExtractLogMap();
	EXPECT_EQ(757, extractLogMap.size());	
	EXPECT_EQ(0, r1.m_extractedLog.size());
	r1.m_extractedLog.clear();
}


/*test for CRtmLog::JosnFormatConversion */
TEST_F(CRtmLogTest, CRtmLogTest_JosnFormatConversion_ok)
{
	VS josnLog;
    for(VS::const_iterator it = testStrList.begin(); it != testStrList.end(); it++)
    {
 		r1.ExtractLogInfo(*it);      
    } 
	r1.JosnFormatConversion(josnLog);
	std::string expectStr1, expectStr2, expectStr3;
    expectStr1 = "{time:\"?\", message:\"PS_DlPrbsAvailInd\", from:\"1011110b\", to:\"1011127f\"}";
    expectStr2 = "{time:\"23:06\", message:\"RTM_NodeId_CpidReq\", from:\"1011127f\", to:\"101107d1\"}";     
    expectStr3 = "{time:\"24:02\", note:\"User Delete: crnti (54135)\"}";
    EXPECT_EQ(0, r1.m_extractedLog.size());
	EXPECT_EQ(3, josnLog.size());
	ASSERT_TRUE(expectStr1 == josnLog[0]);
	ASSERT_TRUE(expectStr2 == josnLog[1]);
	ASSERT_TRUE(expectStr3 == josnLog[2]);
	r1.m_extractedLog.clear();	
}


/*test for CRtmLog::LogParseOnFile */
TEST_F(CRtmLogTest, CRtmLogTest_LogParseOnFile)
{
	r1.LogParseOnFile(test_file0.c_str());
	EXPECT_EQ(0, r1.m_extractedLog.size());
	const char* outfile("test_files/drx_extractLog.txt");		
    std::ifstream infile(outfile);
    ASSERT_TRUE(infile);
	r1.m_extractedLog.clear();
}

/*test for CRtmLog::LogParseOnFileList */
TEST_F(CRtmLogTest, CRtmLogTest_LogParseOnFileList)
{
	r3.LogParseOnFileList(test_fileList_2);
	EXPECT_EQ(0, r3.m_extractedLog.size());
	const char* outfile1("test_files/drx1_extractLog.txt");	
	const char* outfile2("test_files/drx2_extractLog.txt");	
    std::ifstream infile1(outfile1);
    std::ifstream infile2(outfile2);
    ASSERT_TRUE(infile1);
    ASSERT_TRUE(infile2);
	r3.m_extractedLog.clear();
}
