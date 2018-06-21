/*
 * CMediaAnalysisManager.h
 *
 *  Created on: Jun 12, 2018
 *      Author: placeo
 */

#include <string>
#include <vector>

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
	bool extractFileName();
	bool retrieveLevel0Name();
	bool parseLevel0Name(const char* mediaInformation);

	string getFileName() const {
		return fileName_;
	}

	void setFileName(string fileName) {
		fileName_ = fileName;
	}

	string getReadJsonData() {
		return readJsonData_;
	}

	vector<string> getLevel0NameVector() const {
		return level0NameVector_;
	}

private:
	static CMediaAnalysisManager* pInstance_;
	string fileLocation_ = "";
	string fileName_ = "";
	string readJsonData_ = "";

	int	firstLayerCount_ = 0;
	vector<string> level0NameVector_;
};

#endif /* SRC_MEDIAANALYSIS_CMEDIAANALYSISMANAGER_H_ */
