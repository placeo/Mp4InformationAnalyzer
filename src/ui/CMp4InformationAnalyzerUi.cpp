/*
 * CMp4AnalyzerUi.mm
 *
 *  Created on: May 29, 2018
 *      Author: placeo
 */

#include <gdk/gdkkeysyms.h>
#include "CMp4AnalyzerLogger.h"
#include "CMp4InformationAnalyzerUi.h"
#include "CMediaAnalysisManager.h"

const char* LogTag = "UI";

CMp4AnalyzerUi::CMp4AnalyzerUi() {
	mp4Structure_ = new CMp4Structure();
	mp4Information_ = new CMp4Information();
	mp4Binary_ = new CMp4Binary();
}

CMp4AnalyzerUi::~CMp4AnalyzerUi() {
	// TODO Auto-generated destructor stub
}

void CMp4AnalyzerUi::createMainUi(CMp4AnalyzerUi* mp4AnalyzerUi) {
	mp4AnalyzerUi->mainWindow_ = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget* mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	mp4AnalyzerUi->accelGroup_ = gtk_accel_group_new ();
	gtk_window_add_accel_group(GTK_WINDOW (mp4AnalyzerUi->mainWindow_), mp4AnalyzerUi->accelGroup_);

	GtkWidget* menuBar = createMenuBar(mp4AnalyzerUi);
	GtkWidget* toolbar = createToolBar(mp4AnalyzerUi);
	GtkWidget* contentsBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	GtkWidget* structureView = mp4AnalyzerUi->mp4Structure_->initializeTreeView();
	GtkWidget* informationView = mp4AnalyzerUi->mp4Information_->generateMp4InformationView();
	GtkWidget* binaryView = mp4AnalyzerUi->mp4Binary_->generateBinaryView();

	gtk_window_set_title(GTK_WINDOW(mp4AnalyzerUi->mainWindow_), "MP4 Information Analyzer");
    gtk_widget_set_size_request(GTK_WIDGET(mp4AnalyzerUi->mainWindow_), 900, 480);
    gtk_container_set_border_width (GTK_CONTAINER (mp4AnalyzerUi->mainWindow_), 0);


    gtk_widget_set_size_request(GTK_WIDGET(structureView), 200, -1);
    gtk_widget_set_size_request(GTK_WIDGET(informationView), 400, -1);
    gtk_widget_set_size_request(GTK_WIDGET(binaryView), 300, -1);

	gtk_container_add(GTK_CONTAINER(mp4AnalyzerUi->mainWindow_), mainBox);

	gtk_box_pack_start(GTK_BOX (mainBox), menuBar, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX (mainBox), toolbar, FALSE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(contentsBox), structureView, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(contentsBox), informationView, TRUE, TRUE, 2);
	gtk_box_pack_end(GTK_BOX(contentsBox), binaryView, FALSE, FALSE, 2);
	gtk_box_pack_start(GTK_BOX(mainBox), contentsBox, TRUE, TRUE, 2);

	g_signal_connect (G_OBJECT (mp4AnalyzerUi->mainWindow_), "delete-event", G_CALLBACK(CMp4AnalyzerUi::deleteEventCallback), mp4AnalyzerUi);
	gtk_widget_show_all(mp4AnalyzerUi->mainWindow_);
}

void CMp4AnalyzerUi::deleteEventCallback(GtkWidget* widget, GdkEvent* event, gpointer data) {
	gtk_main_quit();
}

void CMp4AnalyzerUi::fileOpenCallback(gpointer userData) {
	TestInfo(LogTag, "%s routine", __FUNCTION__);
	CMp4AnalyzerUi* mp4AnalyzeUi = static_cast<CMp4AnalyzerUi*>(userData);
	GtkWidget *dialog;
	gint response;
	char *openFileName;

	dialog = gtk_file_chooser_dialog_new ("Select file",
			GTK_WINDOW (mp4AnalyzeUi->mainWindow_),
			GTK_FILE_CHOOSER_ACTION_OPEN,
			("_Cancel"), GTK_RESPONSE_CANCEL,
			("_Open"), GTK_RESPONSE_OK,
			NULL);
	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_OK);
	response = gtk_dialog_run (GTK_DIALOG (dialog));

	if (response == GTK_RESPONSE_OK) {
		openFileName = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		mp4AnalyzeUi->loadFile(openFileName, mp4AnalyzeUi);
		g_free (openFileName);
	}
	CMediaAnalysisManager::getInstance()->extractFileName();
	if(false == mp4AnalyzeUi->getMp4Structure()->generateStructureTreeView()) {
		TestError(LogTag, "Failed to generate structure tree view");
	}
	gtk_widget_set_size_request(GTK_WIDGET(dialog), 400, 300);
	gtk_widget_destroy (dialog);
}

