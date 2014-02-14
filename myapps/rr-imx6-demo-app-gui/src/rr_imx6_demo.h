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


#ifndef RR_IMX6_DEMO_H
#define RR_IMX6_DEMO_H

#include <QWidget>
#include <QtGui>
#include <QApplication>
#include <QtGui/QWidget>
#include <QTimer>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <QStringList>
#include <QList>

#include "tools.h"
#include "pipelines.h"
#include "gst_factory.h"
#include "gst_pipeline.h"

namespace Ui {
class rr_imx6_demo;
}

class rr_imx6_demo : public QWidget
{
    Q_OBJECT
    
public:
    explicit rr_imx6_demo(QWidget *parent = 0);
    ~rr_imx6_demo();
    
private slots:
    void on_vide_test_button_clicked();
    void on_close_button_clicked();
    void on_comboBox_activated(int index);
    void on_stream_server_button_clicked();
    void on_stream_client_button_clicked();
    void on_snapshot_button_clicked();
    void on_comboBox_2_activated(int index);
    void on_PlayPause_clicked();
    void on_Backward_clicked();
    void on_Forward_clicked();
    void on_comboBox_3_activated(int index);
    void on_x_pos_sliderMoved(int position);
    void on_y_pos_sliderMoved(int position);
    void on_x_res_sliderMoved(int position);
    void on_y_res_sliderMoved(int position);
    void UpdateCPU();
    void ControlsTimeOut();
    void on_StopPipelines_Button_clicked();
    void on_AlphaValue_valueChanged(int value);
    void on_toolButton_clicked();
    void on_LivePrevieShortcut_clicked();
    void on_LivPMovie_Shortcut_clicked();
    void on_MoviePLive_shortcut_clicked();
    void on_LiveP2Video_Shortcut_clicked();
    void on_LiveP3Video_Shortcut_clicked();

    void on_EnableMotionDetection_toggled(bool checked);

    void on_Out_Display_activated(int index);

private:
    // Window Dimensions
    QRect win;

    // Audio control
    bool _AudioInUse;
    bool _AudioRequest;

    // Camera Control
    bool _CameraInUse;
    bool _CameraRequest;

    // Motion detection control
    bool _EnableMotion;
    int _CameraPipelineIndex;

    Ui::rr_imx6_demo    *ui;
    QDesktopWidget*     qdw;
    Tools*              tools;
    QString             _pipelineDesc;
    QList<QString>      _pipelinePath;      // Paths of executed pipelines
    QList<QString>      _pipelineList;      // list of executed pipelines
    QList<bool>         _isPlaying;         // Pipelines status
    GstFactory*         _gstFactory;
    QList<GstPipeline*> _gstPipeline;       // Pipeline List
    QStringList         _VideoTestOptions,
                        _Cameras,
                        _Overlays,
                        _Patterns,
                        _Videos;
    QList<QComboBox*>   _ComboList;             // List of Combobox
    int                 _CurrentCombobox;       // Current Combobox counter (last combobox index)
    QList<QLabel*>      _ComboTitle;            //Combobox Titles
    int                 _PipelineCount;         // Number of pipelins
    int                 _ShownPipeline;         // The pipeline shown in PipelineBox

    // Streaming Variables
    QString             _CLIEN_IP;              // Client IP Adderss
    QString             _PORT;                  // Port
    QString             _CameraClientPipe;      // For streaming
    QString             _VideoClientPipe;       // For streaming
    QString             _PatternClientPipe;     // For streaming
    QString             _ServerPipe;            // For streaming
    QString             _stream_client_caps;    // For streaming
    QString             _BoardIPAddress;        // For Streaming
    QString             _SnapshotPath;          // Snapshot location
    QString             _motionPipe;            // Motion detection plugin

    // Display Control
    QList<QString>      _DisplayNames;          // List of Display Names
    QString             _screenCount;           // Number of available screens
    QString             _mon1;                  // Configuration name for display number one
    QString             _mon2;                  // Configuration name for display number two
    QString             _mon3;                  // Configuration name for display number three

    bool                _controls_hiden;         // flag to indicates the controls are hiden


    // Pipeline States
    enum PIPELINE_STATE{
        PIPELINE_VOID_PENDING,
        PIPELINE_NULL,
        PIPELINE_READY,
        PIPELINE_PAUSE,
        PIPELINE_PLAY
    };

    // Enum for the available Menus
    enum MENU_TYPE{
        VIDEO_TEST,
        CAMERA_SELECTION,
        OVERLAY_SELECTION,
        PATTERN_SELECTION,
        VIDEO_SELECTION,
        NONE
    };

    QList<MENU_TYPE>    _ComboboxContent;   // To know the current content

    // Messages
    enum MESSAGE_TYPE{
        WARNING,
        ERROR,
        INFO
    };

    pipelines::Pipeline            _CurrentPipeline; // Pipeline Status
    pipelines*                     _Pipeline;
    QTimer*  timer;                                 // For the CPU Usage
    QTimer* _HideControlsTimer;                     // To control the time the controls are shown

    void InitVariables();
    void InitLayout();
    void InitDisplayList();
    void UpdateOutputDisplay();
    void HideCombobox();
    void HideCombobox(int index);
    void DisableCombobox();
    void EnableCombobox(QComboBox* combobox, QLabel *title, MENU_TYPE menu);
    void SetMessage(QString message, MESSAGE_TYPE type, QString action);


    bool        createPipeline(QString pipelineDesc, int idx);
    void        destroyPipelines();
    void        playPipeline(int idx);
    void        pausePipeline(int idx);
    void        setMotionDetect(bool status);
    void        HideControls();
    void        HidePlayControls();
    void        ShowControls();
    void        ShowPlayControls();
    void        ShowOverlayControl();
    void        HideOverlayControl();
    void        UpdatePipelineBox();
    void        UpdateVideoSink();
    void        RestartEnvironment();
    void        SetMotionEnableVisible(bool visible);
    //void        mouseReleaseEvent( QMouseEvent * event );
    void        mouseMoveEvent(QMouseEvent *);
};

#endif // RR_IMX6_DEMO_H
