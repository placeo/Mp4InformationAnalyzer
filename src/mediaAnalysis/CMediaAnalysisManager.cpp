/*
 * CMediaAnalysisManager.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: placeo
 */

#include <json-c/json.h>
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
	level0NameVector_.clear();
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
	char* configurationMessage = NULL;

	filePointer = fopen(Mp4InformationFile, "r");
	if(NULL == filePointer) {
		TestError(LogTag, "Mp4 information file open error");
		return false;
	}

	fseek(filePointer, 0, SEEK_END);
	fileSize = ftell(filePointer);

	configurationMessage = (char*)calloc(fileSize+1, 1);
	if(NULL == configurationMessage) {
		TestError(LogTag, "Mp4 information memory allocation error");
		if(0 != fclose(filePointer)) {
			TestError(LogTag, "Mp4 information file closing error");
		}
		return false;
	}

	rewind(filePointer);
	returnValue = fread(configurationMessage, 1, fileSize, filePointer);
	if(returnValue <= 0) {
		TestError(LogTag, "Mp4 information read error");
		if(0 != fclose(filePointer)) {
			TestError(LogTag, "Mp4 information file closing error");
		}
		return false;
	}

	if(false == parseLevel0Name(configurationMessage)) {
		TestError(LogTag, "Mp4 information file parsing error");
		if(0 != fclose(filePointer)) {
			TestError(LogTag, "Mp4 information file closing error");
		}
		free(configurationMessage);
		return false;
	}

	free(configurationMessage);

	if(0 != fclose(filePointer)) {
		TestError(LogTag, "Mp4 information file closing error");
		return false;
	}

	return true;
}

bool CMediaAnalysisManager::parseLevel0Name(const char* mediaInformation) {
	json_object* jsonData = json_tokener_parse(mediaInformation);
	if(NULL == jsonData) {
		TestError(LogTag, "Failed to tokenize json message");
		json_object_put(jsonData);
		return false;
	}
	else {
		firstLayerCount_ = json_object_array_length(jsonData);
		for(int i=0; i < firstLayerCount_; i++) {
			json_object* jsonFirstLayerObject = nullptr;
			json_object* jsonFirstLayerName = nullptr;

			string Level0NameString = "";

			jsonFirstLayerObject = json_object_array_get_idx(jsonData, i);

			if(0 == json_object_object_get_ex(jsonFirstLayerObject, "name", &jsonFirstLayerName)) {
				TestError(LogTag, "Failed to get first layer name");
				return false;
			}
			else {
				Level0NameString = string(json_object_get_string(jsonFirstLayerName));
				level0NameVector_.push_back(Level0NameString);
				TestInfo(LogTag, "Level 0 name : %s", Level0NameString.data());
			}
		}
	}
	return true;
}
