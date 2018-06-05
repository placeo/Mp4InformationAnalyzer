/*
 * CMm4AnalyzerLogger.h
 *
 *  Created on: Apr 11, 2018
 *      Author: placeo
 */

#ifndef CMP4_ANALYZERLOGGER_H_
#define CMP4_ANALYZERLOGGER_H_

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/RollingFileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"


#define COLOR_RED 			"\x1b[0;31m"
#define COLOR_LIGHT_RED 	"\x1b[1;31m"
#define COLOR_GREEN 		"\x1b[0;32m"
#define COLOR_LIGHT_GREEN 	"\x1b[1;32m"
#define COLOR_ORANGE 		"\x1b[0;33m"
#define COLOR_YELLOW 		"\x1b[1;33m"
#define COLOR_BLUE 			"\x1b[0;34m"
#define COLOR_LIGHT_BLUE	"\x1b[1;34m"
#define COLOR_MAGENTA 		"\x1b[0;35m"
#define COLOR_LIGHT_MAGENTA	"\x1b[1;35m"
#define COLOR_CYAN			"\x1b[0;36m"
#define COLOR_LIGHT_CYAN	"\x1b[1;36m"
#define COLOR_GRAY			"\x1b[0;37m"
#define COLOR_WHITE			"\x1b[1;37m"
#define COLOR_RESET			"\x1b[0m"

using namespace std;
using namespace log4cpp;

#define TestError(component, fmt, ...) do { \
		testerLogContext->error(COLOR_RED "%14.14s %14.14s:  " fmt COLOR_RESET, component, __FUNCTION__, ##__VA_ARGS__); \
	}while(0)

#define TestWarning(component, fmt, ...) do { \
		testerLogContext->warn(COLOR_YELLOW "%14.14s %14.14s:  " fmt COLOR_RESET, component, __FUNCTION__, ##__VA_ARGS__); \
	}while(0)

#define TestNotice(component, fmt, ...) do { \
		testerLogContext->notice(COLOR_LIGHT_GREEN "%14.14s %14.14s:  " fmt COLOR_RESET, component, __FUNCTION__, ##__VA_ARGS__); \
	}while(0)

#define TestInfo(component, fmt, ...) do { \
		testerLogContext->info(COLOR_LIGHT_BLUE "%14.14s %14.14s:  " fmt COLOR_RESET, component, __FUNCTION__, ##__VA_ARGS__); \
	}while(0)

#define TestDebug(component, fmt, ...) do { \
		testerLogContext->debug("%14.14s %14.14s:  " fmt, component, __FUNCTION__, ##__VA_ARGS__); \
	}while(0)

static Category* testerLogContext = &(Category::getInstance("VairiousStreamMuxTester"));
static Appender* testerLogAppender = NULL;
static PatternLayout* logPatternLayout = NULL;

bool initializeTesterLogger();
bool finalizeTesterLogger();

#endif /* CMP4_ANALYZERLOGGER_H_ */
