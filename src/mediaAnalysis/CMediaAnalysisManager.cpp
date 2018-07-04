/*
 * CMediaAnalysisManager.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: placeo
 */

#include "Ap4.h"
#include "CMediaAnalysisManager.h"
#include "CMp4AnalyzerLogger.h"

CMediaAnalysisManager* CMediaAnalysisManager::pInstance_ = NULL;
static const char* LogTag = "MEDIA_ANALYSIS_MANAGER";
static const char* Mp4InformationFile = "./mp4information.json";

CMediaAnalysisManager::CMediaAnalysisManager() {
	// TODO Auto-generated constructor stub

}

CMediaAnalysisManager::~CMediaAnalysisManager() {
	// level0NameVector_.clear();
	// TODO Auto-generated destructor stub
}

bool CMediaAnalysisManager::generateMp4AnalysisJson() {
	// init the variables
	AP4_ByteStream* input = nullptr;
	AP4_ByteStream* output = nullptr;
	AP4_AtomInspector* inspector = NULL;
	AP4_Result result;
	unsigned long long int readDataSize = 0;

	result = AP4_FileByteStream::Create(fileLocation_.data(), AP4_FileByteStream::STREAM_MODE_READ, input);

	if(AP4_FAILED(result)) {
		TestError(LogTag, "Cannot open input, result : %d", result);
		return false;
	}

	if(input == NULL) {
		TestError(LogTag, "No input specified");
		return false;
	}

	result = input->GetSize(readDataSize);

	if(AP4_FAILED(result)) {
		TestError(LogTag, "Cannot get data size, result : %d", result);
		return false;
	}

	TestInfo(LogTag, "read data size : %d", readDataSize);

	AP4_FileByteStream::Create(Mp4InformationFile, AP4_FileByteStream::STREAM_MODE_WRITE, output);

	inspector = new AP4_JsonInspector(*output);

	inspector->SetVerbosity(4);

	AP4_Atom* atom;
	AP4_DefaultAtomFactory atom_factory;
	string jsonString = "";
	while (atom_factory.CreateAtomFromStream(*input, atom) == AP4_SUCCESS) {
		// remember the current stream position because the Inspect method
		// may read from the stream (there may be stream references in some
		// of the atoms
		AP4_Position position;
		input->Tell(position);

		// inspect the atom
		atom->Inspect(*inspector);

		// restore the previous stream position
		input->Seek(position);

		// destroy the atom
		delete atom;
	}

	if(output) output->Release();

	if (input) input->Release();
	delete inspector;

	return true;
}

bool CMediaAnalysisManager::extractFileName() {
	string key = "/";
	size_t found = fileLocation_.rfind(key);
	fileName_ = fileLocation_.substr(found + 1);
	TestInfo(LogTag, "File Name : %s", fileName_.data());
	return true;
}

bool CMediaAnalysisManager::retrieveLevel0Name() {
	FILE* filePointer = NULL;
	long int fileSize = 0;
	int returnValue = 0;
	char* mp4JsonInformation = NULL;

	filePointer = fopen(Mp4InformationFile, "r");
	if(NULL == filePointer) {
		TestError(LogTag, "Mp4 information file open error");
		return false;
	}

	fseek(filePointer, 0, SEEK_END);
	fileSize = ftell(filePointer);

	mp4JsonInformation = (char*)calloc(fileSize+1, 1);
	if(NULL == mp4JsonInformation) {
		TestError(LogTag, "Mp4 information memory allocation error");
		if(0 != fclose(filePointer)) {
			TestError(LogTag, "Mp4 information file closing error");
		}
		return false;
	}

	rewind(filePointer);
	returnValue = fread(mp4JsonInformation, 1, fileSize, filePointer);
	if(returnValue <= 0) {
		TestError(LogTag, "Mp4 information read error");
		if(0 != fclose(filePointer)) {
			TestError(LogTag, "Mp4 information file closing error");
		}
		return false;
	}

	if(false == parseAtoms(mp4JsonInformation)) {
		TestError(LogTag, "Mp4 information file parsing error");
		if(0 != fclose(filePointer)) {
			TestError(LogTag, "Mp4 information file closing error");
		}
		free(mp4JsonInformation);
		return false;
	}

	free(mp4JsonInformation);

	if(0 != fclose(filePointer)) {
		TestError(LogTag, "Mp4 information file closing error");
		return false;
	}

	return true;
}

