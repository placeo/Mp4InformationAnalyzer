/*
 * CMp4Information.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#include <cairo/cairo.h>
#include "CMp4Information.h"
#include "xmp.h"

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

GtkWidget* CMp4Information::generateMp4InformationView() {
	GtkWidget* box = nullptr;
	GtkWidget* label = nullptr;
	GdkPixbuf* logoPixBuf = nullptr;
	GtkWidget* drawingArea = gtk_drawing_area_new();

	scrolledWindow_ = gtk_scrolled_window_new(NULL, NULL);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow_), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	logoPixBuf = gdk_pixbuf_new_from_xpm_data((const gchar **)mp4InformationAnalyzerLogo);

	gtk_container_add(GTK_CONTAINER(scrolledWindow_), drawingArea);
	label = gtk_label_new("Information");
	box = gtk_notebook_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(box), scrolledWindow_, label);

	g_signal_connect(drawingArea, "draw", G_CALLBACK(drawCallback), logoPixBuf);

	return box;
}

bool CMp4Information::terminateMp4InformationView() {
	return true;
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
