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

/*** INTRO
 * Current Scaling Dynamic Clamp
 * 
 * IS_DC_Protocol.cpp, v1.0
 *
 * Author: Francis A. Ortega (2011)
 *
 * Notes in header
 *
 ***/

#include "IS_DC_Protocol.h"
#include <iostream>
#include <qvalidator.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qfiledialog.h>

using namespace std;

/* AddStepInputDialog Class */
AddStepInputDialog::AddStepInputDialog(QWidget* parent ) :
    AddStepDialog( parent, 0, TRUE ) {
    QValidator* validator = new QIntValidator(this);
    BCLEdit->setValidator(validator);
    numBeatsEdit->setValidator(validator);
    scalingPercentageEdit->setValidator(validator);
    
    QObject::connect( addStepButton,SIGNAL(clicked(void)),this,SLOT(addStepClicked(void)) );
    QObject::connect( exitButton, SIGNAL(clicked(void)), this, SLOT( reject() ) );
    QObject::connect( this, SIGNAL(checked(void)), this, SLOT(accept()) ); // Dialog returns Accept after inputs have been checked
    QObject::connect( stepComboBox, SIGNAL(activated(int)), SLOT(stepComboBoxUpdate(int)) ); // Updates when combo box selection is changed
    
    stepComboBoxUpdate(0);
}

AddStepInputDialog::~AddStepInputDialog( void ) { 
}

void AddStepInputDialog::stepComboBoxUpdate( int selection ) {
    switch( (ProtocolStep::stepType_t)selection ) {
    case ProtocolStep::PACE:
        BCLEdit->setEnabled(true);
        numBeatsEdit->setEnabled(true);
        currentToScaleEdit->setEnabled(false);
        scalingPercentageEdit->setEnabled(false);
        waitTimeEdit->setEnabled(false);
        break;
    case ProtocolStep::SCALE:
        BCLEdit->setEnabled(true);
        numBeatsEdit->setEnabled(true);
        currentToScaleEdit->setEnabled(true);
        scalingPercentageEdit->setEnabled(true);
        waitTimeEdit->setEnabled(false);
        break;
    case ProtocolStep::WAIT:
        BCLEdit->setEnabled(false);
        numBeatsEdit->setEnabled(false);
        currentToScaleEdit->setEnabled(false);
        scalingPercentageEdit->setEnabled(false);
        waitTimeEdit->setEnabled(true);
        break;
    case ProtocolStep::STARTMODEL:
        BCLEdit->setEnabled(false);
        numBeatsEdit->setEnabled(false);
        currentToScaleEdit->setEnabled(false);
        scalingPercentageEdit->setEnabled(false);
        waitTimeEdit->setEnabled(false);
        break;
    case ProtocolStep::STOPMODEL:
        BCLEdit->setEnabled(false);
        numBeatsEdit->setEnabled(false);
        currentToScaleEdit->setEnabled(false);
        scalingPercentageEdit->setEnabled(false);
        waitTimeEdit->setEnabled(false);
        break;
    case ProtocolStep::RESETMODEL:
        BCLEdit->setEnabled(false);
        numBeatsEdit->setEnabled(false);
        currentToScaleEdit->setEnabled(false);
        scalingPercentageEdit->setEnabled(false);
        waitTimeEdit->setEnabled(false);
        break;
    }
}

void AddStepInputDialog::addStepClicked( void ) { // Initializes QStrings and checks if they are valid entries
    bool check = true;
    BCL = BCLEdit->text();
    stepType = QString::number( stepComboBox->currentItem() );
    numBeats = numBeatsEdit->text();
    currentToScale = currentToScaleEdit->text();
    scalingPercentage = scalingPercentageEdit->text();
    waitTime = waitTimeEdit->text();
    
    switch( stepComboBox->currentItem() ) {
    case 0:
        if(BCL == "" || numBeats == "")
            check = false;
        break;
        
    case 1:
        if(BCL == "" || numBeats == "" || currentToScale == "")
            check = false;
        break;
        
    case 2:
        if(waitTime == "")
            check = false;
        break;
    }

    if(check)
        emit checked();
    else
        QMessageBox::warning( this, "Error", "Invalid Input, please correct." );
}

