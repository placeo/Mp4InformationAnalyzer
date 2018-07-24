/*
 * CMp4Information.h
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#ifndef SRC_UI_CMP4INFORMATION_H_
#define SRC_UI_CMP4INFORMATION_H_

#include <gtk/gtk.h>
#include <string>

using namespace std;

class CMp4Information {
public:
	CMp4Information();
	virtual ~CMp4Information();
	GtkWidget* initializeMp4InformationView();
	GtkTreeModel* generateMp4InformationWindow();
	bool terminateMp4InformationView();
	static gboolean drawCallback(GtkWidget* widget, cairo_t* cr, gpointer data);

	GtkWidget* getScrolledWindow() {
		return scrolledWindow_;
	}

	void setScrolledWindow(GtkWidget* scrolledWindow) {
		scrolledWindow_ = scrolledWindow;
	}

	GtkWidget* getBox() {
		return box_;
	}

	void setBox(GtkWidget* box) {
		box_ = box;
	}

	GtkWidget* getDrawingArea() const {
		return drawingArea_;
	}

	void setDrawingArea(GtkWidget* drawingArea) {
		drawingArea_ = drawingArea;
	}

	GtkWidget* createInformationView(string atomName, long long int size, long long int offset);
	GtkTreeModel* createInformationModel(string atomName, long long int size, long long int offset);

private:
	GtkWidget* scrolledWindow_ = nullptr;
	GtkWidget* box_ = nullptr;
	GtkWidget* drawingArea_ = nullptr;
};

#endif /* SRC_UI_CMP4INFORMATION_H_ */
