/*
 * CMp4Information.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#include "CMp4Information.h"
#include "xmp.h"

CMp4Information::CMp4Information() {
	// TODO Auto-generated constructor stub

}

CMp4Information::~CMp4Information() {
	// TODO Auto-generated destructor stub
}

GtkWidget* CMp4Information::generateMp4InformationView() {
	GtkWidget* box = nullptr;
	GtkWidget* label = nullptr;
	GtkWidget* logoImage = nullptr;
	GdkPixbuf* sktLogoPixBuf = nullptr;
	scrolledWindow_ = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow_), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
//	logoImage = gtk_image_new_from_file("resource/MP4InformationAnalyzer.png");
//	logoImage = gtk_image_new_from_file("resource/SktLogo.png");
	sktLogoPixBuf = gdk_pixbuf_new_from_xpm_data((const gchar **)sktLogo);
	logoImage = gtk_image_new_from_pixbuf(sktLogoPixBuf);

//	gtk_widget_set_size_request(logoImage, 200, 100);
	//gtk_container_add(GTK_CONTAINER(scrolledWindow_), logoImage);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledWindow_), logoImage);
	label = gtk_label_new("Information");
	box = gtk_notebook_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(box), scrolledWindow_, label);

	return box;
}

bool CMp4Information::terminateMp4InformationView() {
	return true;
}
