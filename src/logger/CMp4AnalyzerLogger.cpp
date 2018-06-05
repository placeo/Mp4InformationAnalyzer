/*
 * CMp4AnalyzerLogger.cpp
 *
 *  Created on: Apr 11, 2018
 *      Author: placeo
 */

#include <unistd.h>
#include <dirent.h>
#include "CMp4AnalyzerLogger.h"
#include "CMp4AnalyzerConfiguration.h"
#ifdef __APPLE__
	#include <mach-o/dyld.h>
#endif

bool initializeTesterLogger() {
	if(0 == CMp4AnalyzerConfiguration::getInstance()->getLogType().compare("console")) {
		testerLogAppender = new OstreamAppender("TesterOstreamerAppender", &cout);
	}
	else if(0 == CMp4AnalyzerConfiguration::getInstance()->getLogType().compare("file")) {
		char executablePath[2048] = {0, };
		DIR* logDirectory = NULL;
		string basePath = "";
		string testerLogFileName = "";

#ifdef __APPLE__
		uint32_t basePathBufferSize = sizeof(executablePath);
		if(_NSGetExecutablePath(executablePath, &basePathBufferSize) == 0) {
			basePath = string(executablePath).substr(0, string(executablePath).rfind("/"));
			printf("Executable path is %s\n", basePath.c_str());
		}
		else {
			printf("Buffer size is too small, size : %u\n", basePathBufferSize);
			return false;
		}
#elif  __linux__
		ssize_t len = 0;
		if((len = readlink("/proc/self/exe", executablePath, sizeof(executablePath))) != -1) {
			basePath = string(executablePath).substr(0, string(executablePath).rfind("/"));
			printf("Executable path is %s\n", basePath.c_str());
		}
		else {
			perror("Read Link Error");
			return false;
		}
#endif

		logDirectory = opendir((basePath + "/log").c_str());

		if(logDirectory) {
//			printf("Log directory exists\n");
			closedir(logDirectory);
		}
		else if(ENOENT == errno) {
			printf("Log directory does not exist, make directory\n");
			mkdir((basePath + "/log").c_str(), 0777);
		}
		else {
			printf("Failed to open directory\n");
			perror("ERROR ");
			exit(1);
		}

		testerLogFileName = basePath + "/log/variousStreamDemuxTester";
		testerLogAppender = new RollingFileAppender("TesterLogAppender", testerLogFileName, CMp4AnalyzerConfiguration::getInstance()->getMaxLogFileSize(), CMp4AnalyzerConfiguration::getInstance()->getMaxLogBackupIndex(), true, 0644);
	}
	else {
		printf("Playback log type is invalid");
		return false;
	}

	if(0 == CMp4AnalyzerConfiguration::getInstance()->getLogLevel().compare("off")) {
		testerLogContext->setPriority(Priority::FATAL);
	}
	else if(0 == CMp4AnalyzerConfiguration::getInstance()->getLogLevel().compare("error")) {
		testerLogContext->setPriority(Priority::ERROR);
	}
	else if(0 == CMp4AnalyzerConfiguration::getInstance()->getLogLevel().compare("warning")) {
		testerLogContext->setPriority(Priority::WARN);
	}
	else if(0 == CMp4AnalyzerConfiguration::getInstance()->getLogLevel().compare("notice")) {
		testerLogContext->setPriority(Priority::NOTICE);
	}
	else if(0 == CMp4AnalyzerConfiguration::getInstance()->getLogLevel().compare("info")) {
		testerLogContext->setPriority(Priority::INFO);
	}
	else if(0 == CMp4AnalyzerConfiguration::getInstance()->getLogLevel().compare("debug")) {
		testerLogContext->setPriority(Priority::DEBUG);
	}
	else if(0 == CMp4AnalyzerConfiguration::getInstance()->getLogLevel().compare("trace")) {
		testerLogContext->setPriority(Priority::DEBUG);
	}

	logPatternLayout = new PatternLayout();
	logPatternLayout->setConversionPattern("%d{%m/%d %H:%M:%S.%l} %6.6p %m%n");

	testerLogAppender->setLayout(logPatternLayout);
	testerLogContext->addAppender(testerLogAppender);

	return true;
}
bool finalizeTesterLogger() {
	testerLogContext->shutdown();
	return true;
}