vector<QString> AddStepInputDialog::gatherInput( void ) {
    std::vector<QString> inputAnswers;
    
    if( exec() == QDialog::Rejected )
        return inputAnswers; // Return an empty vector if step window is closed
    else { // QDialog is accepted when addStep button is pressed and inputs are considered valid
        inputAnswers.push_back( QString::number( stepComboBox->currentItem() ) );
        inputAnswers.push_back( BCL );
        inputAnswers.push_back( numBeats );
        inputAnswers.push_back( currentToScale );
        inputAnswers.push_back( scalingPercentage );
        inputAnswers.push_back( waitTime );
        return inputAnswers;
    }
}

/* Protocol Step Class */

ProtocolStep::ProtocolStep( stepType_t st, int bcl, int nb, string c, int sp, int w) :
    stepType(st), BCL(bcl), numBeats(nb), currentToScale(c), scalingPercentage(sp), waitTime(w) {
}

ProtocolStep::~ProtocolStep( void ) {
}

int ProtocolStep::stepLength( double period ) {
}

/* Protocol Class */

Protocol::Protocol( void ) {
}

Protocol::~Protocol( void ) {
}

// Opens input dialogs to gather step information, then adds to protocol container *at the end*
// Returns true if a step was added
bool Protocol::addStep( QWidget *parent ) {
    AddStepInputDialog *dlg = new AddStepInputDialog(parent); // Special dialog box for step parameter input
    vector<QString> inputAnswers = dlg->gatherInput(); // Opens dialog box for input
    delete dlg;
    
    if( inputAnswers.size() > 0 ) {
        // Add a new step to protocol container
        protocolContainer.push_back( ProtocolStepPtr( new ProtocolStep(
                                                                       (ProtocolStep::stepType_t)( inputAnswers[0].toInt() ), // stepType
                                                                       inputAnswers[1].toInt(), // BCL
                                                                       inputAnswers[2].toInt(), // numBeats
                                                                       inputAnswers[3], // currentToScale
                                                                       inputAnswers[4].toInt(), // scalingPercentage
                                                                       inputAnswers[5].toInt() // waitTime
                                                                       ) ) );
        return true;
    }
    else
        return false; // No step added
}

// Opens input dialogs to gather step information, then adds to protocol container at *a specific point*
// Returns true if a step was added
bool Protocol::addStep( QWidget *parent, int idx ) {
    AddStepInputDialog *dlg = new AddStepInputDialog(parent); // Special dialog box for step parameter input
    vector<QString> inputAnswers = dlg->gatherInput(); // Opens dialog box for input
    delete dlg;

    vector<ProtocolStepPtr>::iterator it = protocolContainer.begin();
    
    if( inputAnswers.size() > 0 ) {
        // Add a new step to protocol container
        protocolContainer.insert( it+idx+1, ProtocolStepPtr( new ProtocolStep(
                                                                       (ProtocolStep::stepType_t)( inputAnswers[0].toInt() ), // stepType
                                                                       inputAnswers[1].toInt(), // BCL
                                                                       inputAnswers[2].toInt(), // numBeats
                                                                       inputAnswers[3], // currentToScale
                                                                       inputAnswers[4].toInt(), // scalingPercentage
                                                                       inputAnswers[5].toInt() // waitTime
                                                                       ) ) );
        return true;
    }
    else
        return false; // No step added
}

    // Deletes a step
void Protocol::deleteStep( QWidget *parent, int stepNumber ) {
    // Message box asking for confirmation whether step should be deleted
    QString text = "Do you wish to delete step " + QString::number(stepNumber+1) + "?"; // Text pointing out specific step
    if( QMessageBox::question(parent,
                              "Delete Step Confirmation",
                              text,
                              "Yes",
                              "No") )
        return ; // Answer is no

    if( protocolContainer.size() == 1 ) // If only 1 step exists, explicitly clear step pointer
        protocolContainer.clear();
    else 
        protocolContainer.erase(protocolContainer.begin() + stepNumber);
    
}

