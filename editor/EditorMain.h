/***************************************************************
 * Name:      EditorMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ammar Qammaz (ammarkov+rgbd@gmail.com)
 * Created:   2013-10-22
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#ifndef EDITORMAIN_H
#define EDITORMAIN_H

//(*Headers(EditorFrame)
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/slider.h>
#include <wx/statusbr.h>
#include <wx/statbox.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)

class EditorFrame: public wxFrame
{
    public:

        EditorFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~EditorFrame();

        void onIdle(wxIdleEvent& evt);
        void guiSnapFrames(int doSnap);
        int  removeOldSegmentedFrames();
        int  refreshSegmentedFrame();

        unsigned char * rgbFrame;
        unsigned short * depthFrame;

        int feed_0_x,feed_0_y,feed_1_x,feed_1_y,feed_2_x,feed_2_y,feed_3_x,feed_3_y;
        int mouse_x,mouse_y;
        int add_new_track_point;

        int recording;
        int recordedFrames;

        int framesDrawn;
        int framesSnapped;

    private:

        void OnOpenModule(wxCommandEvent& event);
        void OnSavePCD(wxCommandEvent& event);
        void OnSaveDepth(wxCommandEvent& event);

        void OnButtonGetExtrinsics(wxCommandEvent& event);

        //(*Handlers(EditorFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnTimerTrigger(wxTimerEvent& event);
        void OnbuttonPlayClick(wxCommandEvent& event);
        void OnbuttonStopClick(wxCommandEvent& event);
        void OnbuttonPreviousFrameClick(wxCommandEvent& event);
        void OnbuttonNextFrameClick(wxCommandEvent& event);
        void OncurrentFrameTextCtrlText(wxCommandEvent& event);
        void OnFrameSliderCmdScroll(wxScrollEvent& event);
        void OnButtonSegmentationClick(wxCommandEvent& event);
        void OnButtonCalibrationClick(wxCommandEvent& event);
        void OnbuttonRecordClick(wxCommandEvent& event);
        void OnButtonAcquisitionGraphClick(wxCommandEvent& event);
        void OnButtonAddClick(wxCommandEvent& event);
        void OnButtonRemoveClick(wxCommandEvent& event);
        void OnButtonExecuteClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(EditorFrame)
        static const long ID_SLIDER1;
        static const long ID_STATICBOX1;
        static const long ID_STATICBOX2;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_BUTTON4;
        static const long ID_STATICTEXT1;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICTEXT3;
        static const long ID_BUTTON5;
        static const long ID_BUTTON6;
        static const long ID_BUTTON7;
        static const long ID_BUTTON8;
        static const long ID_LISTCTRL1;
        static const long ID_BUTTON9;
        static const long ID_BUTTON10;
        static const long ID_BUTTON11;
        static const long ID_LISTCTRL2;
        static const long ID_BUTTON12;
        static const long ID_MENUOPENMODULE;
        static const long ID_MENUSAVEDEPTH;
        static const long ID_MENUSAVEPCD;
        static const long idMenuQuit;
        static const long ID_MENUSEGMENTATION;
        static const long ID_MENUGETEXTRINSICS;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(EditorFrame)
        wxButton* ButtonExecute;
        wxButton* buttonNextFrame;
        wxStatusBar* Status;
        wxStaticText* totalFramesLabel;
        wxButton* buttonRecord;
        wxButton* Button1;
        wxButton* ButtonAdd;
        wxButton* buttonStop;
        wxStaticBox* StaticBox2;
        wxTextCtrl* currentFrameTextCtrl;
        wxStaticText* StaticTextJumpTo;
        wxStaticText* dashForFramesRemainingLabel;
        wxMenuItem* MenuItem3;
        wxButton* buttonPreviousFrame;
        wxSlider* FrameSlider;
        wxMenu* Menu4;
        wxListCtrl* ListCtrl1;
        wxStaticBox* StaticBoxVideoFeed;
        wxButton* buttonPlay;
        wxButton* ButtonAcquisitionGraph;
        wxMenuItem* MenuItem5;
        wxTimer Timer;
        wxListCtrl* ListCtrlPoints;
        wxButton* ButtonCalibration;
        wxButton* ButtonRemove;
        wxMenuItem* MenuItem7;
        wxMenuItem* MenuItem4;
        wxMenuItem* MenuItem6;
        wxButton* ButtonSegmentation;
        //*)

        void render(wxDC& dc);
        void OnPaint(wxPaintEvent& evt);
        void paintNow();
        void OnMotion(wxMouseEvent& event);

        DECLARE_EVENT_TABLE()
};

#endif // EDITORMAIN_H
