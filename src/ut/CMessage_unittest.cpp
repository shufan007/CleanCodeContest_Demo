#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
#include "ForcePublic_CMessage.h"	//Force public on "../src/CMessage.h"
#include "../gtest-1.7.0/include/gtest/gtest.h"


class CMessageTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		test_file1 = "test_files/drx1.log";
		test_file_nonexistent = "test_files/abc.log";
		message_file = "message_info/messages.properties";
		white_list_file = "message_info/white_list.txt";
		black_list_file = "message_info/black_list.txt";

		m1 = CMessage('0');
		m2 = CMessage('1');
		m3 = CMessage('2');
	}
	// virtual void TearDown() {}

	CMessage m1;
	CMessage m2;
	CMessage m3;

	std::string test_file1;
	std::string test_file_nonexistent;
	std::string message_file;
	std::string white_list_file;
	std::string black_list_file;
};

/*test for  CMessage::CMessage()*/
TEST_F(CMessageTest, CMessageTest_constructor_ok)
{
	EXPECT_EQ('0', m1.m_mode) ;
}

/*test for CMessage::MsgIdCheck */
TEST_F(CMessageTest, CMessageTest_MsgIdCheck_ok)
{
	EXPECT_EQ(1, m1.MsgIdCheck("093A")) ;
}

TEST_F(CMessageTest, CMessageTest_MsgIdCheck_nok_nonvalidChar)
{
	EXPECT_EQ(0, m1.MsgIdCheck("093K")) ;
}

TEST_F(CMessageTest, CMessageTest_MsgIdCheck_nok_nonvalidStrLength)
{
	EXPECT_EQ(0, m1.MsgIdCheck("093AA")) ;
}

/*test for CMessage::AdjustMsgId */
TEST_F(CMessageTest, CMessageTest_AdjustMsgId)
{
	ASSERT_TRUE("093a" == m1.AdjustMsgId("93A"));
}


/*test for CMessage::AdjustMsgName */
TEST_F(CMessageTest, CMessageTest_AdjustMsgName)
{
	ASSERT_TRUE("RTM_NodeId_CpidReq" == m1.AdjustMsgName("  RTM_NodeId_CpidReq "));
}


/*test for CMessage::MessagePropertyExtract */
TEST_F(CMessageTest, CMessageTest_MessagePropertyExtract_ok)
{
	m1.m_msgDic.clear();
	std::string msgPropertyStr = "0970=RTM_NodeId_CpidReq";
	m1.MessagePropertyExtract(msgPropertyStr);
	EXPECT_EQ(2, m1.m_msgDic.size());
	ASSERT_TRUE("RTM_NodeId_CpidReq" == m1.m_msgDic["0970"]);
	ASSERT_TRUE("0970" == m1.m_msgDic["RTM_NodeId_CpidReq"]);
	m1.m_msgDic.clear();
}


/*test for CMessage::FilterMessageExtract */
TEST_F(CMessageTest, CMessageTest_FilterMessageExtract_msgId)
{
	m1.m_msgDic.clear();
	m1.GetFinalMsgDic();
	std::string filterMsgStr = "0970";
	m1.FilterMessageExtract(filterMsgStr);
	EXPECT_EQ(1, m1.m_filterMsgList.size());
	ASSERT_TRUE(m1.m_msgDic["0970"].size());
	m1.m_msgDic.clear();
	m1.m_filterMsgList.clear();
}

TEST_F(CMessageTest, CMessageTest_FilterMessageExtract_msgName)
{
	m1.m_msgDic.clear();
	m1.GetFinalMsgDic();
	std::string filterMsgStr = "RTM_NodeId_CpidReq";
	m1.FilterMessageExtract(filterMsgStr);
	EXPECT_EQ(1, m1.m_filterMsgList.size());
	ASSERT_TRUE(m1.m_msgDic["RTM_NodeId_CpidReq"].size());
	m1.m_msgDic.clear();
	m1.m_filterMsgList.clear();
}


/*test for CMessage::LoadMessageProperty */
TEST_F(CMessageTest, CMessageTest_LoadMessageProperty_ok)
{
	m1.m_msgDic.clear();
	m1.LoadMessageProperty(message_file.c_str());
	EXPECT_LT(0, m1.m_msgDic.size()) ;
	m1.m_msgDic.clear();
}

TEST_F(CMessageTest, CMessageTest_LoadMessageProperty_loadFail)
{
	m1.m_msgDic.clear();
	m1.LoadMessageProperty(test_file_nonexistent.c_str());
	EXPECT_EQ(0, m1.m_msgDic.size()) ;
	m1.m_msgDic.clear();
}

/*test for CMessage::LoadFilterMessage */
TEST_F(CMessageTest, CMessageTest_LoadFilterMessage_ok)
{
	m1.m_msgDic.clear();
	m1.LoadMessageProperty(message_file.c_str());
	int size = m1.m_msgDic.size();
	m1.LoadFilterMessage(white_list_file.c_str());
	EXPECT_EQ(m1.m_msgDic.size(), size) ;
	m1.m_msgDic.clear();
}


TEST_F(CMessageTest, CMessageTest_LoadFilterMessage_loadFail)
{
	m1.m_msgDic.clear();
	m1.LoadMessageProperty(message_file.c_str());
	int size = m1.m_msgDic.size();	
	m1.LoadFilterMessage(test_file_nonexistent.c_str());
	EXPECT_EQ(size, m1.m_msgDic.size());
	m1.m_msgDic.clear();
}


/*test for CMessage::GetFinalMsgDic */
TEST_F(CMessageTest, CMessageTest_GetFinalMsgDic_mode_1)
{
	m2.m_msgDic.clear();
	m2.GetFinalMsgDic();
	EXPECT_LT(0, m2.m_msgDic.size());
}

TEST_F(CMessageTest, CMessageTest_GetFinalMsgDic_mode_2)
{
	m3.m_msgDic.clear();
	m3.GetFinalMsgDic();
	EXPECT_LT(0, m3.m_msgDic.size());
	m1.m_msgDic.clear();
}