void Protocol::saveProtocol( QWidget *parent ) {
    // Make sure protocol has at least one segment with one step
    if( protocolContainer.size() == 0 ) { 
        QMessageBox::warning(parent,
                             "Error",
                             "A protocol must contain at least one step" );
        return;
    }
    
    // Create QDomDocument
    QDomDocument protocolDoc("IScaleProtocol");
    QDomElement root = protocolDoc.createElement( "IS_DC_protocol-v1.0");
    protocolDoc.appendChild(root);   
    
    // Save dialog to retrieve desired filename and location
    QString fileName = QFileDialog::getSaveFileName(
                                                    "~/",
                                                    "XML Files (*.xml)",
                                                    parent,
                                                    "save file dialog",
                                                    "Save the protocol" );

    // If filename does not include .xml extension, add extension
    if( !(fileName.endsWith(".xml")) )
        fileName.append(".xml");
    // If filename exists, warn user
    if ( QFileInfo(fileName).exists() &&
         QMessageBox::warning(
                              parent,
                              "File Exists", "Do you wish to overwrite " + fileName + "?",
                              QMessageBox::Yes | QMessageBox::Default, QMessageBox::No
                              | QMessageBox::Escape) != QMessageBox::Yes)
        return ; // Return if answer is no

    // Add segment elements to protocolDoc
    for( int i = 0; i < protocolContainer.size(); i++ ) {
        root.appendChild( stepToNode(protocolDoc, protocolContainer.at(i), i) );
    }

    // Save protocol to file
    QFile file(fileName); // Open file
    if( !file.open(IO_WriteOnly) ) { // Open file, return error if unable to do so
        QMessageBox::warning(parent,
                             "Error",
                             "Unable to save file: Please check folder permissions." );
        return ;
    }
    QTextStream ts(&file); // Open text stream
    ts << protocolDoc.toString(); // Write to file
    file.close(); // Close file
}

QString Protocol::loadProtocol( QWidget *parent ) {
    // If protocol is present, warn user that protocol will be lost upon loading
    if( protocolContainer.size() != 0 &&
        QMessageBox::warning(
                             parent,
                             "Load Protocol", "All unsaved changes to current protocol will be lost.\nDo you wish to continue?",
                             QMessageBox::Yes | QMessageBox::Default, QMessageBox::No
                             | QMessageBox::Escape) != QMessageBox::Yes )
        return ""; // Return if answer is no

    // Save dialog to retrieve desired filename and location
    QString fileName = QFileDialog::getOpenFileName(
                                                    "~/",
                                                    "XML Files (*.xml)",
                                                    parent,
                                                    "open file dialog",
                                                    "Open a protocol" );
    QDomDocument doc( "protocol" );
    QFile file( fileName );

    if( !file.open( IO_ReadOnly ) ) { // Make sure file can be opened, if not, warn user
        QMessageBox::warning(parent,
                             "Error",
                             "Unable to open protocol file" );
        return "";
    }   
    if( !doc.setContent( &file ) ) { // Make sure file contents are loaded into document
            QMessageBox::warning(parent,
                                 "Error",
                                 "Unable to set file contents to document" );
            file.close();
            return "";
        }
    file.close();

    QDomElement root = doc.documentElement(); // Get root element from document
    if( root.tagName() != "IS_DC_protocol-v1.0" ) { // Check if tagname is correct for this module version
        QMessageBox::warning(parent,
                             "Error",
                             "Incompatible XML file" );
        return "";
    }

    // Retrieve information from document and set to protocolContainer
    QDomNode stepNode = root.firstChild(); // Retrieve first segment
    protocolContainer.clear(); // Clear vector containing protocol
          
    while( !stepNode.isNull() ) {// Step iteration
        QDomElement stepElement = stepNode.toElement();

        // Add new step to protocol container
        protocolContainer.push_back( ProtocolStepPtr( new ProtocolStep(
                                                      (ProtocolStep::stepType_t)stepElement.attribute("stepType").toInt(),
                                                      stepElement.attribute( "BCL" ).toInt(),
                                                      stepElement.attribute( "numBeats" ).toInt(),
                                                      stepElement.attribute( "currentToScale" ),
                                                      stepElement.attribute( "scalingPercentage" ).toInt(),
                                                      stepElement.attribute( "waitTime" ).toInt()
                                                      ) ) ); // Add step to segment container            

        stepNode = stepNode.nextSibling(); // Move to next step
    } // End step iteration

    // Update segment summary and table
    if( protocolContainer.size() == 0 )
        QMessageBox::warning(parent,
                             "Error",
                             "Protocol did not contain any steps" );

    return fileName;
}

