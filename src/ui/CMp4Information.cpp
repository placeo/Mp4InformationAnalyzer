/*
 * CMp4Information.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#include <cairo/cairo.h>
#include "CMp4Information.h"
#include "CMp4AnalyzerLogger.h"
#include "xmp.h"

static const char* LogTag = "MP4_INFORMATION";

static void
oval_path (cairo_t *cr,
           double xc, double yc,
           double xr, double yr)
{
  cairo_save (cr);

  cairo_translate (cr, xc, yc);
  cairo_scale (cr, 1.0, yr / xr);
  cairo_move_to (cr, xr, 0.0);
  cairo_arc (cr,
         0, 0,
         xr,
         0, 2 * G_PI);
  cairo_close_path (cr);

  cairo_restore (cr);
}

CMp4Information::CMp4Information() {
	// TODO Auto-generated constructor stub

}

CMp4Information::~CMp4Information() {
	// TODO Auto-generated destructor stub
}

GtkWidget* CMp4Information::initializeMp4InformationView() {
	GtkWidget* label = nullptr;
	GdkPixbuf* logoPixBuf = nullptr;
	drawingArea_ = gtk_drawing_area_new();

	scrolledWindow_ = gtk_scrolled_window_new(NULL, NULL);

//	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow_), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	logoPixBuf = gdk_pixbuf_new_from_xpm_data((const gchar **)mp4InformationAnalyzerLogo);

	gtk_container_add(GTK_CONTAINER(scrolledWindow_), drawingArea_);
	label = gtk_label_new("Information");
	box_ = gtk_notebook_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(box_), scrolledWindow_, label);

	g_signal_connect(drawingArea_, "draw", G_CALLBACK(drawCallback), logoPixBuf);

	return box_;
}

bool CMp4Information::terminateMp4InformationView() {
	return true;
}

GtkTreeModel* CMp4Information::generateMp4InformationWindow() {

}

gboolean CMp4Information::drawCallback(GtkWidget* widget, cairo_t* cr, gpointer data) {
	cairo_surface_t* overlay;
	cairo_t* overlay_cr;
	GtkAllocation allocation;
	GdkPixbuf* logoPixBuf = (GdkPixbuf*)data;
	int width = gtk_widget_get_allocated_width(widget);
	int height = gtk_widget_get_allocated_height(widget);
	gtk_widget_get_allocation (widget, &allocation);

	overlay = cairo_surface_create_similar (cairo_get_target (cr),
					  CAIRO_CONTENT_COLOR_ALPHA,
					  width, height);

	cairo_rectangle(cr, 0, 0, width, height);
	cairo_set_source_rgb(cr, 1.0, 0.75, 0.0);
	cairo_fill(cr);

	overlay_cr = cairo_create(overlay);

	cairo_set_operator (overlay_cr, CAIRO_OPERATOR_OVER);
	gdk_cairo_set_source_pixbuf(overlay_cr, logoPixBuf, (allocation.width - gdk_pixbuf_get_width(logoPixBuf)) / 2, (allocation.height - gdk_pixbuf_get_height(logoPixBuf)) / 2);
	cairo_paint (overlay_cr);

	cairo_destroy (overlay_cr);

	cairo_set_source_surface (cr, overlay, 0, 0);
	cairo_paint (cr);

	cairo_surface_destroy (overlay);

	return FALSE;
}


GtkWidget* CMp4Information::createInformationView(string atomName, long long int size, long long int offset) {
	GtkCellRenderer     *renderer;
	GtkTreeModel        *model;
	GtkWidget           *view;

	view = gtk_tree_view_new ();

	/* --- Column #1 --- */

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
											   -1,
											   "First",
											   renderer,
											   "text", 0,
											   NULL);

	/* --- Column #2 --- */

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
											   -1,
											   "Second",
											   renderer,
											   "text", 1,
											   NULL);

	model = createInformationModel(atomName, size, offset);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), FALSE);
	gtk_tree_view_set_grid_lines(GTK_TREE_VIEW (view), GTK_TREE_VIEW_GRID_LINES_BOTH);
	gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

	/* The tree view has acquired its own reference to the
	*  model, so we can drop ours. That way the model will
	*  be freed automatically when the tree view is destroyed */

	g_object_unref (model);

	return view;
}

GtkTreeModel* CMp4Information::createInformationModel(string atomName, long long int size, long long int offset) {
	TestInfo(LogTag, "%s routine", __FUNCTION__);
	GtkListStore* boxInformationListStore;
	GtkTreeIter treeIter;
	boxInformationListStore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);


	gtk_list_store_append(boxInformationListStore, &treeIter);
	gtk_list_store_set(boxInformationListStore, &treeIter, 0, "atom type", 1, atomName.c_str(), -1);

	gtk_list_store_append(boxInformationListStore, &treeIter);
	gtk_list_store_set(boxInformationListStore, &treeIter, 0, "atom size", 1, to_string(size).c_str(), -1);

	gtk_list_store_append(boxInformationListStore, &treeIter);
	gtk_list_store_set(boxInformationListStore, &treeIter, 0, "atom offset", 1, to_string(offset).c_str(), -1);

	return GTK_TREE_MODEL(boxInformationListStore);
}
