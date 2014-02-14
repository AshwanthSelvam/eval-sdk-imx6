/*
 * Ridgerun
 * 	2013 efernandez edison.fernandez@ridgerun.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#include "rr_imx6_demo.h"
#include "ui_rr_imx6_demo.h"
#include <stdio.h>

rr_imx6_demo::rr_imx6_demo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rr_imx6_demo)
{
    ui->setupUi(this);
    QWidget::showFullScreen();

    // Enable mouse tracking to detect its movement
    setMouseTracking(true);

    tools = new Tools();
     _Pipeline = new pipelines();

     timer = new QTimer(this);
     _HideControlsTimer =  new QTimer(this);
     QObject::connect(timer,SIGNAL(timeout()),this,SLOT(UpdateCPU()));
     timer->start(1000);

     // Connect timeout timer
     QObject::connect(_HideControlsTimer,SIGNAL(timeout()),this,SLOT(ControlsTimeOut()));

    // Init Variables
    InitVariables();

    // Init Layout
    InitLayout();

    // Populate display List
    InitDisplayList();

    // Hide combobox
    DisableCombobox();
    HideCombobox();
    HideOverlayControl();
    HidePlayControls();
    SetMotionEnableVisible(false);

    // Transparency to 0
    tools->set_alpha(255,0);
}

rr_imx6_demo::~rr_imx6_demo()
{
    delete ui;
}

/**
  * Retart all control and status variables to its default state
  */
void rr_imx6_demo::RestartEnvironment(){

    // Destroy all pipelines
    if(_PipelineCount > 0){
        destroyPipelines();
    }

    // Message
    SetMessage("Pipelines Destroyed...",rr_imx6_demo::INFO,"REPLACE");

    // Initialize variables
    _pipelinePath.clear();  // Clear paths
    _pipelineList.clear();  // Clear pipeline list
    _isPlaying.clear();     // Clear playing status
    _isPlaying << false;
    _gstPipeline.clear();   // Clear pipelines
    _CurrentCombobox = 0;   // Set default combobox
    _PipelineCount  = 0;    // Reset piepline counter
    _ShownPipeline = 0;
    // Audio Control
    _AudioInUse = false;
    _AudioRequest = false;

    // Camera Control
    _CameraInUse = false;
    _CameraRequest = false;

    // Hide components
    HideCombobox();
    HideOverlayControl();
    ShowPlayControls(); // to set icons and everything
    HidePlayControls();
    SetMotionEnableVisible(false);

    // Show Main Controls
    ShowControls();

    // Set alpha
    tools->set_alpha(255,0);

    // Message
    SetMessage("Environment Restarted...",rr_imx6_demo::INFO,"APPEND");

}

/**
  * Initializes all the variables
  */
void rr_imx6_demo::InitVariables(){
    // Init Constantants
    _VideoTestOptions << "Camera Loopback"
                      << "From File"
                      << "Video Test Pattern";
    _Cameras          << "Camera 0"
#ifdef ENABLE_2CAMERAS      // Define number of cameras
                     << "Camera 1";
#else
                         ;
#endif
    _Overlays         << "Full Screen"
                      << "Overlay";
    _Patterns         << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9"
                      << "10" << "11" << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20";
    _Videos           << "Video 1080p"
                      << "Video 720p"
                      << "Video 480p";

    // Init box list
    _ComboList << ui->comboBox
               << ui->comboBox_2
               << ui->comboBox_3;
    _CurrentCombobox = 0;

    // Combobox Labels
    _ComboTitle << ui->label_2
                << ui->label_3
                << ui->label_4
                << ui->label_5;

    _ComboboxContent << rr_imx6_demo::NONE <<
                        rr_imx6_demo::NONE <<
                        rr_imx6_demo::NONE <<
                        rr_imx6_demo::NONE;

    _PipelineCount  = 0;
    _isPlaying << false;
    _ShownPipeline = 0;


    // Audio Control
    _AudioInUse = false;
    _AudioRequest = false;

    // Camera Control
    _CameraInUse = false;
    _CameraRequest = false;

    // Controls are visible
    _controls_hiden = false;

    /*** Init Streaming variables ***/

    // Client IP Address
    if(!tools->get_key("/usr/share/rr-imx6-demo.config","CLIENT_IP_ADDRESS", _CLIEN_IP))
        tools->PrintToStdOUT("*****Filed to load Clients IP address...");

    // Port
    if(!tools->get_key("/usr/share/rr-imx6-demo.config","PORT", _PORT))
        tools->PrintToStdOUT("*****Filed to load PORT...");

    // Camera sercer client pipeline
    if(!tools->get_key("/usr/share/rr-imx6-demo.config","CAMERA_CLIENT", _CameraClientPipe))
        tools->PrintToStdOUT("*****Filed to load Camera Pipeline...");
    else _CameraClientPipe.replace("$PORT", _PORT);

    // Video streaming client pipeline
    if(!tools->get_key("/usr/share/rr-imx6-demo.config","VIDEO_CLIENT", _VideoClientPipe))
        tools->PrintToStdOUT("*****Filed to load Camera Pipeline...");
    else _VideoClientPipe.replace("$PORT", _PORT);

    // Pattern streaming client pipeline
    if(!tools->get_key("/usr/share/rr-imx6-demo.config","PATTERN_CLIENT", _PatternClientPipe))
        tools->PrintToStdOUT("*****Filed to load Pattern Pipeline...");
    else _PatternClientPipe.replace("$PORT", _PORT);

    // Get board IP address
    _BoardIPAddress = tools->GetIpAddress();    // Get Board IP Address

    // Streaming Server Pipeline
    if(!tools->get_key("/usr/share/rr-imx6-demo.config","STREAMING_SERVER_PIPELINE", _ServerPipe))
        tools->PrintToStdOUT("*****Filed to load Server Pipeline...");
    else{
        _ServerPipe.replace("$PORT", _PORT);
        _ServerPipe.replace("$BOARD_IP", _BoardIPAddress);
    }

    // Streaming client caps
    if(!tools->get_key("/usr/share/rr-imx6-demo.config","STREAM_CLIENT_CAPS", _stream_client_caps))
        tools->PrintToStdOUT("*****Filed to load client  caps...");

    // Get Snapshot path
    if(!tools->get_key("/usr/share/rr-imx6-demo.config","SNAPSHOT_PATH", _SnapshotPath))
        tools->PrintToStdOUT("*****Filed to load snapshot path...");

    // Get motion detection plugin pipe
    if(!tools->get_key("/usr/share/rr-imx6-demo.config","MOTION_PIPE", _motionPipe))
        tools->PrintToStdOUT("*****Filed to load motion detection pipe...");

    // Get Screen Count
    if(!tools->get_key("etc/available-screens","AVAILABLE_SCREENS", _screenCount)){
        tools->PrintToStdOUT("*****Filed to load number of available screens, assuming 1...");
        _screenCount = "1";
    }
    else
        tools->PrintToStdOUT("Number of available screens: " + _screenCount);

    // Load display names
    QString currentName;
    bool is_hdmi = false; // HDMI flag
    bool is_ldb = false; // ldb flag
    for (int i=0; i<_screenCount.toInt(); i++){
        if (tools->get_key("etc/available-screens","DISP" + tools->to_string(i) + "_name", currentName)){ // Search for DISPn_name
            _DisplayNames << currentName;

            // Update HDMI and LDB flags
            if (currentName == "hdmi") is_hdmi = true;
            else if (currentName == "ldb") is_ldb = true;
        }
    }

    /* Identify configurations to be used based on the available displays
     *
     * The configuration used is:
     * Display0 -> mon1
     * Display1 -> If HDMI and LBD displays are present use mon2.2, if not use mon2
     * Display3 -> mon3
     *
     * The complete configuration file is located at /usr/share/vssconfig and it's used by the mfw_isink plugin to know what buffer has to be
     * Used.
     * If HDMI and LDB displays are present at the same time a different configuration needs to be used for the ldb display becouse they share the same IPU and
     * for that reason only the HDMI gets both a BG and a FG buffer modifying the frame buffer assignation.*/
    _mon1 = "mon1";
    _mon2 = (is_hdmi == true && is_ldb == true)? "mon2.2" : "mon2";
    _mon3 = "mon3";



}

