/*
 * CMp4AnalyzerConfiguration.h
 *
 *  Created on: Apr 11, 2018
 *      Author: placeo
 */

#ifndef _CMp4AnalyzerConfiguration_H_
#define _CMp4AnalyzerConfiguration_H_

#include <string>

using namespace std;

class CMp4AnalyzerConfiguration {
public:
	CMp4AnalyzerConfiguration();
	virtual ~CMp4AnalyzerConfiguration();

	static CMp4AnalyzerConfiguration* getInstance() {
		if(NULL == pInstance_) pInstance_ = new CMp4AnalyzerConfiguration();
		return pInstance_;
	}

	void destroyInstance() {
		if(NULL != pInstance_) {
			delete pInstance_;
			pInstance_ = NULL;
		}
	}

	bool loadConfiguration();
	bool parseConfiguration(char* configurationMessage);
	bool displayConfiguration();

	const string& getBasePath() const {
		return basePath_;
	}

	const string& getConfigurationPath() const {
		return configurationPath_;
	}

	const string& getLogLevel() const {
		return logLevel_;
	}

	const string& getLogType() const {
		return logType_;
	}

	int getMaxLogBackupIndex() const {
		return maxLogBackupIndex_;
	}

	int getMaxLogFileSize() const {
		return maxLogFileSize_;
	}

private :
	static CMp4AnalyzerConfiguration* pInstance_;
	string logLevel_;
	string logType_;
	int maxLogFileSize_;
	int maxLogBackupIndex_;

	string basePath_;
	string configurationPath_;
};

#endif /* _CMp4AnalyzerConfiguration_H_ */
