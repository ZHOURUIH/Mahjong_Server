﻿#include "txCommand.h"
#include "txCommandReceiver.h"

txCommand::txCommand(const char* file, const int& line, const bool& showInfo)
:
mReceiver(NULL),
mFile(file),
mLine(line),
mShowDebugInfo(showInfo),
mEndCallback(NULL),
mStartCallback(NULL),
mEndUserData(NULL),
mStartUserData(NULL),
mDelayCommand(false)
{}