/*
 * CMp4Information.h
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#ifndef SRC_UI_CMP4INFORMATION_H_
#define SRC_UI_CMP4INFORMATION_H_

#include <gtk/gtk.h>

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

private:
	GtkWidget* scrolledWindow_ = nullptr;
	GtkWidget* box_ = nullptr;
};

#endif /* SRC_UI_CMP4INFORMATION_H_ */
