/*
 * CMp4Structure.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#include "CMp4Structure.h"
#include "CMp4AnalyzerLogger.h"
#include "CMediaAnalysisManager.h"

static const char* LogTag = "MP4_STRUCTURE";

enum {
	ATOME_NAME,
	STYLE_COLUMN
};

CMp4Structure::CMp4Structure() {
	// TODO Auto-generated constructor stub

}

CMp4Structure::~CMp4Structure() {
	// TODO Auto-generated destructor stub
}

bool CMp4Structure::generateStructureTreeView() {
	GtkTreeIter iter;
	GtkTreeIter childIter;

	gtk_tree_model_get_iter_first(GTK_TREE_MODEL(structureTreeStore_), &iter);
//	gtk_tree_selection_select_iter(GTK_TREE_SELECTION(treeSelection_), &iter);

	g_object_set(cellRenderer_, "weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);

	gtk_tree_store_append(GTK_TREE_STORE(structureTreeStore_), &iter, nullptr);
	gtk_tree_store_set(GTK_TREE_STORE(structureTreeStore_), &iter, ATOME_NAME, CMediaAnalysisManager::getInstance()->getFileName().data(), -1);
//	gtk_tree_store_set(GTK_TREE_STORE(structureTreeStore_), &iter, "weight", PANGO_WEIGHT_, -1);

//	g_object_set(cellRenderer_, "weight", PANGO_WEIGHT_NORMAL, "weight-set", TRUE, NULL);

//	for(auto level0Name : CMediaAnalysisManager::getInstance()->getLevel0NameVector()) {
//		gtk_tree_store_append(GTK_TREE_STORE(structureTreeStore_), &childIter, &iter);
//		gtk_tree_store_set(GTK_TREE_STORE(structureTreeStore_), &childIter, ATOME_NAME, level0Name.c_str(), -1);
//	}

/*
	gtk_tree_store_append(GTK_TREE_STORE(structureTreeModel_), &iter, nullptr);
	gtk_tree_store_set(GTK_TREE_STORE(structureTreeModel_), &iter, 0, "second", -1);
	gtk_tree_store_append(GTK_TREE_STORE(structureTreeModel_), &childIter, &iter);
	gtk_tree_store_set(GTK_TREE_STORE(structureTreeModel_), &childIter, 0, "three", -1);
	gtk_tree_store_append(GTK_TREE_STORE(structureTreeModel_), &childIter, &iter);
	gtk_tree_store_set(GTK_TREE_STORE(structureTreeModel_), &childIter, 0, "four", -1);
*/
//	gtk_tree_view_collapse_all(GTK_TREE_VIEW(structureTreeView_));
// 	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(structureTreeView_), TRUE);

	gtk_widget_grab_focus(structureTreeView_);

	return true;
}

bool CMp4Structure::terminateStructureTreeView() {
	gtk_tree_store_clear (structureTreeStore_);
	return true;
}

void CMp4Structure::treeViewSelectionCallback(GtkTreeSelection* treeSelection, GtkTreeModel* treeModel) {
	TestInfo(LogTag, "%s routine", __FUNCTION__);

}

void CMp4Structure::treeRowActivatedCallback(GtkTreeView* treeView, GtkTreePath* treePath, GtkTreeViewColumn* column) {
	TestInfo(LogTag, "%s routine", __FUNCTION__);
	gchar* treePathString = nullptr;
	treePathString = gtk_tree_path_to_string(treePath);
	TestInfo(LogTag, "Tree path string : %s", treePathString);
	g_free(treePathString);
}

GtkWidget* CMp4Structure::initializeTreeView() {

	GtkWidget* box = nullptr;
	GtkWidget* label = nullptr;

	structureTreeStore_ = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	structureTreeView_ = gtk_tree_view_new();
	gtk_tree_view_set_model(GTK_TREE_VIEW(structureTreeView_), GTK_TREE_MODEL(structureTreeStore_));
	gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(structureTreeView_), TRUE);
	gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(structureTreeView_), TRUE);
	treeSelection_ = gtk_tree_view_get_selection(GTK_TREE_VIEW(structureTreeView_));

	gtk_tree_selection_set_mode(GTK_TREE_SELECTION(treeSelection_), GTK_SELECTION_BROWSE);
	gtk_widget_set_size_request(structureTreeView_, -1, -1);

	//titleColumn = gtk_tree_view_column_new_with_attributes("Structure", titleCell, "text", 0, nullptr);
	structureColumn_ = gtk_tree_view_column_new();
	cellRenderer_ = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(structureColumn_, cellRenderer_, TRUE);
	gtk_tree_view_column_add_attribute(structureColumn_, cellRenderer_, "text", 0);
	gtk_tree_view_append_column(GTK_TREE_VIEW(structureTreeView_), GTK_TREE_VIEW_COLUMN(structureColumn_));

	g_signal_connect(structureTreeView_, "row_activated", G_CALLBACK(CMp4Structure::treeRowActivatedCallback), structureTreeStore_);
	g_signal_connect(treeSelection_, "changed", G_CALLBACK(CMp4Structure::treeViewSelectionCallback), structureTreeStore_);

//	gtk_tree_view_collapse_all(GTK_TREE_VIEW(structureTreeView_));
//	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(structureTreeView_), FALSE);

	scrolledWindow_ = gtk_scrolled_window_new(nullptr, nullptr);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow_), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolledWindow_), structureTreeView_);

	label = gtk_label_new("Structure");
	box = gtk_notebook_new();
	gtk_notebook_append_page(GTK_NOTEBOOK(box), scrolledWindow_, label);

	gtk_widget_grab_focus(structureTreeView_);

	return box;
}
