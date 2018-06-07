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
	GtkWidget* generateMp4InformationView();
	bool terminateMp4InformationView();
	static gboolean drawCallback(GtkWidget* widget, cairo_t* cr, gpointer data);

private:
	GtkWidget* scrolledWindow_ = nullptr;
};

#endif /* SRC_UI_CMP4INFORMATION_H_ */
