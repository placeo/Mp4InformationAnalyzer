/*
 * CMp4Information.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#include "CMp4Information.h"

CMp4Information::CMp4Information() {
	// TODO Auto-generated constructor stub

}

CMp4Information::~CMp4Information() {
	// TODO Auto-generated destructor stub
}

GtkWidget* CMp4Information::generateMp4InformationView() {
	GtkWidget* box = nullptr;
	GtkWidget* label = nullptr;

	scrolledWindow_ = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow_), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	// gtk_container_add(GTK_CONTAINER(scrolledWindow_), structureTreeView_);

	label = gtk_label_new("Information");
	box = gtk_notebook_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(box), scrolledWindow_, label);

	return box;
}

bool CMp4Information::terminateMp4InformationView() {
	return true;
}
