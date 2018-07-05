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
	static void treeRowActivatedCallback(GtkTreeView* treeView, GtkTreePath* treePath, GtkTreeViewColumn* column);

private:
	GtkWidget* scrolledWindow_ = nullptr;
	GtkWidget* structureTreeView_ = nullptr;
	GtkTreeStore* structureTreeStore_ = nullptr;
	GtkTreeSelection* treeSelection_ = nullptr;
	GtkCellRenderer* cellRenderer_ = nullptr;
	GtkTreeViewColumn* structureColumn_ = nullptr;
	map<string, GtkTreeIter> subTreeIteratorMap_;
};

#endif /* SRC_UI_CMP4STRUCTURE_H_ */