void CMp4AnalyzerUi::fileCloseCallback(gpointer userData) {
	TestInfo(LogTag, "%s routine", __FUNCTION__);
	CMp4AnalyzerUi* mp4AnalyzeUi = static_cast<CMp4AnalyzerUi*>(userData);
	if(mp4AnalyzeUi->filePointer_ != nullptr) {
		fclose(mp4AnalyzeUi->filePointer_);
		mp4AnalyzeUi->filePointer_ = nullptr;
	}
	else {
		TestWarning(LogTag, "Failed to close file");
	}
}

void CMp4AnalyzerUi::quitCallback(gpointer userData) {
	TestInfo(LogTag, "%s routine", __FUNCTION__);
	gtk_main_quit();
}

void CMp4AnalyzerUi::aboutCallback(gpointer userData) {
	TestInfo(LogTag, "%s routine", __FUNCTION__);
}

GtkWidget* CMp4AnalyzerUi::createMenuBar(CMp4AnalyzerUi* mp4AnalyzerUi) {
	GtkWidget* menuBar = nullptr;
	GtkWidget* fileMenuItem = nullptr;
	GtkWidget* fileMenu = nullptr;
	GtkWidget* helpMenuItem = nullptr;
	GtkWidget* helpMenu = nullptr;

	menuBar = gtk_menu_bar_new();

	fileMenuItem = gtk_menu_item_new_with_mnemonic(("_File"));
			//("media-playback-stop", GTK_ICON_SIZE_BUTTON); // gtk_image_menu_item_new_from_(GTK_STOCK_HOME, mp4AnalyzerUi->accelGroup_);
	gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), fileMenuItem);
	fileMenu = createFileMenu(mp4AnalyzerUi);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMenuItem), fileMenu);

	helpMenuItem = gtk_menu_item_new_with_mnemonic(("_Help"));
	gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), helpMenuItem);
	helpMenu = createHelpMenu(mp4AnalyzerUi);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(helpMenuItem), helpMenu);

	return menuBar;
}

GtkWidget* CMp4AnalyzerUi::createFileMenu(CMp4AnalyzerUi* mp4AnalyzerUi) {
    GtkWidget* menu;
    GtkWidget* fileOpenItem;
    GtkWidget* fileCloseItem;
    GtkWidget* separatorItem;
    GtkWidget* quitItem;

    menu = gtk_menu_new ();
    fileOpenItem = gtk_menu_item_new_with_mnemonic(("_Open"));
    fileCloseItem = gtk_menu_item_new_with_mnemonic(("_Close"));
    separatorItem = gtk_separator_menu_item_new();
    quitItem = gtk_menu_item_new_with_mnemonic(("_Quit"));

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), fileOpenItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), fileCloseItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), separatorItem);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), quitItem);

    gtk_widget_add_accelerator(fileOpenItem, "activate", mp4AnalyzerUi->accelGroup_, GDK_KEY_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(fileCloseItem, "activate", mp4AnalyzerUi->accelGroup_, GDK_KEY_c, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(quitItem, "activate", mp4AnalyzerUi->accelGroup_, GDK_KEY_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	g_signal_connect_swapped(fileOpenItem, "activate", G_CALLBACK(CMp4AnalyzerUi::fileOpenCallback), mp4AnalyzerUi);
	g_signal_connect_swapped(fileCloseItem, "activate", G_CALLBACK(CMp4AnalyzerUi::fileCloseCallback), mp4AnalyzerUi);
	g_signal_connect_swapped(quitItem, "activate", G_CALLBACK(CMp4AnalyzerUi::quitCallback), mp4AnalyzerUi);

    return menu;
}

GtkWidget* CMp4AnalyzerUi::createHelpMenu(CMp4AnalyzerUi* mp4AnalyzerUi) {
    GtkWidget* menu;
    GtkWidget* aboutItem;

    menu = gtk_menu_new ();
    aboutItem = gtk_menu_item_new_with_mnemonic(("_About"));

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), aboutItem);

    gtk_widget_add_accelerator(aboutItem, "activate", mp4AnalyzerUi->accelGroup_, GDK_KEY_a, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    g_signal_connect_swapped(aboutItem, "activate", G_CALLBACK(CMp4AnalyzerUi::aboutCallback), mp4AnalyzerUi);

    return menu;
}

GtkWidget* CMp4AnalyzerUi::createToolBar(CMp4AnalyzerUi* mp4AnalyzerUi) {
	GtkWidget* toolBar = nullptr;
//	GtkWidget* grid = nullptr;
	GtkToolItem* item = nullptr;

	toolBar = gtk_toolbar_new();

	item = gtk_tool_button_new(NULL, NULL);
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(item), "document-open");
	g_signal_connect(GTK_TOOL_BUTTON(item), "clicked", G_CALLBACK(toolBarFileOpenCallback), mp4AnalyzerUi);
	gtk_toolbar_insert(GTK_TOOLBAR(toolBar), item, -1);

	item = gtk_tool_button_new(NULL, NULL);
	gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(item), "application-exit");
	g_signal_connect(GTK_TOOL_BUTTON(item), "clicked", G_CALLBACK(quitCallback), mp4AnalyzerUi);
	gtk_toolbar_insert(GTK_TOOLBAR(toolBar), item, -1);
