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

CMediaAnalysisManager::CMediaAnalysisManager() {
	// TODO Auto-generated constructor stub

}

CMediaAnalysisManager::~CMediaAnalysisManager() {
	// TODO Auto-generated destructor stub
}

bool CMediaAnalysisManager::generateMp4AnalysisJson() {
	// init the variables
	AP4_ByteStream* input = nullptr;
	// AP4_ByteStream* output = nullptr;
	AP4_MemoryByteStream* outputByteStream = nullptr;
	AP4_AtomInspector* inspector = NULL;
	AP4_Result result;
	unsigned long long int readDataSize = 0;

	result = AP4_FileByteStream::Create(fileLocation_.data(), AP4_FileByteStream::STREAM_MODE_READ, input);
	outputByteStream = new AP4_MemoryByteStream();
	outputByteStream->AddReference();

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

//	AP4_FileByteStream::Create("-stdout", AP4_FileByteStream::STREAM_MODE_WRITE, output);
//	AP4_FileByteStream::Create("jsonResult", AP4_FileByteStream::STREAM_MODE_WRITE, output);

//	inspector = new AP4_JsonInspector(*output);
	inspector = new AP4_JsonInspector(*outputByteStream);

	inspector->SetVerbosity(3);

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

	readJsonData_ = string(reinterpret_cast<char*>(const_cast<unsigned char*>(outputByteStream->GetData())));

	readJsonData_.resize(outputByteStream->GetDataSize());
	readJsonData_ += readJsonData_ + "\n]\n";
	TestInfo(LogTag, "Json Dump :\n%s\n", readJsonData_.data());

	if (input) input->Release();
	delete inspector;
	outputByteStream->Release();
//	delete outputByteStream;

	return true;
}
