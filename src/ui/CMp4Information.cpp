/*
 * CMp4Information.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#include <cairo/cairo.h>
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
	GdkPixbuf* logoPixBuf = nullptr;
	GtkWidget* drawingArea = gtk_drawing_area_new();

	scrolledWindow_ = gtk_scrolled_window_new(NULL, NULL);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow_), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	logoPixBuf = gdk_pixbuf_new_from_xpm_data((const gchar **)mp4InformationAnalyzerLogo);

	g_signal_connect(drawingArea, "expose-event", G_CALLBACK(alphaWindowExpose), logoPixBuf);

	//gtk_container_add(GTK_CONTAINER(scrolledWindow_), drawingArea);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledWindow_), drawingArea);
	label = gtk_label_new("Information");
	box = gtk_notebook_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(box), scrolledWindow_, label);

	return box;
}

bool CMp4Information::terminateMp4InformationView() {
	return true;
}

gboolean CMp4Information::alphaWindowExpose(GtkWidget* widget, GdkEventExpose* expose, gpointer data) {
	cairo_t *cr;
	cairo_pattern_t *pattern;
	int radius;
	GdkPixbuf* logoPixBuf = (GdkPixbuf*)data;

	cr = gdk_cairo_create (widget->window);

	radius = MIN (widget->allocation.width, widget->allocation.height) / 2;
	pattern = cairo_pattern_create_radial (widget->allocation.width / 2,
		widget->allocation.height / 2,
		0.0,
		widget->allocation.width / 2,
		widget->allocation.height / 2,
		radius * 1.33);

	if (gdk_screen_get_rgba_colormap (gtk_widget_get_screen (widget)) && gtk_widget_is_composited (widget))
		cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0); /* transparent */
	else
		cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); /* opaque white */

	cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	cairo_paint (cr);

	cairo_pattern_add_color_stop_rgba (pattern, 0.0, 1.0, 0.75, 0.0, 1.0); /* solid orange */
//	cairo_pattern_add_color_stop_rgba (pattern, 1.0, 1.0, 0.75, 0.0, 0.0); /* transparent orange */

	cairo_set_source (cr, pattern);
	cairo_pattern_destroy (pattern);

	cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
	cairo_paint (cr);

	cairo_destroy (cr);

	gdk_draw_pixbuf(widget->window, gtk_widget_get_style (widget)->black_gc,
			logoPixBuf,
		0, 0,
		(widget->allocation.width - gdk_pixbuf_get_width(logoPixBuf)) / 2,
		(widget->allocation.height - gdk_pixbuf_get_height(logoPixBuf)) / 2,
		gdk_pixbuf_get_width(logoPixBuf),
		gdk_pixbuf_get_height(logoPixBuf),
		GDK_RGB_DITHER_NORMAL,
		0, 0);
	return FALSE;
}
