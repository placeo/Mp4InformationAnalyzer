/*
 * CMp4AnalyzerConfiguration.cpp
 *
 *  Created on: Apr 11, 2018
 *      Author: placeo
 */

#include "CMp4AnalyzerConfiguration.h"

#include <json-c/json.h>
#ifdef __APPLE__
 #include <mach-o/dyld.h>
#endif

CMp4AnalyzerConfiguration* CMp4AnalyzerConfiguration::pInstance_ = NULL;

CMp4AnalyzerConfiguration::CMp4AnalyzerConfiguration() : logLevel_(),
																				logType_(),
																				maxLogFileSize_(0),
																				maxLogBackupIndex_(0),
																				basePath_(),
																				configurationPath_() {

}

CMp4AnalyzerConfiguration::~CMp4AnalyzerConfiguration() {
	// TODO Auto-generated destructor stub
}

bool CMp4AnalyzerConfiguration::loadConfiguration() {
	char executablePath[2048] = {0, };
	FILE* filePointer = NULL;
	long int fileSize = 0;
	int returnValue = 0;
	char* configurationMessage = NULL;

#ifdef __APPLE__
	uint32_t basePathBufferSize = sizeof(executablePath);
	if(_NSGetExecutablePath(executablePath, &basePathBufferSize) == 0) {
		basePath_ = string(executablePath).substr(0, string(executablePath).rfind("/"));
		printf("Executable path is %s\n", basePath_.c_str());
	}
	else {
		printf("Buffer size is too small, size : %u\n", basePathBufferSize);
		return false;
	}
#elif  __linux__
	ssize_t len = 0;
	if((len = readlink("/proc/self/exe", executablePath, sizeof(executablePath))) != -1) {
		basePath_ = string(executablePath).substr(0, string(executablePath).rfind("/"));
		printf("Executable path is %s\n", basePath_.c_str());
	}
	else {
		perror("Read Link Error");
		return false;
	}
#endif

	filePointer = fopen((basePath_ + "/conf/tester.config").c_str(), "r");
	if(NULL == filePointer) {
		printf("Tester configuration file open error\n");
		return false;
	}

	fseek(filePointer, 0, SEEK_END);
	fileSize = ftell(filePointer);

	configurationMessage = (char*)calloc(fileSize+1, 1);
	if(NULL == configurationMessage) {
		printf("Configuration message memory allocation error\n");
		if(0 != fclose(filePointer)) {
			printf("Configuration file closing error\n");
		}
		return false;
	}

	rewind(filePointer);

	returnValue = fread(configurationMessage, 1, fileSize, filePointer);
	if(returnValue <= 0) {
		printf("Configuration message read error\n");
		if(0 != fclose(filePointer)) {
			printf("Configuration file closing error\n");
		}
		return false;
	}

	printf("Configuration Message : %s", configurationMessage);

	if(false == parseConfiguration(configurationMessage)) {
		printf("Configuration file parsing error\n");
		if(0 != fclose(filePointer)) {
			printf("Configuration file closing error\n");
		}
		free(configurationMessage);
		return false;
	}

	free(configurationMessage);

	if(0 != fclose(filePointer)) {
		printf("Configuration file closing error\n");
		return false;
	}

	return true;
}

bool CMp4AnalyzerConfiguration::parseConfiguration(char* configurationMessage) {
	json_object* jsonData = json_tokener_parse(configurationMessage);
	if(NULL == jsonData) {
		printf("Failed to tokenize json message\n");
		json_object_put(jsonData);
		return false;
	}
	else {
		json_object* jsonLog = NULL;

		if(0 == json_object_object_get_ex(jsonData, "log", &jsonLog)) {
			printf("Failed to parse json log\n");
			json_object_put(jsonData);
			return false;
		}
		else {
			json_object* jsonLogType = NULL;
			json_object* jsonLogLevel = NULL;
			json_object* jsonMaxFileSize = NULL;
			json_object* jsonMaxBackupIndex = NULL;

			if(0 == json_object_object_get_ex(jsonLog, "type", &jsonLogType)) {
				printf("Failed to parse json log type\n");
				json_object_put(jsonData);
				return false;
			}
			else {
				logType_ = string(json_object_get_string(jsonLogType));
			}

			if(0 == json_object_object_get_ex(jsonLog, "level", &jsonLogLevel)) {
				printf("Failed to parse json log level\n");
				json_object_put(jsonData);
				return false;
			}
			else {
				logLevel_ = string(json_object_get_string(jsonLogLevel));
			}

			if(0 == json_object_object_get_ex(jsonLog, "maxFileSize", &jsonMaxFileSize)) {
				printf("Failed to parse json log max file size\n");
				json_object_put(jsonData);
				return false;
			}
			else {
				maxLogFileSize_ = json_object_get_int(jsonMaxFileSize);
			}

			if(0 == json_object_object_get_ex(jsonLog, "maxBackupIndex", &jsonMaxBackupIndex)) {
				printf("Failed to parse json log max backup index\n");
				json_object_put(jsonData);
				return false;
			}
			else {
				maxLogBackupIndex_ = json_object_get_int(jsonMaxBackupIndex);
			}
		}
	}
	json_object_put(jsonData);
	return true;
}

bool CMp4AnalyzerConfiguration::displayConfiguration() {
	printf("[LOG INFO]\n");
	printf("LogType : %s, LogLevel : %s, MaxFileSize : %d, MaxBackupIndex : %d\n", logType_.c_str(), logLevel_.c_str(), maxLogFileSize_, maxLogBackupIndex_);
	return true;
}
