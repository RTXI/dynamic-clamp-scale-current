/*
 * Copyright (C) 2011 Weill Medical College of Cornell University
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef ADDSTEPDIALOG_H
#define ADDSTEPDIALOG_H

#include <QtGlobal>
#if QT_VERSION >= 0x050000
	#include <QtWidgets>
#else
	#include <QtGui>
#endif

class AddStepDialog : public QDialog {
	Q_OBJECT

	public:
		AddStepDialog( QWidget* parent = 0 );
		~AddStepDialog();

		QComboBox* stepComboBox;
		QLabel* BCLLabel;
		QLineEdit* BCLEdit;
		QLabel* numBeatsLabel;
		QLineEdit* numBeatsEdit;
		QLabel* DILabel;
		QLineEdit* DIEdit;
		QLabel* currentToScaleLabel;
		QLineEdit* currentToScaleEdit;
		QLabel* scalingPercentageLabel;
		QLineEdit* scalingPercentageEdit;
		QLabel* waitTimeLabel;
		QLineEdit* waitTimeEdit;
		QLabel* modelLabel;
		QComboBox* modelComboBox;
		QButtonGroup* buttonGroup;
		QGroupBox* buttonGroupBox;
		QPushButton* addStepButton;
		QPushButton* exitButton;

	protected:
		QGridLayout* AddStepDialogLayout;
		QHBoxLayout* buttonGroupLayout;
		QHBoxLayout* buttonGroupBoxLayout;
};

#endif // ADDSTEPDIALOG_H
