/*
 * CMp4Structure.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#include "CMp4Structure.h"
#include "CMp4AnalyzerLogger.h"

static const char* LogTag = "MP4_STRUCTURE";

CMp4Structure::CMp4Structure() {
	// TODO Auto-generated constructor stub

}

CMp4Structure::~CMp4Structure() {
	// TODO Auto-generated destructor stub
}

GtkWidget* CMp4Structure::generateStructureTreeView() {
	GtkTreeSelection* treeSelection = nullptr;
	GtkTreeIter iter;
	GtkTreeIter childIter;
	GtkCellRenderer* titleCell = nullptr;
	GtkTreeViewColumn* titleColumn = nullptr;
	GtkWidget* box = nullptr;
	GtkWidget* label = nullptr;

	structureTreeModel_ = gtk_tree_store_new(1, G_TYPE_STRING);
	structureTreeView_ = gtk_tree_view_new();
	gtk_tree_view_set_model(GTK_TREE_VIEW(structureTreeView_), GTK_TREE_MODEL(structureTreeModel_));
	treeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(structureTreeView_));

	gtk_tree_selection_set_mode(GTK_TREE_SELECTION(treeSelection), GTK_SELECTION_BROWSE);
	gtk_widget_set_size_request(structureTreeView_, -1, -1);

	gtk_tree_store_append(GTK_TREE_STORE(structureTreeModel_), &iter, nullptr);
	gtk_tree_store_set(GTK_TREE_STORE(structureTreeModel_), &iter, 0, "first", -1);

	gtk_tree_store_append(GTK_TREE_STORE(structureTreeModel_), &childIter, &iter);
	gtk_tree_store_set(GTK_TREE_STORE(structureTreeModel_), &childIter, 0, "one", -1);
	gtk_tree_store_append(GTK_TREE_STORE(structureTreeModel_), &childIter, &iter);
	gtk_tree_store_set(GTK_TREE_STORE(structureTreeModel_), &childIter, 0, "two", -1);

	gtk_tree_store_append(GTK_TREE_STORE(structureTreeModel_), &iter, nullptr);
	gtk_tree_store_set(GTK_TREE_STORE(structureTreeModel_), &iter, 0, "second", -1);

	gtk_tree_store_append(GTK_TREE_STORE(structureTreeModel_), &childIter, &iter);
	gtk_tree_store_set(GTK_TREE_STORE(structureTreeModel_), &childIter, 0, "three", -1);
	gtk_tree_store_append(GTK_TREE_STORE(structureTreeModel_), &childIter, &iter);
	gtk_tree_store_set(GTK_TREE_STORE(structureTreeModel_), &childIter, 0, "four", -1);

	titleCell = gtk_cell_renderer_text_new();
	titleColumn = gtk_tree_view_column_new_with_attributes("Structure", titleCell, "text", 0, nullptr);
	gtk_tree_view_append_column(GTK_TREE_VIEW(structureTreeView_), GTK_TREE_VIEW_COLUMN(titleColumn));

	gtk_tree_model_get_iter_first(GTK_TREE_MODEL(structureTreeModel_), &iter);
	gtk_tree_selection_select_iter(GTK_TREE_SELECTION(treeSelection), &iter);

	g_signal_connect(treeSelection, "changed", G_CALLBACK(CMp4Structure::treeViewSelectionCallback), structureTreeModel_);
	g_signal_connect(structureTreeView_, "row_activated", G_CALLBACK(CMp4Structure::treeRowActivatedCallback), structureTreeModel_);

	gtk_tree_view_collapse_all(GTK_TREE_VIEW(structureTreeView_));
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(structureTreeView_), FALSE);

	scrolledWindow_ = gtk_scrolled_window_new(nullptr, nullptr);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow_), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolledWindow_), structureTreeView_);

	label = gtk_label_new("Structure");
	box = gtk_notebook_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(box), scrolledWindow_, label);

	gtk_widget_grab_focus(structureTreeView_);

	return box;
}

bool CMp4Structure::terminateStructureTreeView() {
	return true;
}

void CMp4Structure::treeViewSelectionCallback(GtkTreeSelection* treeSelection, GtkTreeModel* treeModel) {
	TestInfo(LogTag, "%s routine", __FUNCTION__);
}

void CMp4Structure::treeRowActivatedCallback(GtkTreeView* treeView, GtkTreePath* treePath, GtkTreeViewColumn* column) {
	TestInfo(LogTag, "%s routine", __FUNCTION__);
}
