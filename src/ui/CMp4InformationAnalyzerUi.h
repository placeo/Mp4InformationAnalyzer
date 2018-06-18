/*
 * CMp4AnalyzerUi.h
 *
 *  Created on: May 29, 2018
 *      Author: placeo
 */

#ifndef SRC_UI_CMP4INFORMATIONANALYZERUI_H_
#define SRC_UI_CMP4INFORMATIONANALYZERUI_H_

#include <gtk/gtk.h>
#include <string>
#include "CMp4Structure.h"
#include "CMp4Information.h"
#include "CMp4Binary.h"

using namespace std;

class CMp4AnalyzerUi {
public:
	CMp4AnalyzerUi();
	virtual ~CMp4AnalyzerUi();

	static void createMainUi(CMp4AnalyzerUi* mp4AnalyzerUi);

	static GtkWidget* createMenuBar(CMp4AnalyzerUi* mp4AnalyzerUi);
	static GtkWidget* createFileMenu(CMp4AnalyzerUi* mp4AnalyzerUi);
	static GtkWidget* createHelpMenu(CMp4AnalyzerUi* mp4AnalyzerUi);
	static GtkWidget* createToolBar(CMp4AnalyzerUi* mp4AnalyzerUi);

	static void deleteEventCallback(GtkWidget* widget, GdkEvent* event, gpointer data);
	static void fileOpenCallback(gpointer userData);
	static void fileCloseCallback(gpointer userData);
	static void quitCallback(gpointer userData);
	static void aboutCallback(gpointer userData);

	static void toolBarFileOpenCallback(GtkWidget* widget, gpointer userData);
	static void toolBarFileCloseCallback(GtkWidget* widget, gpointer userData);

	static bool loadFile(const char* openFileName, gpointer data);

	CMp4Binary* getMp4Binary() const {
		return mp4Binary_;
	}

	CMp4Information* getMp4Information() const {
		return mp4Information_;
	}

	CMp4Structure* getMp4Structure() const {
		return mp4Structure_;
	}

private:
	GtkWidget* mainWindow_ = nullptr;
	GtkAccelGroup* accelGroup_ = nullptr;
	string fileName_ = "";
	FILE* filePointer_ = nullptr;
	long int fileSize_ = 0;

	CMp4Structure* mp4Structure_ = nullptr;
	CMp4Information* mp4Information_ = nullptr;
	CMp4Binary* mp4Binary_ = nullptr;

};

#endif /* SRC_UI_CMP4INFORMATIONANALYZERUI_H_ */