//	gtk_grid_attach(GTK_GRID(grid), toolBar, 0, 0, 2, 1);

//	item = gtk_menu_tool_button_new(NULL, NULL);

//	gtk_toolbar_insert_stock(GTK_TOOLBAR (toolBar), GTK_STOCK_OPEN, "Open file", "Toolbar/Open", G_CALLBACK (CMp4AnalyzerUi::toolBarFileOpenCallback), mp4AnalyzerUi, -1);
//	gtk_toolbar_insert_stock(GTK_TOOLBAR (toolBar), GTK_STOCK_CLOSE, "Close file", "Toolbar/Close", G_CALLBACK (CMp4AnalyzerUi::toolBarFileCloseCallback), mp4AnalyzerUi, -1);

//  gtk_toolbar_append_space(GTK_TOOLBAR(toolBar));
    return toolBar;
}

bool CMp4AnalyzerUi::loadFile(const char* openFileName, gpointer data) {
	CMp4AnalyzerUi* mp4AnalyzerUi = static_cast<CMp4AnalyzerUi*>(data);
	GtkWidget *errorDialog = nullptr;
	GError *error = nullptr;
	int returnValue = 0;

	if(false == mp4AnalyzerUi->getMp4Structure()->terminateStructureTreeView()) {
		TestError(LogTag, "Failed to terminate structure tree view");
	}

	// open and close file
	if(nullptr == (mp4AnalyzerUi->filePointer_ = fopen(openFileName, "rb"))) {
		errorDialog = gtk_message_dialog_new (GTK_WINDOW(mp4AnalyzerUi->mainWindow_),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE,
				"Error loading file %s:\n%s",
				openFileName,
				error->message);
		g_error_free (error);
	}
	else {
		fseek(mp4AnalyzerUi->filePointer_, 0, SEEK_END);
		mp4AnalyzerUi->fileSize_ = ftell(mp4AnalyzerUi->filePointer_);
		rewind(mp4AnalyzerUi->filePointer_);
		TestInfo(LogTag, "Load file name : %s", openFileName);
	}

	if((returnValue = fclose(mp4AnalyzerUi->filePointer_)) != 0) {
		TestError(LogTag, "Failed to close file, return value : %d", returnValue);
	}

	CMediaAnalysisManager::getInstance()->setFileLocation(string(openFileName));
	if(false == CMediaAnalysisManager::getInstance()->generateMp4AnalysisJson()) {
		TestError(LogTag, "Failed to generate mp4 analysis json");
		return false;
	}

	if(false == CMediaAnalysisManager::getInstance()->retrieveMediaInformation()) {
		TestError(LogTag, "Failed to parse media information");
	}

	if (errorDialog)
	{
	  g_signal_connect (errorDialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
	  gtk_widget_show (errorDialog);
	  return false;
	}
	return true;
}

void CMp4AnalyzerUi::toolBarFileOpenCallback(GtkWidget* widget, gpointer userData) {
	fileOpenCallback(userData);
}

void CMp4AnalyzerUi::toolBarFileCloseCallback(GtkWidget* widget, gpointer userData) {
	fileCloseCallback(userData);
}
