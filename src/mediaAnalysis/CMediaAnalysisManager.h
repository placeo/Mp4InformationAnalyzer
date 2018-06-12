/*
 * CMediaAnalysisManager.h
 *
 *  Created on: Jun 12, 2018
 *      Author: placeo
 */

#include <string>

#ifndef SRC_MEDIAANALYSIS_CMEDIAANALYSISMANAGER_H_
#define SRC_MEDIAANALYSIS_CMEDIAANALYSISMANAGER_H_

using namespace std;

class CMediaAnalysisManager {
public:
	CMediaAnalysisManager();
	virtual ~CMediaAnalysisManager();

	static CMediaAnalysisManager* getInstance() {
		if(NULL == pInstance_) pInstance_ = new CMediaAnalysisManager();
		return pInstance_;
	}

	void destroyInstance() {
		if(pInstance_) {
			delete pInstance_;
			pInstance_ = NULL;
		}
	}

	string getFileLocation() const {
		return fileLocation_;
	}

	void setFileLocation(string fileLocation) {
		fileLocation_ = fileLocation;
	}

	bool generateMp4AnalysisJson();

private:
	static CMediaAnalysisManager* pInstance_;
	string fileLocation_ = "";
	string readJsonData_ = "";
};

#endif /* SRC_MEDIAANALYSIS_CMEDIAANALYSISMANAGER_H_ */