/**
  * Initializes the layout
  */
void rr_imx6_demo::InitLayout(){

    // Get window size
    qdw = QApplication::desktop();
    win = qdw->availableGeometry(0);
    //QWidget::resize(win.width(),win.height());

    // Move Test Ccontrol buttons
    int offset = win.width() / 20;
    ui->close_button->move(win.width() - win.width() / 20 - ui->close_button->width(), win.height() / 12);
    ui->TestControlDock->resize(win.width() - 2 * offset, 20);
    ui->TestControlDock->move(offset,win.height()/2 - ui->TestControlDock->height()/2);

    // Pipeline control Buttons
    int button_size = win.width() / 13;
    ui->Backward->resize(button_size, button_size);
    ui->Backward->move(win.width() / 2 - 2 * ui->Backward->width(), win.height() - 1.2 * button_size);
    ui->PlayPause->resize(button_size, button_size);
    ui->PlayPause->move(win.width() / 2 - ui->PlayPause->width() / 2, win.height() - 1.2 * button_size);
    ui->Forward->resize(button_size, button_size);
    ui->Forward->move(win.width() / 2 + ui->Forward->width(), win.height() - 1.2 * button_size);
    ui->Backward->hide();
    ui->PlayPause->hide();
    ui->Forward->hide();
    // Stop Pipelines
    ui->StopPipelines_Button->move(win.width() / 20, win.height() - 1.2 * button_size);
    ui->StopPipelines_Button->hide();

    // Shortcuts
    ui->ShorcutsLabel->move(3 * win.width() / 5, win.height() / 10 - 30);
    ui->shorcuts_frame->move(3 * win.width() / 5, win.height() / 10);
    ui->shorcuts_frame->resize(200,190);


    // Move Combobox
    ui->comboBox->move(win.width() / 20, win.height()/2 + 50);
    ui->label_2->move(ui->comboBox->x(),ui->comboBox->y() - 20);
    ui->comboBox_2->move(ui->comboBox->x() + ui->comboBox->width() + 40,  win.height()/2 + 50);
    ui->label_3->move(ui->comboBox_2->x(),ui->comboBox_2->y() - 20);
    ui->comboBox_3->move(ui->comboBox_2->x() + ui->comboBox_2->width() + 40,  win.height()/2 + 50);
    ui->label_4->move(ui->comboBox_3->x(),ui->comboBox_3->y() - 20);
    ui->x_res->move(win.width() - win.width() / 20 - 110,  win.height()/2 + 40);
    ui->label_7->move(win.width() - win.width() / 20 - 110, ui->comboBox_3->y() - 20);
    ui->x_res_label->move(win.width() - win.width() / 20 - 110, ui->comboBox_3->y() + 50);
    ui->y_res->move(win.width() - win.width() / 20 - 50,  win.height()/2 + 40);
    ui->label_8->move(win.width() - win.width() / 20 - 50, ui->comboBox_3->y() - 20);
    ui->y_res_label->move(win.width() - win.width() / 20 - 50, ui->comboBox_3->y() + 50);
    ui->x_pos->move(win.width() - win.width() / 20 - 240,  win.height()/2 + 40);
    ui->label_5->move(win.width() - win.width() / 20 - 240, ui->comboBox_3->y() - 20);
    ui->x_pos_label->move(win.width() - win.width() / 20 - 240, ui->comboBox_3->y() + 50);
    ui->y_pos->move(win.width() - win.width() / 20 - 180,  win.height()/2 + 40);
    ui->label_6->move(win.width() - win.width() / 20 - 180, ui->comboBox_3->y() - 20);
    ui->y_pos_label->move(win.width() - win.width() / 20 - 180, ui->comboBox_3->y() + 50);

    // Adjust Dials limits
    ui->x_pos->setMaximum(win.width());
    ui->x_res->setMaximum(win.width());
    ui->x_res->setValue(win.width());
    ui->x_res_label->setText(QString::number(win.width()));
    ui->y_pos->setMaximum(win.height());
    ui->y_res->setMaximum(win.height());
    ui->y_res->setValue(win.height());
    ui->y_res_label->setText(QString::number(win.height()));


    // Alpha Control
    ui->AlphaValue->move(win.width() - win.width() / 4, ui->StopPipelines_Button->y() + 10);
    ui->TransLabel->move(win.width() - win.width() / 4  - 20, ui->StopPipelines_Button->y() - 10);

    // Move Status Window
    ui->StatusLabel->move(win.width() / 20,win.height() / 10 - 20);
    ui->Messages->move(win.width() / 20,win.height() / 10);
    ui->Messages->resize(win.width() / 3, win.height() / 4);
    ui->PipelineBox->move(win.width() / 20,ui->StopPipelines_Button->y() - 1.5 * ui->PipelineBox->height());
    ui->PipelineBox->resize(win.width() - win.width() / 10, 40);
    ui->PipelineBox->hide();

    // Move CPU Status Labels
    ui->CPU0_load->move(ui->Messages->x() + ui->Messages->width() + 20,win.height() / 10);
    ui->CPU1_load->move(ui->Messages->x() + ui->Messages->width() + 20,ui->CPU0_load->y() + 20);
    ui->CPU2_load->move(ui->Messages->x() + ui->Messages->width() + 20,ui->CPU1_load->y() + 20);
    ui->CPU3_load->move(ui->Messages->x() + ui->Messages->width() + 20,ui->CPU2_load->y() + 20);
    ui->CPUUsageLabel->move(ui->CPU0_load->x(),ui->StatusLabel->y());

    // Motion Detection checkbox
    ui->EnableMotionDetection->move(ui->Messages->x() + ui->Messages->width() + 20, ui->CPU3_load->y() + 40);
    ui->EnableMotionDetection->setVisible(false);

    // RR Logo
    ui->image->resize(700, 200);
    ui->image->move((win.width() - 700) / 2, win.height()/3);



}

/**
  * Initialize Available Display list by using the _DisplayNames variable
  */
void rr_imx6_demo::InitDisplayList(){
    // Clear List
    ui->Out_Display->clear();

    // Populate List
    ui->Out_Display->addItems(_DisplayNames);
}


/**
  *Hide Main Combobox
  */
void rr_imx6_demo::HideCombobox(){
    ui->comboBox->hide();
    ui->label_2->hide();
    ui->comboBox_2->hide();
    ui->label_3->hide();
    ui->comboBox_3->hide();
    ui->label_4->hide();
}


/**
  * Hide the selected Combobox
  *@param index combobox index
  */
void rr_imx6_demo::HideCombobox(int index){

    switch(index){
        case 0:
            ui->comboBox->hide();
            ui->label_2->hide();
            break;
        case 1:
            ui->comboBox_2->hide();
            ui->label_3->hide();
            break;
        case 2:
            ui->comboBox_3->hide();
            ui->label_4->hide();
            break;
        case 3:
            ui->label_5->hide();
            break;
    }
}


/**
  * Disable Combobox
  */
void rr_imx6_demo::DisableCombobox(){
    ui->comboBox->setEnabled(false);
    ui->comboBox_2->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
}

/**
  * Set MotionDetection Enable Visible/invisible
  **/

