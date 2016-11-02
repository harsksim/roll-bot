#include "stdafx.h"
#include "dice.h"
#include <sstream>

using namespace std;

CQTool tool;
string reg="^[\\./][rR]( ([1-9])[dD]([1-9][0-9]{0,2})(([\\+-])([1-9][0-9]{0,3}))?)?( [\\D]+)?";
string reg2="^[\\./][rR] ([1-9][0-9]{0,2})( [\\D]+)?";
int32_t auth;
unsigned int seed = time(NULL);

int32_t sendDice(int32_t AuthCode, int64_t groupCode, int64_t qqCode, const char *msg) {
	// 接收消息转成字符串
	string reqMsg = msg;

	auth = AuthCode;

	// 判断触发key
	smatch m;
	string result = "";
	if (regex_match(reqMsg, m, regex(reg2))) {
		string result = rollSingle(m);

		string reason = "";
		if (m.length() >2 && m[2].length() > 0) {
			reason = " 为" + m[2].str();
		}

		// 取发消息的用户信息
		if (groupCode > 0) {
			string reqUser = CQ_getGroupMemberInfoV2(AuthCode, groupCode, qqCode, false);
			CQ_Type_GroupMember member;
			tool.GetGroupMemberInfo(AuthCode, groupCode, qqCode, member);

			string req = "1d" + m[1].str();

			string name = member.nick;
			if (member.nick.length() == 0) {
				name = member.username;
			}
			

			reqMsg = ">" + name + reason + " 掷 " + req + " → " + result;

			CQ_sendGroupMsg(AuthCode, groupCode, reqMsg.c_str());

		} else {
			string req = "1d" + m[1].str();
			reqMsg = ">你" + reason + " 掷 " + req + " → " + result;
						
			CQ_sendPrivateMsg(AuthCode, qqCode, reqMsg.c_str());
		}

	} else if (regex_match(reqMsg, m, regex(reg))) {
		string result = roll(m);

		string reason = "";
		if (m.length() > 7 && m[7].length() > 0) {
			reason = " 为" + m[7].str();
		}

		// 取发消息的用户信息
		if (groupCode > 0) {
			string reqUser = CQ_getGroupMemberInfoV2(AuthCode, groupCode, qqCode, false);
			CQ_Type_GroupMember member;
			tool.GetGroupMemberInfo(AuthCode, groupCode, qqCode, member);

			string req = "";
			if (m[1].str().length() > 0) {
				req = m[1].str();
			} else {
				req = "1d100";
			}

			string name = member.nick;
			if (member.nick.length() == 0) {
				name = member.username;
			}

			reqMsg = ">" + name + reason + " 掷 " + req + " → " + result;

			CQ_sendGroupMsg(AuthCode, groupCode, reqMsg.c_str());

		} else {
			if (m[1].str().length() > 0) {
				reqMsg = ">你" + reason + " 掷 " + m[1].str() + " → " + result;
			} else {
				reqMsg = ">你" + reason + " 掷 1d100 → " + result;
			}
			
			CQ_sendPrivateMsg(AuthCode, qqCode, reqMsg.c_str());
		}
	}


	return EVENT_BLOCK;

}

string rollSingle(smatch &m) {
	// 初始化随机数
	srand(seed);

	CQ_addLog(auth, CQLOG_DEBUG, "rollsingle", m.str().c_str());
	for (int32_t i = 0; i < m.length(); i++) {
		CQ_addLog(auth, CQLOG_DEBUG, "roll", m[i].str().c_str());
	}

	string result = "";
	
	int mod = 1;
	if (m[1].str().length() > 0) {
		mod = str2int(m[1].str());
	} else {
		mod = 100;
	}

	int32_t random = (rand() % mod) + 1;

	CQ_addLog(auth, CQLOG_DEBUG, "rollsingle", int2str(random).c_str());

	result = int2str(random);

	seed = rand();

	return result;
}

string roll(smatch &m) {
	// 初始化随机数
	srand(seed);

	CQ_addLog(auth, CQLOG_DEBUG, "roll", m.str().c_str());
	for (int32_t i = 0; i < m.length(); i++) {
		CQ_addLog(auth, CQLOG_DEBUG, "roll", m[i].str().c_str());
	}

	string result = "";

	if (m.length() > 2 && m[2].length() > 0 && m[3].length() > 0) {

		int32_t round = str2int(m[2].str());
		int32_t range = str2int(m[3].str());
		
		int32_t sum = 0;
		for (int32_t i=0; i < round; i++) {
			int32_t random = (rand() % range) + 1;
			sum += random;
			if (result.size() > 0) {
				result.append("+");
			} else if (round > 1) {
				result.append("(");
			}
			result.append(int2str(random));
		}

		if (round > 1) {
			result.append(")");
		}

		if (m[4].str().length() > 0) {
			result.append(m[5]);
			if (m[5].str() == "+") {
				sum += str2int(m[6].str());
			} else {
				sum -= str2int(m[6].str());
			}
		}

		if (round > 1 || m[4].str().length() > 0) {
			result.append(m[6].str()).append(" → ").append(int2str(sum));
		}

		CQ_addLog(auth, CQLOG_DEBUG, "roll_group", result.c_str());

	} else {
		int32_t random = (rand() % 100) + 1;

		CQ_addLog(auth, CQLOG_DEBUG, "roll_default", int2str(random).c_str());

		result = int2str(random);
	}

	seed = rand();

	return result;
}

string int2str(int32_t i) {
	return static_cast<ostringstream*>( &(ostringstream() << i) )->str();
}

int32_t str2int(string s) {
	int32_t i;
	if ( ! (istringstream(s) >> i) ) i = 0;
	return i;
}