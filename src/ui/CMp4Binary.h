/*
 * CMp4Binary.h
 *
 *  Created on: Jun 5, 2018
 *      Author: placeo
 */

#ifndef SRC_UI_CMP4BINARY_H_
#define SRC_UI_CMP4BINARY_H_

#include <gtk/gtk.h>

class CMp4Binary {
public:
	CMp4Binary();
	virtual ~CMp4Binary();
	GtkWidget* generateBinaryView();
	bool terminateBinaryView();
private :
	GtkWidget* scrolledWindow_ = nullptr;
};

#endif /* SRC_UI_CMP4BINARY_H_ */