void rr_imx6_demo::SetMotionEnableVisible(bool visible){
#ifdef ENABLE_MOTION_DETECTION
        ui->EnableMotionDetection->setVisible(visible);
#endif
}

/**
  * Hide main controls
  */
void rr_imx6_demo::HideControls(){
    ui->close_button->hide();
    ui->StatusLabel->hide();
    ui->Messages->hide();
    ui->ShorcutsLabel->hide();
    ui->shorcuts_frame->hide();
    ui->TestControlDock->hide();
    HidePlayControls();
    _controls_hiden = true;
}


/**
  * Hide Play Controls
  */
void rr_imx6_demo::HidePlayControls(){
    ui->Backward->hide();
    ui->PlayPause->hide();
    ui->Forward->hide();
    ui->PipelineBox->hide();
    ui->StopPipelines_Button->hide();
    ui->TransLabel->hide();
    ui->AlphaValue->hide();
}


/**
  * Show Main controls
  */
void rr_imx6_demo::ShowControls(){
    ui->close_button->show();
    ui->StatusLabel->show();
    ui->Messages->show();
    ui->ShorcutsLabel->show();
    ui->shorcuts_frame->show();
    ui->TestControlDock->show();
    _controls_hiden = false;
}


/**
  * Show Play/Pause Forwar, Backward buttons
  */
void rr_imx6_demo::ShowPlayControls(){
    ui->PipelineBox->show();
    ui->PlayPause->show();
    ui->Backward->show();
    ui->Forward->show();
    ui->StopPipelines_Button->show();
    ui->PlayPause->setEnabled(true);
    if(_PipelineCount > 0){ // At least one pipeline in execution
        if(_ShownPipeline >= 0) ui->Backward->setEnabled(true);
        else ui->Backward->setEnabled(false);
        if(_ShownPipeline < _PipelineCount - 1) ui->Forward->setEnabled(true);
        else {
            ui->Forward->setEnabled(false);
            ui->PlayPause->setIcon(QIcon(":/Play.jpg"));   // Change Icon
        }
    }
    else{ // No pipelines created
        ui->Backward->setEnabled(false);
        ui->Forward->setEnabled(false);
    }
    ui->TransLabel->show();
    ui->AlphaValue->show();
}

/**
  *Hide the Dials
  */
void rr_imx6_demo::HideOverlayControl(){
    ui->x_pos->hide();
    ui->y_pos->hide();
    ui->x_res->hide();
    ui->y_res->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    ui->x_pos_label->hide();
    ui->y_pos_label->hide();
    ui->x_res_label->hide();
    ui->y_res_label->hide();
}

/**
  *Show the Dials
  */
void rr_imx6_demo::ShowOverlayControl(){
    ui->x_pos->show();
    ui->y_pos->show();
    ui->x_res->show();
    ui->y_res->show();
    ui->label_5->show();
    ui->label_6->show();
    ui->label_7->show();
    ui->label_8->show();
    ui->x_pos_label->show();
    ui->y_pos_label->show();
    ui->x_res_label->show();
    ui->y_res_label->show();
}

/**
  * Update the content of the PipelineBox with the current pipeline content
  */
void rr_imx6_demo::UpdatePipelineBox(){
    if(_ShownPipeline == _PipelineCount) // Displayed pipeline is the last created
        ui->PipelineBox->setText(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element);
    else
        ui->PipelineBox->setText(_pipelineList.at(_ShownPipeline));

    // Start timeout timer
    if(_PipelineCount > 0) _HideControlsTimer->start(10000);
}

/**
  * Set the desired message with the apropriate color acording to the type
  *@param message The message to be displayed
  *@param type The type of message (INFO, WARNING, ERROR)
  *@param action Replace existing text or append message (REPLACE, APPEND)
  */
void rr_imx6_demo::SetMessage(QString message, MESSAGE_TYPE type, QString action){
    if(action == "REPLACE"){
        switch(type){
            case rr_imx6_demo::INFO:
                ui->Messages->setTextColor(QColor("blue"));
                ui->Messages->setText("INFO: ");
                break;
            case rr_imx6_demo::WARNING:
                ui->Messages->setTextColor(QColor("yellow"));
                ui->Messages->setText("WARNING: ");
                break;
            case rr_imx6_demo::ERROR:
                ui->Messages->setTextColor(QColor("red"));
                ui->Messages->setText("ERROR: ");
                break;
        }
    }
    else{
        switch(type){
            case rr_imx6_demo::INFO:
                ui->Messages->setTextColor(QColor("blue"));
                ui->Messages->append("INFO: ");
                break;
            case rr_imx6_demo::WARNING:
                ui->Messages->setTextColor(QColor("yellow"));
                ui->Messages->append("WARNING: ");
                break;
            case rr_imx6_demo::ERROR:
                ui->Messages->setTextColor(QColor("red"));
                ui->Messages->append("ERROR: ");
                break;
        }
    }
    ui->Messages->setTextColor(QColor("white"));
    ui->Messages->append(message);
}

/**
  * Detects when the mouse moves
  */
void rr_imx6_demo::mouseMoveEvent( QMouseEvent * event ){
    if (_PipelineCount > 0 && _controls_hiden == true) { // Only if at least one pipeline has been created and the controls are hiden
        ShowControls();
        ShowPlayControls();
        tools->set_alpha(ui->AlphaValue->value(),0);
        // Start timeout timer
        _HideControlsTimer->start(10000);
    }
}

/**
  * Enable and shows the Combobox menu according to the menu type
  */
void rr_imx6_demo::EnableCombobox(QComboBox *combobox, QLabel* title, MENU_TYPE menu){

    // Clear previous content
    combobox->clear();

    // Add new Items
    switch(menu){
    case rr_imx6_demo::VIDEO_TEST:
        combobox->addItems(_VideoTestOptions);
        title->setText("Select Video Test");
        break;
    case rr_imx6_demo::CAMERA_SELECTION:
        combobox->addItems(_Cameras);
        title->setText("Select Camera");
        break;
    case rr_imx6_demo::OVERLAY_SELECTION:
        combobox->addItems(_Overlays);
        title->setText("Select Overlay");
        break;
    case rr_imx6_demo::PATTERN_SELECTION:
        combobox->addItems(_Patterns);
        title->setText("Select Pattern");
        break;
    case rr_imx6_demo::VIDEO_SELECTION:
        combobox->addItems(_Videos);
        title->setText("Select Video");
        break;
    case rr_imx6_demo::NONE:
        break;
    }

    // Enable and show combobox
    combobox->show();
    title->show();
    combobox->setEnabled(true);
}


/*GSTD functions*/

bool rr_imx6_demo::createPipeline(QString pipelineDesc, int idx){

    if(idx == _PipelineCount){
         /* Create proxy object to use factory interface */
         _gstFactory = new GstFactory("com.ridgerun.gstreamer.gstd",
                                       "/com/ridgerun/gstreamer/gstd/factory",
                                       QDBusConnection::systemBus(), this);

         if (!_gstFactory->isValid()){
           // ui->errorLabel->setText("ERROR: Failed to create GstFactory");
            return false;
        }

        /* Create pipeline path */
        _pipelinePath << _gstFactory->Create(pipelineDesc);
        _pipelineList << pipelineDesc;  // Add pipeline to list

        /* Create proxy object to use pipeline interface */
        _gstPipeline << new GstPipeline("com.ridgerun.gstreamer.gstd",
                                        _pipelinePath[_PipelineCount],
                                         QDBusConnection::systemBus(), this);

        if (!(_gstPipeline[_PipelineCount]->isValid())){
          //  ui->errorLabel->setText("ERROR: Failed to create GstPipeline");
            return false;
        }

        _gstPipeline[_PipelineCount]->PipelineSetState(PIPELINE_PAUSE);

        // Increase pipeline counter
        _PipelineCount++;
        _ShownPipeline++;

        return true;
    }
    else return false;
}

