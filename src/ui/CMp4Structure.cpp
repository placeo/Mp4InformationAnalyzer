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
	GtkTreeIter rootIter;
	GtkTreeIter rootChildIter;

	gtk_tree_model_get_iter_first(GTK_TREE_MODEL(structureTreeStore_), &rootIter);
//	gtk_tree_selection_select_iter(GTK_TREE_SELECTION(treeSelection_), &iter);

	g_object_set(cellRenderer_, "weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);

	gtk_tree_store_append(GTK_TREE_STORE(structureTreeStore_), &rootIter, nullptr);
	gtk_tree_store_set(GTK_TREE_STORE(structureTreeStore_), &rootIter, ATOME_NAME, CMediaAnalysisManager::getInstance()->getFileName().data(), -1);

	int comparingAtomPathLength = 3;
	int maxAtomPathLength = 0;

	for(auto atom : CMediaAnalysisManager::getInstance()->getAtomMap()) {
		json_object* jsonNameObject = nullptr;
		string atomName;
		if(atom.first.length() == comparingAtomPathLength) {
			if(0 == json_object_object_get_ex(atom.second, "name", &jsonNameObject)) {
				TestError(LogTag, "Failed to get atom name");
				return false;
			}
			else {
				atomName = string(json_object_get_string(jsonNameObject));
				gtk_tree_store_append(GTK_TREE_STORE(structureTreeStore_), &rootChildIter, &rootIter);
				gtk_tree_store_set(GTK_TREE_STORE(structureTreeStore_), &rootChildIter, ATOME_NAME, atomName.c_str(), -1);
				TestInfo(LogTag, "Add atom name to sub tree iterator map : %s, key :%s", atomName.c_str(), atom.first.c_str());
				subTreeIteratorMap_[atom.first] = rootChildIter;
			}
		}
		if(atom.first.length() > maxAtomPathLength) maxAtomPathLength = atom.first.length();
	}

	generateStructureSubTreeView(subTreeIteratorMap_, CMediaAnalysisManager::getInstance()->getAtomMap(), comparingAtomPathLength + 2, maxAtomPathLength);

	gtk_widget_grab_focus(structureTreeView_);

	return true;
}

bool CMp4Structure::generateStructureSubTreeView(	map<string, GtkTreeIter>& subTreeIteratorMap, map<string, json_object*>& atomMap, int comparingAtomPathLength, int maxAtomPathLength) {
	GtkTreeIter subIter;
	GtkTreeIter subChildIter;

	for(auto atom : atomMap) {
		json_object* jsonNameObject = nullptr;
		string atomName;
		if(atom.first.length() == comparingAtomPathLength) {
			TestInfo(LogTag, "atom tree path string : %s", atom.first.substr(0, comparingAtomPathLength - 2).c_str());
			auto mapIter = subTreeIteratorMap.find(atom.first.substr(0, comparingAtomPathLength - 2)) ;
			if(mapIter != subTreeIteratorMap.end()) {
				if(0 == json_object_object_get_ex(atom.second, "name", &jsonNameObject)) {
					TestError(LogTag, "Failed to get atom name");
					return false;
				}
				else {

					atomName = string(json_object_get_string(jsonNameObject));
					TestInfo(LogTag, "retrieved atom name : %s", atomName.c_str());
					subIter = mapIter->second;
					gtk_tree_store_append(GTK_TREE_STORE(structureTreeStore_), &subChildIter, &subIter);
					gtk_tree_store_set(GTK_TREE_STORE(structureTreeStore_), &subChildIter, ATOME_NAME, atomName.c_str(), -1);
					subTreeIteratorMap[atom.first] = subChildIter;
				}
			}
			else {
				TestError(LogTag, "Failed to retrieve tree iterator");
				return false;
			}
		}
	}
	comparingAtomPathLength += 2;
	if(comparingAtomPathLength > maxAtomPathLength || false == generateStructureSubTreeView(subTreeIteratorMap, atomMap, comparingAtomPathLength, maxAtomPathLength)) {
		TestWarning(LogTag, "Failed to get sub tree view");
		return false;
	}
	return true;
}

bool CMp4Structure::terminateStructureTreeView() {
	gtk_tree_store_clear (structureTreeStore_);
	subTreeIteratorMap_.clear();
	CMediaAnalysisManager::getInstance()->getAtomMap().clear();
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

	auto mapIter = CMediaAnalysisManager::getInstance()->getAtomMap().find(string(treePathString));
	g_free(treePathString);

	if(mapIter != CMediaAnalysisManager::getInstance()->getAtomMap().end()) {
		json_object* atomJsonObjet = mapIter->second;
		json_object* atomNameJsonObject = nullptr;
		json_object* atomOffsetJsonObject = nullptr;
		json_object* atomSizeJsonObject = nullptr;
		string atomName;
		long long int atomOffset;
		long long int atomSize;

		if(0 == json_object_object_get_ex(atomJsonObjet, "name", &atomNameJsonObject)) {
			TestError(LogTag, "Failed to get atom name");
			return;
		}
		else {
			atomName = string(json_object_get_string(atomNameJsonObject));
			TestInfo(LogTag, "retrieved atom name : %s", atomName.c_str());
		}

		if(0 == json_object_object_get_ex(atomJsonObjet, "offset", &atomOffsetJsonObject)) {
			TestError(LogTag, "Failed to get atom offset");
			return;
		}
		else {
			atomOffset = json_object_get_int64(atomOffsetJsonObject);
			TestInfo(LogTag, "retrieved atom offset : %lld", atomOffset);
		}

		if(0 == json_object_object_get_ex(atomJsonObjet, "size", &atomSizeJsonObject)) {
			TestError(LogTag, "Failed to get atom size");
			return;
		}
		else {
			atomSize = json_object_get_int64(atomSizeJsonObject);
			TestInfo(LogTag, "retrieved atom size : %lld", atomSize);
		}
	}
	else {
		TestError(LogTag, "Failed to retrieve tree iterator");
		return;
	}
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
