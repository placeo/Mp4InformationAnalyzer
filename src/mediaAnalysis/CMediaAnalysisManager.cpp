/*
 * CMediaAnalysisManager.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: placeo
 */

#include "Ap4.h"
#include "CMediaAnalysisManager.h"
#include "CMp4AnalyzerLogger.h"
#include <json-c/json.h>

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

//	if(false == parseLevel0Name(mp4JsonInformation)) {
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
	json_object* jsonData = json_tokener_parse(mediaInformation);
	if(NULL == jsonData) {
		TestError(LogTag, "Failed to tokenize json message");
		json_object_put(jsonData);
		return false;
	}
	else {
		int firstLayerCount = json_object_array_length(jsonData);
		for(int i=0; i < firstLayerCount; i++) {
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

bool CMediaAnalysisManager::parseAtoms(const char* mediaInformation) {
	json_object* jsonData = json_tokener_parse(mediaInformation);
	if(NULL == jsonData) {
		TestError(LogTag, "Failed to tokenize json message");
		json_object_put(jsonData);
		return false;
	}
	else {
		int firstLayerCount = json_object_array_length(jsonData);
		for(int i=0; i < firstLayerCount; i++) {
			json_object* jsonFirstLayerObject = nullptr;
			json_object* jsonFirstLayerNameObject = nullptr;
			json_object* jsonFirstLayerChildrenObject = nullptr;

			string Level0NameString = "";

			jsonFirstLayerObject = json_object_array_get_idx(jsonData, i);

			if(0 == json_object_object_get_ex(jsonFirstLayerObject, "name", &jsonFirstLayerNameObject)) {
				TestError(LogTag, "Failed to get first layer name");
				return false;
			}
			else {
				Level0NameString = string(json_object_get_string(jsonFirstLayerNameObject));
				level0NameVector_.push_back(Level0NameString);
				TestInfo(LogTag, "Level 0 name : %s", Level0NameString.data());
			}

			if(0 == json_object_object_get_ex(jsonFirstLayerObject, "children", &jsonFirstLayerChildrenObject)) {
				TestWarning(LogTag, "first, No child exists");
			}
			else {
				int secondLayerCount = json_object_array_length(jsonFirstLayerChildrenObject);
				json_object* jsonSecondLayerObject = nullptr;
				json_object* jsonSecondLayerNameObject = nullptr;
				json_object* jsonSecondLayerChildrenObject = nullptr;

				for(int i=0; i < secondLayerCount; i++) {

					string Level1NameString = "";

					jsonSecondLayerObject = json_object_array_get_idx(jsonFirstLayerChildrenObject, i);

					if(0 == json_object_object_get_ex(jsonSecondLayerObject, "name", &jsonSecondLayerNameObject)) {
						TestError(LogTag, "Failed to get second layer name");
						return false;
					}
					else {
						Level1NameString = string(json_object_get_string(jsonSecondLayerNameObject));
						// Insert to tree view
						TestInfo(LogTag, "Level 1 name : %s", Level1NameString.data());
					}

					if(0 == json_object_object_get_ex(jsonSecondLayerObject, "children", &jsonSecondLayerChildrenObject)) {
						TestWarning(LogTag, "second, No child exists");
					}
					else {
						int thirdLayerCount = json_object_array_length(jsonSecondLayerChildrenObject);
						json_object* jsonThirdLayerObject = nullptr;
						json_object* jsonThirdLayerNameObject = nullptr;
						json_object* jsonThirdLayerChildrenObject = nullptr;

						for(int i=0; i < thirdLayerCount; i++) {
							json_object* jsonThirdLayerObject = nullptr;
							json_object* jsonThirdLayerNameObject = nullptr;

							string Level2NameString = "";

							jsonThirdLayerObject = json_object_array_get_idx(jsonSecondLayerChildrenObject, i);

							if(0 == json_object_object_get_ex(jsonThirdLayerObject, "name", &jsonThirdLayerNameObject)) {
								TestError(LogTag, "Failed to get third layer name");
								return false;
							}
							else {
								Level2NameString = string(json_object_get_string(jsonThirdLayerNameObject));
								// Insert to tree view
								TestInfo(LogTag, "Level 2 name : %s", Level2NameString.data());
							}

							if(0 == json_object_object_get_ex(jsonThirdLayerObject, "children", &jsonThirdLayerChildrenObject)) {
								TestWarning(LogTag, "third, No child exists");
							}
							else {
								int forthLayerCount = json_object_array_length(jsonThirdLayerChildrenObject);
								json_object* jsonForthLayerObject = nullptr;
								json_object* jsonForthLayerNameObject = nullptr;
								json_object* jsonForthLayerChildrenObject = nullptr;

								for(int i=0; i < forthLayerCount; i++) {
									json_object* jsonForthLayerObject = nullptr;
									json_object* jsonForthLayerNameObject = nullptr;

									string Level3NameString = "";

									jsonForthLayerObject = json_object_array_get_idx(jsonThirdLayerChildrenObject, i);

									if(0 == json_object_object_get_ex(jsonForthLayerObject, "name", &jsonForthLayerNameObject)) {
										TestError(LogTag, "Failed to get forth layer name");
										return false;
									}
									else {
										Level3NameString = string(json_object_get_string(jsonForthLayerNameObject));
										// Insert to tree view
										TestInfo(LogTag, "Level 3 name : %s", Level3NameString.data());
									}

									if(0 == json_object_object_get_ex(jsonForthLayerObject, "children", &jsonForthLayerChildrenObject)) {
										TestWarning(LogTag, "forth, No child exists");
									}
									else {
										int fifthLayerCount = json_object_array_length(jsonForthLayerChildrenObject);
										json_object* jsonFifthLayerObject = nullptr;
										json_object* jsonFifthLayerNameObject = nullptr;
										json_object* jsonFifthLayerChildrenObject = nullptr;

										for(int i=0; i < fifthLayerCount; i++) {
											json_object* jsonFifthLayerObject = nullptr;
											json_object* jsonFifthLayerNameObject = nullptr;

											string Level4NameString = "";

											jsonFifthLayerObject = json_object_array_get_idx(jsonForthLayerChildrenObject, i);

											if(0 == json_object_object_get_ex(jsonFifthLayerObject, "name", &jsonFifthLayerNameObject)) {
												TestError(LogTag, "Failed to get fifth layer name");
												return false;
											}
											else {
												Level4NameString = string(json_object_get_string(jsonFifthLayerNameObject));
												// Insert to tree view
												TestInfo(LogTag, "Level 4 name : %s", Level4NameString.data());
											}

											if(0 == json_object_object_get_ex(jsonFifthLayerObject, "children", &jsonFifthLayerChildrenObject)) {
												TestWarning(LogTag, "fifth, No child exists");
											}
											else {
												int sixthLayerCount = json_object_array_length(jsonFifthLayerChildrenObject);
												json_object* jsonSixthLayerObject = nullptr;
												json_object* jsonSixthLayerNameObject = nullptr;
												json_object* jsonSixthLayerChildrenObject = nullptr;

												for(int i=0; i < sixthLayerCount; i++) {
													json_object* jsonSixthLayerObject = nullptr;
													json_object* jsonSixthLayerNameObject = nullptr;

													string Level5NameString = "";

													jsonSixthLayerObject = json_object_array_get_idx(jsonFifthLayerChildrenObject, i);

													if(0 == json_object_object_get_ex(jsonSixthLayerObject, "name", &jsonSixthLayerNameObject)) {
														TestError(LogTag, "Failed to get sixth layer name");
														return false;
													}
													else {
														Level5NameString = string(json_object_get_string(jsonSixthLayerNameObject));
														// Insert to tree view
														TestInfo(LogTag, "Level 5 name : %s", Level5NameString.data());
													}

													if(0 == json_object_object_get_ex(jsonSixthLayerObject, "children", &jsonSixthLayerChildrenObject)) {
														TestWarning(LogTag, "sixth, No child exists");
													}
													else {
														int seventhLayerCount = json_object_array_length(jsonSixthLayerChildrenObject);
														json_object* jsonSeventhLayerObject = nullptr;
														json_object* jsonSeventhLayerNameObject = nullptr;
														json_object* jsonSeventhLayerChildrenObject = nullptr;

														for(int i=0; i < seventhLayerCount; i++) {
															json_object* jsonSeventhLayerObject = nullptr;
															json_object* jsonSeventhLayerNameObject = nullptr;

															string Level6NameString = "";

															jsonSeventhLayerObject = json_object_array_get_idx(jsonSixthLayerChildrenObject, i);

															if(0 == json_object_object_get_ex(jsonSeventhLayerObject, "name", &jsonSeventhLayerNameObject)) {
																TestError(LogTag, "Failed to get seventh layer name");
																return false;
															}
															else {
																Level6NameString = string(json_object_get_string(jsonSeventhLayerNameObject));
																// Insert to tree view
																TestInfo(LogTag, "Level 6 name : %s", Level6NameString.data());
															}

															if(0 == json_object_object_get_ex(jsonSeventhLayerObject, "children", &jsonSeventhLayerChildrenObject)) {
																TestWarning(LogTag, "seventh, No child exists");
															}
															else {
																int eighthLayerCount = json_object_array_length(jsonSeventhLayerChildrenObject);
																json_object* jsonEighthLayerObject = nullptr;
																json_object* jsonEighthLayerNameObject = nullptr;
																json_object* jsonEighthLayerChildrenObject = nullptr;

																for(int i=0; i < eighthLayerCount; i++) {
																	json_object* jsonEighthLayerObject = nullptr;
																	json_object* jsonEighthLayerNameObject = nullptr;

																	string Level7NameString = "";

																	jsonEighthLayerObject = json_object_array_get_idx(jsonSeventhLayerChildrenObject, i);

																	if(0 == json_object_object_get_ex(jsonEighthLayerObject, "name", &jsonEighthLayerNameObject)) {
																		TestError(LogTag, "Failed to get eighth layer name");
																		return false;
																	}
																	else {
																		Level7NameString = string(json_object_get_string(jsonEighthLayerNameObject));
																		// Insert to tree view
																		TestInfo(LogTag, "Level 7 name : %s", Level7NameString.data());
																	}

																	if(0 == json_object_object_get_ex(jsonEighthLayerObject, "children", &jsonEighthLayerChildrenObject)) {
																		TestWarning(LogTag, "eighth, No child exists");
																	}
																	else {
																		int ninthLayerCount = json_object_array_length(jsonEighthLayerChildrenObject);
																		json_object* jsonNinthLayerObject = nullptr;
																		json_object* jsonNinthLayerNameObject = nullptr;
																		json_object* jsonNinthLayerChildrenObject = nullptr;

																		for(int i=0; i < ninthLayerCount; i++) {
																			json_object* jsonNinthLayerObject = nullptr;
																			json_object* jsonNinthLayerNameObject = nullptr;

																			string Level8NameString = "";

																			jsonNinthLayerObject = json_object_array_get_idx(jsonEighthLayerChildrenObject, i);

																			if(0 == json_object_object_get_ex(jsonNinthLayerObject, "name", &jsonNinthLayerNameObject)) {
																				TestError(LogTag, "Failed to get ninth layer name");
																				return false;
																			}
																			else {
																				Level8NameString = string(json_object_get_string(jsonNinthLayerNameObject));
																				// Insert to tree view
																				TestInfo(LogTag, "Level 8 name : %s", Level8NameString.data());
																			}

																			if(0 == json_object_object_get_ex(jsonNinthLayerObject, "children", &jsonNinthLayerChildrenObject)) {
																				TestWarning(LogTag, "ninth, No child exists");
																			}
																			else {
																				int tenthLayerCount = json_object_array_length(jsonNinthLayerChildrenObject);
																				json_object* jsonTenthLayerObject = nullptr;
																				json_object* jsonTenthLayerNameObject = nullptr;
																				json_object* jsonTenthLayerChildrenObject = nullptr;

																				for(int i=0; i < tenthLayerCount; i++) {
																					json_object* jsonTenthLayerObject = nullptr;
																					json_object* jsonTenthLayerNameObject = nullptr;

																					string Level9NameString = "";

																					jsonTenthLayerObject = json_object_array_get_idx(jsonNinthLayerChildrenObject, i);

																					if(0 == json_object_object_get_ex(jsonTenthLayerObject, "name", &jsonTenthLayerNameObject)) {
																						TestError(LogTag, "Failed to get tenth layer name");
																						return false;
																					}
																					else {
																						Level9NameString = string(json_object_get_string(jsonTenthLayerNameObject));
																						// Insert to tree view
																						TestInfo(LogTag, "Level 9 name : %s", Level9NameString.data());
																					}

																					if(0 == json_object_object_get_ex(jsonTenthLayerObject, "children", &jsonTenthLayerChildrenObject)) {
																						TestWarning(LogTag, "tenth, No child exists");
																					}
																					else {
																						int eleventhLayerCount = json_object_array_length(jsonTenthLayerChildrenObject);
																						json_object* jsonEleventhLayerObject = nullptr;
																						json_object* jsonEleventhLayerNameObject = nullptr;
																						json_object* jsonEleventhLayerChildrenObject = nullptr;

																						for(int i=0; i < eleventhLayerCount; i++) {
																							json_object* jsonEleventhLayerObject = nullptr;
																							json_object* jsonEleventhLayerNameObject = nullptr;

																							string Level10NameString = "";

																							jsonEleventhLayerObject = json_object_array_get_idx(jsonTenthLayerChildrenObject, i);

																							if(0 == json_object_object_get_ex(jsonEleventhLayerObject, "name", &jsonEleventhLayerNameObject)) {
																								TestError(LogTag, "Failed to get eleventh layer name");
																								return false;
																							}
																							else {
																								Level10NameString = string(json_object_get_string(jsonEleventhLayerNameObject));
																								// Insert to tree view
																								TestInfo(LogTag, "Level 10 name : %s", Level10NameString.data());
																							}
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return true;
}