void rr_imx6_demo::destroyPipelines(){
    _gstFactory->DestroyAll();
}

/**
  * Start selected pipeline
  */
void rr_imx6_demo::playPipeline(int idx){
    if(idx < _PipelineCount){
        _gstPipeline.at(idx)->PipelineSetState(PIPELINE_PLAY);
    }
}

/**
  *Pauses selected pipeline
  */
void rr_imx6_demo::pausePipeline(int idx){
    if(idx < _PipelineCount){
        _gstPipeline.at(idx)->PipelineSetState(PIPELINE_PAUSE);
    }
}

/**
  * Update Output Display accordingg to selection
  */
void rr_imx6_demo::UpdateOutputDisplay(){
    /* Find and occurrence of display=mon*
    and replace it according to the selection*/
    QRegExp rx("display=mon[0-9]{1,1}\\.?[0-9]?\\b");
    QString after;



    // Identify selection
    switch(ui->Out_Display->currentIndex()){
    case 0:
        after="display=" + _mon1;
        break;
    case 1:
        after="display=" + _mon2;
        break;
    case 2:
        after="display=" + _mon3;
        break;
    default:
        after="display=" + _mon1;
        break;
    }

    // Update pipeline
    _CurrentPipeline.Sink_element.replace(rx,after);

}

/**
  * Enable Video Test Options
  */
void rr_imx6_demo::on_vide_test_button_clicked()
{
    // Audio
    _AudioRequest = false;
    // Camera
    _CameraRequest = true;

    // Update Status Message
    SetMessage("Video Test selected, \n"
               "Select the desired configuration and press PLAY to start...", INFO, "REPLACE");

    // Hide Combobox
    HideCombobox();
    // Hide Sliders
    HideOverlayControl();
    // Set First Combobox
    _CurrentCombobox = 0;
    _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::VIDEO_TEST;

    // Enable Video Test Options
    EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  VIDEO_TEST);
    _CurrentCombobox = 1;
    // Enable Camera Options
    EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  CAMERA_SELECTION);
    _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::CAMERA_SELECTION;
    _CurrentCombobox = 2;
    // Enable Overlay Options
    EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  OVERLAY_SELECTION);
    _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::OVERLAY_SELECTION;
    _CurrentCombobox = 3;

    // Set default pipeline
    _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0";
#ifdef ENABLE_MOTION_DETECTION
    _CurrentPipeline.Src_element += " ! " + _motionPipe;
#endif
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1";

    //Set shown pipeline as the last one
    _ShownPipeline = _PipelineCount;

    // Update Output display
    UpdateOutputDisplay();

    // Update PiepelineBox
    UpdatePipelineBox();

    // Enable Play buttons
    ShowPlayControls();

    // Show EnableMotionDetection option
    SetMotionEnableVisible(true);

    // Update Pipeline Status
    _CurrentPipeline.test = pipelines::VIDEO;
}


/**
  * Enable Streaming Server Options
  */
void rr_imx6_demo::on_stream_server_button_clicked()
{
    _AudioRequest = false;

    // Camera
    _CameraRequest = true;

    // Update Status Message
    SetMessage("Streaming Server Test selected,\n"
               "Select the desired configuration and press PLAY to start...", INFO, "REPLACE");
    SetMessage("Please run the clients pipeline before to start...", WARNING, "APPEND");

    // Hide Combobox
    HideCombobox();
    // Hide Dials
    HideOverlayControl();
    // Set First Combobox
    _CurrentCombobox = 0;

    // Enable Streaming Options
    EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  VIDEO_TEST);
    _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::VIDEO_TEST;
    _CurrentCombobox = 1;
    // Enable Camera Options
    EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  CAMERA_SELECTION);
    _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::CAMERA_SELECTION;
    _CurrentCombobox = 2;

    // Set Default Pipeline
    _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0 ! vpuenc codec=6 ! queue ! rtph264pay";
    _CurrentPipeline.Sink_element = "udpsink host=";
    _CurrentPipeline.Sink_element.append(_CLIEN_IP);
    _CurrentPipeline.Sink_element.append(" port=" + _PORT);
    _CurrentPipeline.StreamClientPipe = _CameraClientPipe;  // Init Client Pipeline

    // Print Clients Pipeline
    tools->PrintToStdOUT("\nClients pipeline:\n");
    tools->PrintToStdOUT(_CurrentPipeline.StreamClientPipe);

    //Set shown pipeline as the last one
    _ShownPipeline = _PipelineCount;

    // Update PiepelineBox
    UpdatePipelineBox();

    // Enable Play buttons
    ShowPlayControls();

    // Show EnableMotionDetection option
    SetMotionEnableVisible(false);

    _CurrentPipeline.test = pipelines::STREAMING_SERVER;
}

/**
  * Enable Streaming Client Options
  */
void rr_imx6_demo::on_stream_client_button_clicked()
{
    _AudioRequest = false;

    // Camera
    _CameraRequest = false;

    // Update Status Message
    SetMessage("Streaming Client Test selected,", INFO, "REPLACE");
    SetMessage("Please run the server pipeline...", INFO, "APPEND");

    // Hide Combobox
    HideCombobox();
    // Hide Sliders
    HideOverlayControl();
    // Set First Combobox
    _CurrentCombobox = 0;

    // Enable Overlay Options
    EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  OVERLAY_SELECTION);
    _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::OVERLAY_SELECTION;
    _CurrentCombobox = 1;

    //Set Piepline
    _CurrentPipeline.Src_element = "udpsrc port=";
    _CurrentPipeline.Src_element.append(_PORT + " ! capsfilter caps=" + _stream_client_caps + " ! rtph264depay ! queue ! vpudec");
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 sync=false";

    //Set shown pipeline as the last one
    _ShownPipeline = _PipelineCount;

    // Update Output display
    UpdateOutputDisplay();

    // Update PiepelineBox
    UpdatePipelineBox();

    // Enable Play buttons
    ShowPlayControls();

    // Show EnableMotionDetection option
    SetMotionEnableVisible(false);

    _CurrentPipeline.test = pipelines::STREAMING_CLIENT;
}

/**
  * Enable Snapshot Options
  */
void rr_imx6_demo::on_snapshot_button_clicked()
{
    _AudioRequest = false;

    // Camera
    _CameraRequest = true;

    // Update Status Message
    SetMessage("Snapshot Test selected,\n"
               "Select a camera and press PLAY to take the snapshot...", INFO, "REPLACE");

    // Hide Combobox
    HideCombobox();
    // Hide Sliders
    HideOverlayControl();
    // Set First Combobox
    _CurrentCombobox = 0;

    // Enable Overlay Options
    EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  CAMERA_SELECTION);
    _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::CAMERA_SELECTION;
    _CurrentCombobox = 1;

    //Set Piepline
    _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0 num-buffers=1 ! vpuenc codec=12";
    _CurrentPipeline.Sink_element = "multifilesink location=";
    _CurrentPipeline.Sink_element.append(_SnapshotPath);

    //Set shown pipeline as the last one
    _ShownPipeline = _PipelineCount;

    // Update PiepelineBox
    UpdatePipelineBox();

    // Enable Play buttons
    ShowPlayControls();

    // Show EnableMotionDetection option
    SetMotionEnableVisible(false);

    _CurrentPipeline.test = pipelines::SNAPSHOT;
}


/**
  * Updates the video sink acording to the current overlay
  * and dials status.
  */
