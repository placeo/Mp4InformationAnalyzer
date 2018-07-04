/*
 * CMediaAnalysisManager.h
 *
 *  Created on: Jun 12, 2018
 *      Author: placeo
 */

#include <string>
// #include <vector>
#include <map>
#include <json-c/json.h>

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
	bool parseAtoms(const char* mediaInformation);
	bool parseChildAtoms(json_object* layerObject, int levelCount, string atomPath);

	string getFileName() const {
		return fileName_;
	}

	void setFileName(string fileName) {
		fileName_ = fileName;
	}

	string getReadJsonData() {
		return readJsonData_;
	}

	map<string, json_object*>& getAtomMap() {
		return atomMap_;
	}

//	vector<string> getLevel0NameVector() const {
//		return level0NameVector_;
//	}

private:
	static CMediaAnalysisManager* pInstance_;
	string fileLocation_ = "";
	string fileName_ = "";
	string readJsonData_ = "";

//	vector<string> level0NameVector_;
	json_object* fullAtomsJsonObject_ = nullptr;
	map<string, json_object*> atomMap_; // path string, json_object
};

#endif /* SRC_MEDIAANALYSIS_CMEDIAANALYSISMANAGER_H_ */