bool CMediaAnalysisManager::parseLevel0Name(const char* mediaInformation) {
	fullAtomsJsonObject_ = json_tokener_parse(mediaInformation);
	if(NULL == fullAtomsJsonObject_) {
		TestError(LogTag, "Failed to tokenize json message");
		json_object_put(fullAtomsJsonObject_);
		return false;
	}
	else {
		int firstLayerCount = json_object_array_length(fullAtomsJsonObject_);
		for(int i=0; i < firstLayerCount; i++) {
			json_object* jsonFirstLayerObject = nullptr;
			json_object* jsonFirstLayerName = nullptr;

			string Level0NameString = "";

			jsonFirstLayerObject = json_object_array_get_idx(fullAtomsJsonObject_, i);

			if(0 == json_object_object_get_ex(jsonFirstLayerObject, "name", &jsonFirstLayerName)) {
				TestError(LogTag, "Failed to get first layer name");
				return false;
			}
			else {
				Level0NameString = string(json_object_get_string(jsonFirstLayerName));
//				level0NameVector_.push_back(Level0NameString);
//				TestInfo(LogTag, "Level 0 name : %s", Level0NameString.data());
			}
		}
	}
	return true;
}

bool CMediaAnalysisManager::parseChildAtoms(json_object* layerObject, int levelCount, string atomPath) {
	json_object* jsonChildrenObject = nullptr;
	if(0 == json_object_object_get_ex(layerObject, "children", &jsonChildrenObject)) {
		TestWarning(LogTag, "No child exists");
		return false;
	}
	else {
		int layerCount = json_object_array_length(jsonChildrenObject);
		json_object* jsonLayerChildrenObject = nullptr;
		levelCount++;
		for(int i=0; i < layerCount; i++) {
			json_object* jsonChildLayerObject = nullptr;
			json_object* jsonChildLayerNameObject = nullptr;

			string atomNameString = "";
			string revisedAtomPath = atomPath + ":" + to_string(i);

			jsonChildLayerObject = json_object_array_get_idx(jsonChildrenObject, i);

			if(0 == json_object_object_get_ex(jsonChildLayerObject, "name", &jsonChildLayerNameObject)) {
				TestError(LogTag, "Failed to get layer name");
				return false;
			}
			else {
				atomNameString = string(json_object_get_string(jsonChildLayerNameObject));
				// Insert to tree view
				TestInfo(LogTag, "Level : %d, path : %s, name : %s", levelCount, revisedAtomPath.c_str(), atomNameString.data());
			}
			parseChildAtoms(jsonChildLayerObject, levelCount, revisedAtomPath);
		}
	}
	return true;
}

bool CMediaAnalysisManager::parseAtoms(const char* mediaInformation) {
	fullAtomsJsonObject_ = json_tokener_parse(mediaInformation);
	int levelCount = 1;
	string atomPath = "0";
	if(NULL == fullAtomsJsonObject_) {
		TestError(LogTag, "Failed to tokenize json message");
		json_object_put(fullAtomsJsonObject_);
		return false;
	}
	else {
		int firstLayerCount = json_object_array_length(fullAtomsJsonObject_);
		for(int i=0; i < firstLayerCount; i++) {
			json_object* jsonFirstLayerObject = nullptr;
			json_object* jsonFirstLayerNameObject = nullptr;
			json_object* jsonFirstLayerChildrenObject = nullptr;

			string atomNameString = "";
			string revisedAtomPath = atomPath + ":" + to_string(i);

			jsonFirstLayerObject = json_object_array_get_idx(fullAtomsJsonObject_, i);

			if(0 == json_object_object_get_ex(jsonFirstLayerObject, "name", &jsonFirstLayerNameObject)) {
				TestError(LogTag, "Failed to get first layer name");
				return false;
			}
			else {
				atomNameString = string(json_object_get_string(jsonFirstLayerNameObject));
//				level0NameVector_.push_back(atomNameString);
				TestInfo(LogTag, "Level : %d, path : %s, name : %s", levelCount, revisedAtomPath.c_str(), atomNameString.data());
			}
			parseChildAtoms(jsonFirstLayerObject, levelCount, revisedAtomPath);
		}
	}
	return true;
}
