/*
 * CMp4Structure.h
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#ifndef SRC_UI_CMP4STRUCTURE_H_
#define SRC_UI_CMP4STRUCTURE_H_

#include <gtk/gtk.h>

class CMp4Structure {
public:
	CMp4Structure();
	virtual ~CMp4Structure();
	GtkWidget* initializeTreeView();
	bool generateStructureTreeView();
	bool terminateStructureTreeView();

	static void treeViewSelectionCallback(GtkTreeSelection* treeSelection, GtkTreeModel* treeModel);
	static void treeRowActivatedCallback(GtkTreeView* treeView, GtkTreePath* treePath, GtkTreeViewColumn* column);

private:
	GtkWidget* scrolledWindow_ = nullptr;
	GtkWidget* structureTreeView_ = nullptr;
	GtkTreeStore* structureTreeStore_ = nullptr;
	GtkTreeSelection* treeSelection_ = nullptr;
	GtkCellRenderer* cellRenderer_ = nullptr;
};

#endif /* SRC_UI_CMP4STRUCTURE_H_ */