void rr_imx6_demo::UpdateVideoSink(){
    if(_CurrentPipeline.test == pipelines::VIDEO){
        if(ui->comboBox_3->currentIndex() == 0) { // Full screen
          if(_ComboboxContent[1] == rr_imx6_demo::VIDEO_SELECTION) {// Add audio part for video playback
            _CurrentPipeline.Sink_element = "mfw_isink display=mon1 demux. ! queue ! mpegaudioparse ! beepdec ! audioconvert ! alsasink";
            // Audio
            _AudioRequest = true;
          }
          else
            _CurrentPipeline.Sink_element = "mfw_isink display=mon1";
        }
        else{ // OVerlay
            _CurrentPipeline.Sink_element = "mfw_isink display=mon1 ";
            _CurrentPipeline.Sink_element.append("axis-left=" + QString::number(ui->x_pos->value()));
            _CurrentPipeline.Sink_element.append(" axis-top=" + QString::number(ui->y_pos->value()));
            _CurrentPipeline.Sink_element.append(" disp-width=" + QString::number(ui->x_res->value()));
            _CurrentPipeline.Sink_element.append(" disp-height=" + QString::number(ui->y_res->value()));
        }
    }
    else if(_CurrentPipeline.test == pipelines::STREAMING_CLIENT){
        if(ui->comboBox->currentIndex() == 0)  // Full screen
            _CurrentPipeline.Sink_element = "mfw_isink display=mon1 sync=false";
        else{ // Overlay
            _CurrentPipeline.Sink_element = "mfw_isink display=mon1 ";
            _CurrentPipeline.Sink_element.append("axis-left=" + QString::number(ui->x_pos->value()));
            _CurrentPipeline.Sink_element.append(" axis-top=" + QString::number(ui->y_pos->value()));
            _CurrentPipeline.Sink_element.append(" disp-width=" + QString::number(ui->x_res->value()));
            _CurrentPipeline.Sink_element.append(" disp-height=" + QString::number(ui->y_res->value()));
            _CurrentPipeline.Sink_element.append(" sync=false");
        }
    }
    // Update Output display
    UpdateOutputDisplay();
}

/**
  *First Combobox options
  */
void rr_imx6_demo::on_comboBox_activated(int index)
{
    // Audio
    _AudioRequest = false;
    // Camera
    _CameraRequest = false;

    // Don't enable  motion detection
    _EnableMotion = false;

    switch(_CurrentPipeline.test){
        case pipelines::VIDEO:
            // Hide Combobox 1
             HideCombobox(1);
            _CurrentCombobox = 1;
            if(index == 0){ // Camera Options
                EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  CAMERA_SELECTION);
                _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::CAMERA_SELECTION;
                _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0";
            #ifdef ENABLE_MOTION_DETECTION
                _CurrentPipeline.Src_element += " ! " + _motionPipe;
            #endif
                // Camera request flag
                _CameraRequest = true;
                // Enable Motion Detection
                _EnableMotion = true;
            }
            else if(index == 1){ // Video Preview Options
                EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  VIDEO_SELECTION);
                _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::VIDEO_SELECTION;
                _CurrentPipeline.Src_element = "filesrc location=";
                _CurrentPipeline.Src_element.append(_Pipeline->preview_1080p_video);
                _CurrentPipeline.Src_element.append(" typefind=true ! aiurdemux name=demux ! queue ! vpudec");
            }
            else{ // Video Test Pattern Options
                EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  PATTERN_SELECTION);
                _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::PATTERN_SELECTION;
                _CurrentPipeline.Src_element = "videotestsrc";
            }
            _CurrentCombobox = 3;
            break;
        case pipelines::STREAMING_SERVER:
            // Hide Combobox 1
             HideCombobox(1);
            _CurrentCombobox = 1;
            if(index == 0){ // Camera Options
                EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  CAMERA_SELECTION);
                _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::CAMERA_SELECTION;
                // Set Default Pipeline
                _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0 ! vpuenc codec=6 ! queue ! rtph264pay";
                _CurrentPipeline.Sink_element = "udpsink host=";
                _CurrentPipeline.Sink_element.append(_CLIEN_IP);
                _CurrentPipeline.Sink_element.append(" port=" + _PORT);
                _CurrentPipeline.StreamClientPipe = _CameraClientPipe;  // Init Client Pipeline

            }
            else if(index == 1){ // Video Preview Options
                // Set Default Pipeline
                _CurrentPipeline.Src_element = "filesrc location=";
                _CurrentPipeline.Src_element.append(_Pipeline->preview_1080p_video + " ! qtdemux ! rtph264pay");
                _CurrentPipeline.Sink_element = "udpsink host=";
                _CurrentPipeline.Sink_element.append(_CLIEN_IP);
                _CurrentPipeline.Sink_element.append(" port=" + _PORT);
                _CurrentPipeline.StreamClientPipe = _VideoClientPipe;  // Init Client Pipeline

            }
            else{ // Video Test Pattern Options
                EnableCombobox(_ComboList.at(_CurrentCombobox),_ComboTitle.at(_CurrentCombobox),  PATTERN_SELECTION);

                //Set Default Pipeline
                _CurrentPipeline.Src_element = "videotestsrc pattern=0 ! vpuenc  codec=6 ! queue ! h264parse ! rtph264pay";
                _CurrentPipeline.Sink_element = "udpsink host=";
                _CurrentPipeline.Sink_element.append(_CLIEN_IP);
                _CurrentPipeline.Sink_element.append(" port=" + _PORT);
                 _CurrentPipeline.StreamClientPipe = _PatternClientPipe;  // Init Client Pipeline
                _ComboboxContent[_CurrentCombobox] = rr_imx6_demo::PATTERN_SELECTION;
            }
            _CurrentCombobox = 2;
            // Print Clients Pipeline
            tools->PrintToStdOUT("\nClients pipeline:\n");
            tools->PrintToStdOUT(_CurrentPipeline.StreamClientPipe);
            break;
        case pipelines::STREAMING_CLIENT:
            // Hide Combobox 1
             HideCombobox(1);
             HideCombobox(2);
             if(index == 0) { // Full screen
                 _CurrentPipeline.Sink_element = "mfw_isink display=mon1 sync=false";
                 HideOverlayControl();
             }
             else{ // Overlay
                 ShowOverlayControl();
             }
            _CurrentCombobox = 1;
            break;
        case pipelines::SNAPSHOT:
            if(index == 0) _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0 num-buffers=1 ! vpuenc codec=12";
            else _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video1 num-buffers=1 ! vpuenc codec=12";
            // Camera request flag
            _CameraRequest = true;
            break;
    }

    // Update video sink
    UpdateVideoSink();

    //Set shown pipeline as the last one
    _ShownPipeline = _PipelineCount;

    // Set Enable Motion Control  status
    SetMotionEnableVisible(_EnableMotion);

    // Update PiepelineBox
    UpdatePipelineBox();
}

/**
  * Second Combobox options
  */
