#pragma once

#include "stdafx.h"
#include <stdlib.h>
#include <string>
#include "cqp.h"
#include "QTool.h"
#include <regex>
#include <time.h>

using namespace std;

int32_t sendDice(int32_t AuthCode, int64_t groupCode, int64_t qqCode, const char *msg);
string roll(smatch &m);
string rollSingle(smatch &m);
string int2str(int32_t i);
int32_t str2int(string s);