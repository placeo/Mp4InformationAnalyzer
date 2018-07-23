/*
 * CMp4Structure.h
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#ifndef SRC_UI_CMP4STRUCTURE_H_
#define SRC_UI_CMP4STRUCTURE_H_

#include <gtk/gtk.h>
#include <json-c/json.h>
#include <map>
#include "CMp4Information.h"
#include "CMp4Binary.h"

using namespace std;

class CMp4Structure {
public:
	CMp4Structure();
	virtual ~CMp4Structure();
	GtkWidget* initializeTreeView();
	bool generateStructureTreeView();
	bool terminateStructureTreeView();
	bool generateStructureSubTreeView(map<string, GtkTreeIter>& subTreeIteratorMap, map<string, json_object*>& atomMap, int comparingAtomPathLength, int maxAtomPathLength);

	static void treeViewSelectionCallback(GtkTreeSelection* treeSelection, GtkTreeModel* treeModel);
	static void treeRowActivatedCallback(GtkTreeView* treeView, GtkTreePath* treePath, GtkTreeViewColumn* column, gpointer userData);

	CMp4Binary* getMp4Binary() const {
		return mp4Binary_;
	}

	void setMp4Binary(CMp4Binary* mp4Binary) {
		mp4Binary_ = mp4Binary;
	}

	CMp4Information* getMp4Information() {
		return mp4Information_;
	}

	void setMp4Information(CMp4Information* mp4Information) {
		mp4Information_ = mp4Information;
	}

private:
	GtkWidget* scrolledWindow_ = nullptr;
	GtkWidget* structureTreeView_ = nullptr;
	GtkTreeStore* structureTreeStore_ = nullptr;
	GtkTreeSelection* treeSelection_ = nullptr;
	GtkCellRenderer* cellRenderer_ = nullptr;
	GtkTreeViewColumn* structureColumn_ = nullptr;
	map<string, GtkTreeIter> subTreeIteratorMap_;
	CMp4Information* mp4Information_ = nullptr;
	CMp4Binary* mp4Binary_ = nullptr;
};

#endif /* SRC_UI_CMP4STRUCTURE_H_ */