void rr_imx6_demo::on_comboBox_2_activated(int index)
{
    // Audio
    _AudioRequest = false;
    // Camera
    _CameraRequest = false;

    // Disable Motion Detection
    _EnableMotion = false;

    switch(_CurrentPipeline.test){
        case pipelines::VIDEO:
            switch(_ComboboxContent.at(1)){
                case rr_imx6_demo::CAMERA_SELECTION:
                    if(index == 0) {
                        _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0";
                    #ifdef ENABLE_MOTION_DETECTION
                        _CurrentPipeline.Src_element += " ! " + _motionPipe;
                    #endif
                    }
                    else {
                        _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video1";
                    #ifdef ENABLE_MOTION_DETECTION
                        _CurrentPipeline.Src_element += " ! " + _motionPipe;
                    #endif
                    }
                    // Camera flag
                    _CameraRequest = true;
                    // Show EnableMotionDetection option
                    _EnableMotion = true;
                    break;
                case rr_imx6_demo::VIDEO_SELECTION:
                    _CurrentPipeline.Src_element = "filesrc location=";
                    if(index == 0)_CurrentPipeline.Src_element.append(_Pipeline->preview_1080p_video);
                    else if(index == 1)_CurrentPipeline.Src_element.append(_Pipeline->preview_720p_video);
                    else _CurrentPipeline.Src_element.append(_Pipeline->preview_480p_video);
                    _CurrentPipeline.Src_element.append(" typefind=true ! aiurdemux name=demux ! queue ! vpudec");
                    break;
                case rr_imx6_demo::PATTERN_SELECTION:
                    _CurrentPipeline.Src_element = "videotestsrc pattern=";
                    _CurrentPipeline.Src_element.append(QString::number(index));
                    break;
                    // Defaults
            case rr_imx6_demo::VIDEO_TEST:
            case rr_imx6_demo::OVERLAY_SELECTION:
            case rr_imx6_demo::NONE:
                break;
            }
            break;
        case pipelines::STREAMING_SERVER:
            switch(_ComboboxContent.at(1)){
                case rr_imx6_demo::CAMERA_SELECTION:
                    if(index == 0)  _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0 ! vpuenc codec=6 ! queue ! rtph264pay";
                    else  _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video1 ! vpuenc codec=6 ! queue ! rtph264pay";
                    // Camera flag
                    _CameraRequest = true;
                    break;
                case rr_imx6_demo::PATTERN_SELECTION:
                    _CurrentPipeline.Src_element = "videotestsrc pattern=";
                    _CurrentPipeline.Src_element.append(QString::number(index) + " ! vpuenc  codec=6 ! queue ! h264parse ! rtph264pay");
                    break;
                    // Defaults
            case rr_imx6_demo::VIDEO_TEST:
            case rr_imx6_demo::OVERLAY_SELECTION:
            case rr_imx6_demo::VIDEO_SELECTION:
            case rr_imx6_demo::NONE:
                break;
            }
            break;
            //defaults
    case pipelines::STREAMING_CLIENT:
    case pipelines::SNAPSHOT:
        break;
    }

    // Update video sink
    UpdateVideoSink();

    //Set shown pipeline as the last one
    _ShownPipeline = _PipelineCount;

    // Set Enable Motion Control  status
    SetMotionEnableVisible(_EnableMotion);

    // Update PiepelineBox
    UpdatePipelineBox();
}


/**
  *Third combobox options
  */
void rr_imx6_demo::on_comboBox_3_activated(int index)
{
    // Audio
    _AudioRequest = false;
    // Camera
    _CameraRequest = false;

    if(index == 0) { // Full screen
        if(_ComboboxContent[1] == rr_imx6_demo::VIDEO_SELECTION){
            _CurrentPipeline.Sink_element = "mfw_isink display=mon1 demux. ! queue ! mpegaudioparse ! beepdec ! audioconvert ! alsasink";

            // Audio
            _AudioRequest = true;
        }
        else
            _CurrentPipeline.Sink_element = "mfw_isink display=mon1";

        HideOverlayControl();
    }
    else{ // Overlay
        _CurrentPipeline.Sink_element = "mfw_isink display=mon1 axis-left=";
        _CurrentPipeline.Sink_element.append(QString::number(ui->x_pos->value()));
        _CurrentPipeline.Sink_element.append(" axis-top=");
        _CurrentPipeline.Sink_element.append(QString::number(ui->y_pos->value()));
        _CurrentPipeline.Sink_element.append(" disp-width=");
        _CurrentPipeline.Sink_element.append(QString::number(ui->x_res->value()));
        _CurrentPipeline.Sink_element.append(" disp-height=");
        _CurrentPipeline.Sink_element.append(QString::number(ui->y_res->value()));
        ShowOverlayControl();
    }

    //Set shown pipeline as the last one
    _ShownPipeline = _PipelineCount;

    // Update Output display
    UpdateOutputDisplay();

    // Set Enable Motion Control  status
    SetMotionEnableVisible(false);

    // Update PiepelineBox
    UpdatePipelineBox();
}



/**
  * Close application
  */
void rr_imx6_demo::on_close_button_clicked()
{
    if(_PipelineCount > 0)  destroyPipelines();
    exit(0);
}


// Pipeline Control Buttons

/**
  * Play/Pause control
  */
void rr_imx6_demo::on_PlayPause_clicked()
{
    int idx = _ShownPipeline;

    if(idx == (_PipelineCount)){ // Create new pipeline
        if((!_AudioRequest || !_AudioInUse) && (!_CameraRequest || !_CameraInUse)){
            ui->PlayPause->setIcon(QIcon(":/Play.jpg"));   // Change Icon
            createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
            playPipeline(idx);
            _isPlaying[idx] = true;

            // Audio Flag
            if(_AudioRequest) _AudioInUse = true;

            // Camera Flag
            if(_CameraRequest) _CameraInUse = true;

            // Update pipeline box
            ui->PipelineBox->setText(_pipelineList.at(idx));

            // Set the Camera index to be used for motion detection enable/disable
            if(_EnableMotion) _CameraPipelineIndex = idx;       // Save the index

        }
        else{
            if(_AudioRequest) SetMessage("Audio in use...", rr_imx6_demo::ERROR, "APPEND");
            if(_CameraRequest) SetMessage("Camera in use...", rr_imx6_demo::ERROR, "APPEND");
        }

        if(_CurrentPipeline.test == pipelines::STREAMING_CLIENT){ // Display the Server pipeline
            tools->PrintToStdOUT("\nServer pipeline:\n");
            tools->PrintToStdOUT(_ServerPipe);
        }
    }
    else{
        if(_isPlaying.at(idx) == true){ // Pause
            ui->PlayPause->setIcon(QIcon(":/Play.jpg"));   // Change Icon
            pausePipeline(idx);
            _isPlaying[idx] = false;
        }
        else{ //Play
            ui->PlayPause->setIcon(QIcon(":/Pause.jpg"));   // Change Icon
            playPipeline(idx);
            _isPlaying[idx] = true;
        }
        // Update pipeline box
        ui->PipelineBox->setText(_pipelineList.at(idx));
    }
    HideControls(); // Hide Controls
    HideCombobox(); // Hide Combobox
    tools->set_alpha(0,0); // Maximum transparency

}

/**
  *Backwards control
  */
void rr_imx6_demo::on_Backward_clicked()
{
    if(_ShownPipeline > 0){
        _ShownPipeline--;
        UpdatePipelineBox();
        ui->Forward->setEnabled(true);
    }
    if(_ShownPipeline == 0) ui->Backward->setEnabled(false);

    // Update Play/Pause Button
    if(_isPlaying.at(_ShownPipeline) == true){ // Pause
        ui->PlayPause->setIcon(QIcon(":/Pause.jpg"));   // Change Icon
    }
    else{ //Play
        ui->PlayPause->setIcon(QIcon(":/Play.jpg"));   // Change Icon
    }
}

/**
  * Forwards Control
  */
void rr_imx6_demo::on_Forward_clicked()
{
    if(_ShownPipeline < (_PipelineCount)){
        _ShownPipeline++;
        UpdatePipelineBox();
        ui->Backward->setEnabled(true);
    }
    if(_ShownPipeline == (_PipelineCount)){
        ui->Forward->setEnabled(false);
        ui->PlayPause->setIcon(QIcon(":/Play.jpg"));   // Change Icon
    }
    else{
        // Update Play/Pause Button
        if(_isPlaying.at(_ShownPipeline) == true){ // Pause
            ui->PlayPause->setIcon(QIcon(":/Pause.jpg"));   // Change Icon
        }
        else{ //Play
            ui->PlayPause->setIcon(QIcon(":/Play.jpg"));   // Change Icon
        }
    }
}