// Load protocol straight from designated filename, used for settings file loading
void Protocol::loadProtocol( QWidget *parent, QString fileName ) {
    // If protocol is present, warn user that protocol will be lost upon loading
    if( protocolContainer.size() != 0 &&
        QMessageBox::warning(
                             parent,
                             "Load Protocol", "All unsaved changes to current protocol will be lost.\nDo you wish to continue?",
                             QMessageBox::Yes | QMessageBox::Default, QMessageBox::No
                             | QMessageBox::Escape) != QMessageBox::Yes )
        return ; // Return if answer is no

    QDomDocument doc( "protocol" );
    QFile file( fileName );

    if( !file.open( IO_ReadOnly ) ) { // Make sure file can be opened, if not, warn user
        QMessageBox::warning(parent,
                             "Error",
                             "Unable to open protocol file" );
        return ;
    }   
    if( !doc.setContent( &file ) ) { // Make sure file contents are loaded into document
            QMessageBox::warning(parent,
                                 "Error",
                                 "Unable to set file contents to document" );
            file.close();
            return ;
        }
    file.close();

    QDomElement root = doc.documentElement(); // Get root element from document
    if( root.tagName() != "IS_DC_protocol-v1.0" ) { // Check if tagname is correct for this module version
        QMessageBox::warning(parent,
                             "Error",
                             "Incompatible XML file" );
        return;
    }

    // Retrieve information from document and set to protocolContainer
    QDomNode stepNode = root.firstChild(); // Retrieve first segment
    protocolContainer.clear(); // Clear vector containing protocol
          
    while( !stepNode.isNull() ) {// Step iteration
        QDomElement stepElement = stepNode.toElement();

        // Add new step to protocol container
        protocolContainer.push_back( ProtocolStepPtr( new ProtocolStep(
                                                      (ProtocolStep::stepType_t)stepElement.attribute("stepType").toInt(),
                                                      stepElement.attribute( "BCL" ).toInt(),
                                                      stepElement.attribute( "numBeats" ).toInt(),
                                                      stepElement.attribute( "currentToScale" ),
                                                      stepElement.attribute( "scalingPercentage" ).toInt(),
                                                      stepElement.attribute( "waitTime" ).toInt()
                                                      ) ) ); // Add step to segment container            

        stepNode = stepNode.nextSibling(); // Move to next step
    } // End step iteration

    // Update segment summary and table
    if( protocolContainer.size() == 0 )
        QMessageBox::warning(parent,
                             "Error",
                             "Protocol did not contain any steps" );
}

QDomElement Protocol::stepToNode( QDomDocument &doc, const ProtocolStepPtr stepPtr, int stepNumber ) {
    QDomElement stepElement = doc.createElement("step"); // Step element

    // Set attributes of step to element
    stepElement.setAttribute( "stepType", QString::number( stepPtr->stepType ) );
    stepElement.setAttribute( "BCL", QString::number( stepPtr->BCL ) );
    stepElement.setAttribute( "numBeats", QString::number( stepPtr->numBeats ) );
    stepElement.setAttribute( "currentToScale", stepPtr->currentToScale );
    stepElement.setAttribute( "scalingPercentage", stepPtr->scalingPercentage );
    stepElement.setAttribute( "waitTime", stepPtr->waitTime );

    return stepElement;
}

void Protocol::clearProtocol( void ) {
    protocolContainer.clear();
}

QString Protocol::getStepDescription( int stepNumber ) {
    ProtocolStepPtr step = protocolContainer.at( stepNumber );

    QString type;
    QString description;
    
    switch( step->stepType ) {
        
    case ProtocolStep::PACE:
        type = "Pace ";
        description = type + ": " + QString::number( step->numBeats ) + " beats - " + QString::number( step->BCL ) + "ms BCL";
        break;
            
    case ProtocolStep::SCALE:
        type = "Scale " + step->currentToScale + "(" + QString::number( step->scalingPercentage ) + "%)";
        description = type + ": " + QString::number( step->numBeats ) + " beats - " + QString::number( step->BCL ) + "ms BCL";
        break;
                
    case ProtocolStep::WAIT:
        type = "Wait ";
        description = type + " : " + QString::number( step->waitTime ) + "ms";
        break;

    case ProtocolStep::STARTMODEL:
        description = "Start Model Cell Voltage Clamp";
        break;

    case ProtocolStep::STOPMODEL:
        description = "Stop Model Cell Voltage Clamp";
        break;

    case ProtocolStep::RESETMODEL:
        description = "Reset Model Cell to Initial Conditions";
        break;
    }

    return description;
}