/**
  *Stop Pipelines
  */
void rr_imx6_demo::on_StopPipelines_Button_clicked()
{
    // Restart the environment
    RestartEnvironment();
}

// Sliders control

void rr_imx6_demo::on_x_pos_sliderMoved(int position)
{
    ui->x_pos_label->setText(QString::number(position));
    ui->x_res->setMaximum(1024 - position);
    ui->x_res_label->setText(QString::number(ui->x_res->value()));

    // Update sink element
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 axis-left=";
    _CurrentPipeline.Sink_element.append(QString::number(ui->x_pos->value()));
    _CurrentPipeline.Sink_element.append(" axis-top=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->y_pos->value()));
    _CurrentPipeline.Sink_element.append(" disp-width=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->x_res->value()));
    _CurrentPipeline.Sink_element.append(" disp-height=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->y_res->value()));
   if(_CurrentPipeline.test == pipelines::STREAMING_CLIENT) _CurrentPipeline.Sink_element.append(" sync=false");

   //Set shown pipeline as the last one
   _ShownPipeline = _PipelineCount;

   // Update Output display
   UpdateOutputDisplay();

   // Update PiepelineBox
   UpdatePipelineBox();
}

void rr_imx6_demo::on_y_pos_sliderMoved(int position)
{
    ui->y_pos_label->setText(QString::number(position));
    ui->y_res->setMaximum(768 - position);
    ui->y_res_label->setText(QString::number(ui->y_res->value()));

    // Update sink element
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 axis-left=";
    _CurrentPipeline.Sink_element.append(QString::number(ui->x_pos->value()));
    _CurrentPipeline.Sink_element.append(" axis-top=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->y_pos->value()));
    _CurrentPipeline.Sink_element.append(" disp-width=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->x_res->value()));
    _CurrentPipeline.Sink_element.append(" disp-height=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->y_res->value()));
    if(_CurrentPipeline.test == pipelines::STREAMING_CLIENT)_CurrentPipeline.Sink_element.append(" sync=false");

    //Set shown pipeline as the last one
    _ShownPipeline = _PipelineCount;

    // Update Output display
    UpdateOutputDisplay();

    // Update PiepelineBox
    UpdatePipelineBox();
}

void rr_imx6_demo::on_x_res_sliderMoved(int position)
{
    ui->x_res_label->setText(QString::number(position));

    // Update sink element
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 axis-left=";
    _CurrentPipeline.Sink_element.append(QString::number(ui->x_pos->value()));
    _CurrentPipeline.Sink_element.append(" axis-top=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->y_pos->value()));
    _CurrentPipeline.Sink_element.append(" disp-width=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->x_res->value()));
    _CurrentPipeline.Sink_element.append(" disp-height=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->y_res->value()));
    if(_CurrentPipeline.test == pipelines::STREAMING_CLIENT)_CurrentPipeline.Sink_element.append(" sync=false");

    //Set shown pipeline as the last one
    _ShownPipeline = _PipelineCount;

    // Update Output display
    UpdateOutputDisplay();

    // Update PiepelineBox
    UpdatePipelineBox();
}

void rr_imx6_demo::on_y_res_sliderMoved(int position)
{
    ui->y_res_label->setText(QString::number(position));

    // Update sink element
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 axis-left=";
    _CurrentPipeline.Sink_element.append(QString::number(ui->x_pos->value()));
    _CurrentPipeline.Sink_element.append(" axis-top=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->y_pos->value()));
    _CurrentPipeline.Sink_element.append(" disp-width=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->x_res->value()));
    _CurrentPipeline.Sink_element.append(" disp-height=");
    _CurrentPipeline.Sink_element.append(QString::number(ui->y_res->value()));
    if(_CurrentPipeline.test == pipelines::STREAMING_CLIENT)_CurrentPipeline.Sink_element.append(" sync=false");

    //Set shown pipeline as the last one
    _ShownPipeline = _PipelineCount;

    // Update Output display
    UpdateOutputDisplay();

    // Update PiepelineBox
    UpdatePipelineBox();
}

/**
  *Update CPU Usage labels
  */
void rr_imx6_demo::UpdateCPU(){
     tools->UPDATE_CPU_USEGE(); // Update Values
     // Display Values
     ui->CPU0_load->setValue(tools->cpu_usage[0]);
     ui->CPU1_load->setValue(tools->cpu_usage[1]);
     ui->CPU2_load->setValue(tools->cpu_usage[2]);
     ui->CPU3_load->setValue(tools->cpu_usage[3]);
}


/**
  * Hide Controls because of time out
  */
void rr_imx6_demo::ControlsTimeOut(){
    tools->set_alpha(0,0);
    _HideControlsTimer->stop();
    _controls_hiden = true;
}



void rr_imx6_demo::on_AlphaValue_valueChanged(int value)
{
    tools->set_alpha(value,0);
}




  // Shortcut buttons

/**
* Play a movie
*/
void rr_imx6_demo::on_toolButton_clicked()
{
    // Restart the environment
    RestartEnvironment();
    int idx = 0;

    // Set pipeline
    _CurrentPipeline.Src_element = "filesrc location=";
    _CurrentPipeline.Src_element.append(_Pipeline->preview_1080p_video);
    _CurrentPipeline.Src_element.append(" typefind=true ! aiurdemux name=demux ! queue ! vpudec");
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 demux. ! queue ! mpegaudioparse ! beepdec ! audioconvert ! alsasink";

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;
    // Update Pipeline box
    ui->PipelineBox->setText(_pipelineList.at(idx));

    //Show Play Controls
    ShowPlayControls();

    // Set Audio Flag
    _AudioInUse = true;

    HideControls(); // Hide Controls
    HideCombobox(); // Hide Combobox
    tools->set_alpha(0,0); // Maximum transparency

    // Set Enable Motion Control  status
    SetMotionEnableVisible(false);

}

/**
  * Live Preview
  */
void rr_imx6_demo::on_LivePrevieShortcut_clicked()
{
    // Restart the environment
    RestartEnvironment();
    int idx = 0;

    // Set pipeline
    _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0";
#ifdef ENABLE_MOTION_DETECTION
    _CurrentPipeline.Src_element += " ! " + _motionPipe;
#endif
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1";

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;
    // Update Pipeline box
    ui->PipelineBox->setText(_pipelineList.at(idx));

    //Show Play Controls
    ShowPlayControls();

    // Set Audio Flag
    _CameraInUse = true;
    _CameraPipelineIndex = idx;

    HideControls(); // Hide Controls
    HideCombobox(); // Hide Combobox
    tools->set_alpha(0,0); // Maximum transparency

    // Set Enable Motion Control  status
    SetMotionEnableVisible(true);
}


/**
  * Live Preview + Movie
  */
void rr_imx6_demo::on_LivPMovie_Shortcut_clicked()
{
    // Restart the environment
    RestartEnvironment();
    int idx = 0;

    // Set pipeline
    _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0";
#ifdef ENABLE_MOTION_DETECTION
    _CurrentPipeline.Src_element += " ! " + _motionPipe;
#endif
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1";

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;


    idx = _ShownPipeline;

    // Set pipeline
    _CurrentPipeline.Src_element = "filesrc location=";
    _CurrentPipeline.Src_element.append(_Pipeline->preview_1080p_video);
    _CurrentPipeline.Src_element.append(" typefind=true ! aiurdemux name=demux ! queue ! vpudec");
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 disp-width=320 disp-height=240 demux. ! queue ! mpegaudioparse ! beepdec ! audioconvert ! alsasink";

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;
    // Update Pipeline box
    ui->PipelineBox->setText(_pipelineList.at(idx));

    //Show Play Controls
    ShowPlayControls();

    // Set Audio Flag
    _CameraInUse = true;
    _AudioInUse = true;
    _CameraPipelineIndex = 0;

    HideControls(); // Hide Controls
    HideCombobox(); // Hide Combobox
    tools->set_alpha(0,0); // Maximum transparency

    // Set Enable Motion Control  status
    SetMotionEnableVisible(true);
}

/**
  * Movie + live preview
  */
void rr_imx6_demo::on_MoviePLive_shortcut_clicked()
{
    // Restart the environment
    RestartEnvironment();
    int idx = 0;

    // Set pipeline
    _CurrentPipeline.Src_element = "filesrc location=";
    _CurrentPipeline.Src_element.append(_Pipeline->preview_1080p_video);
    _CurrentPipeline.Src_element.append(" typefind=true ! aiurdemux name=demux ! queue ! vpudec");
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 demux. ! queue ! mpegaudioparse ! beepdec ! audioconvert ! alsasink";

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;


    idx = _ShownPipeline;

    // Set pipeline
    _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0";
#ifdef ENABLE_MOTION_DETECTION
    _CurrentPipeline.Src_element += " ! " + _motionPipe;
#endif
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 disp-width=320 disp-height=240";

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;
    // Update Pipeline box
    ui->PipelineBox->setText(_pipelineList.at(idx));

    //Show Play Controls
    ShowPlayControls();

    // Set Audio Flag
    _CameraInUse = true;
    _AudioInUse = true;
    _CameraPipelineIndex = 1; // Second pipeline is camera

    HideControls(); // Hide Controls
    HideCombobox(); // Hide Combobox
    tools->set_alpha(0,0); // Maximum transparency

    // Set Enable Motion Control  status
    SetMotionEnableVisible(true);
}

/**
  * Live Preview + 2 videos
  */
void rr_imx6_demo::on_LiveP2Video_Shortcut_clicked()
{
    // Restart the environment
    RestartEnvironment();
    int idx = 0;
    // Set video dimensions according to window size
    int _width = win.height()/ 2;
    int _height = _width * 3 / 4; // 4:3 ratio
    QString videoWidth = QString::number(_width);
    QString videoHeight = QString::number(_height);

    // Set pipeline
    _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0";
#ifdef ENABLE_MOTION_DETECTION
    _CurrentPipeline.Src_element += " ! " + _motionPipe;
#endif
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1";

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;


    idx = _ShownPipeline;

    // Set pipeline
    _CurrentPipeline.Src_element = "filesrc location=";
    _CurrentPipeline.Src_element.append(_Pipeline->preview_1080p_video);
    _CurrentPipeline.Src_element.append(" typefind=true ! aiurdemux name=demux ! queue ! vpudec");
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 disp-width=" + videoWidth + " disp-height=" + videoHeight;

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;

    idx = _ShownPipeline;

    // Set pipeline
    _CurrentPipeline.Src_element = "filesrc location=";
    _CurrentPipeline.Src_element.append(_Pipeline->preview_720p_video);
    _CurrentPipeline.Src_element.append(" typefind=true ! aiurdemux name=demux ! queue ! vpudec");
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 axis-left=" + videoWidth + " axis-top="+ videoHeight + " disp-width=" + videoWidth +
                                " disp-height=" + videoHeight +  " demux. ! queue ! mpegaudioparse ! beepdec ! audioconvert ! alsasink";

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;
    // Update Pipeline box
    ui->PipelineBox->setText(_pipelineList.at(idx));

    //Show Play Controls
    ShowPlayControls();

    // Set Audio Flag
    _CameraInUse = true;
    _AudioInUse = true;
    _CameraPipelineIndex = 0; // First pipeline is camera

    HideControls(); // Hide Controls
    HideCombobox(); // Hide Combobox
    tools->set_alpha(0,0); // Maximum transparency

    // Set Enable Motion Control  status
    SetMotionEnableVisible(true);
}

/**
  * Live Preview + 3 videos
  */
void rr_imx6_demo::on_LiveP3Video_Shortcut_clicked()
{
    // Restart the environment
    RestartEnvironment();
    int idx = 0;
    // Set video dimensions according to window size
    int _width = win.height()/ 3;
    int _height = _width * 3 / 4; // 4:3 ratio
    QString videoWidth = QString::number(_width);
    QString videoHeight = QString::number(_height);
    QString video3Left = QString::number(2 * _width);
    QString video3Top = QString::number(2 * _height);

    // Set pipeline
    _CurrentPipeline.Src_element = "mfw_v4lsrc device=/dev/video0";
#ifdef ENABLE_MOTION_DETECTION
    _CurrentPipeline.Src_element += " ! " + _motionPipe;
#endif
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1";

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;


    idx = _ShownPipeline;

    // Set pipeline
    _CurrentPipeline.Src_element = "filesrc location=";
    _CurrentPipeline.Src_element.append(_Pipeline->preview_1080p_video);
    _CurrentPipeline.Src_element.append(" typefind=true ! aiurdemux name=demux ! queue ! vpudec");
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 disp-width=" + videoWidth + " disp-height=" + videoHeight;

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;

    idx = _ShownPipeline;

    // Set pipeline
    _CurrentPipeline.Src_element = "filesrc location=";
    _CurrentPipeline.Src_element.append(_Pipeline->preview_480p_video);
    _CurrentPipeline.Src_element.append(" typefind=true ! aiurdemux name=demux ! queue ! vpudec");
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 axis-left=" + videoWidth + " axis-top=" + videoHeight + " disp-width=" + videoWidth +
                                    " disp-height=" + videoHeight;

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;

    idx = _ShownPipeline;

    // Set pipeline
    _CurrentPipeline.Src_element = "filesrc location=";
    _CurrentPipeline.Src_element.append(_Pipeline->preview_720p_video);
    _CurrentPipeline.Src_element.append(" typefind=true ! aiurdemux name=demux ! queue ! vpudec");
    _CurrentPipeline.Sink_element = "mfw_isink display=mon1 axis-left=" + video3Left + " axis-top=" + video3Top + " disp-width=" +
                                    videoWidth + " disp-height=" + videoHeight + " demux. ! queue ! mpegaudioparse ! beepdec ! audioconvert ! alsasink";

    // Update Output display
    UpdateOutputDisplay();

    // Start pipeline
    createPipeline(_CurrentPipeline.Src_element + " ! " + _CurrentPipeline.Sink_element, idx);
    playPipeline(idx);
    _isPlaying[idx] = true;
    // Update Pipeline box
    ui->PipelineBox->setText(_pipelineList.at(idx));

    //Show Play Controls
    ShowPlayControls();

    // Set Audio Flag
    _CameraInUse = true;
    _AudioInUse = true;
    _CameraPipelineIndex = 0;  // Pipeline 0 is camera

    HideControls(); // Hide Controls
    HideCombobox(); // Hide Combobox
    tools->set_alpha(0,0); // Maximum transparency

    // Set Enable Motion Control  status
    SetMotionEnableVisible(true);
}


// Set the Motion detection plugin status
void rr_imx6_demo::on_EnableMotionDetection_toggled(bool checked)
{
    setMotionDetect(checked);
}


// Set Motion detection status
void rr_imx6_demo::setMotionDetect(bool status){
    if(_PipelineCount > 0 && _CameraInUse){
        _gstPipeline.at(_CameraPipelineIndex)->ElementSetPropertyBoolean("motion_plugin", "enable", status);
    }
}

void rr_imx6_demo::on_Out_Display_activated(int index)
{
    // Update Output display
    UpdateOutputDisplay();

    // Update Piline Box
    UpdatePipelineBox();
}
