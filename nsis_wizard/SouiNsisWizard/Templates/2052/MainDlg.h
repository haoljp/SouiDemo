/**
* Copyright (C) 2014-2050 SongJia
* All rights reserved.
*
* @file       MainDlg.h
* @brief	  SOUI主界面程序
* @version    v1.0
* @author     SongJia
* @date       2015-04-23
*
* Describe
*/

//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <map>
using namespace std;

#include "pluginapi.h" // nsis plugin
#include "event.h"
#include <helper/smenu.h>

#define TMR_AUTOCHANGE     10000001
extern extra_parameters *g_pluginParms;
extern HWND g_hwndParent;
extern SApplication *theApp;
extern SComMgr *pComMgr;

class CMainDlg : public SHostWnd
{
public:
    CMainDlg();
    ~CMainDlg();

    void OnClose();
    void OnMaximize();
    void OnRestore();
    void OnMinimize();
    void OnSize(UINT nType, CSize size);

    void OnBtnMsgBox();
    //托盘通知消息处理函数
    LRESULT OnIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);
    //演示如何响应菜单事件
    void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);

	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
    //切换视图
    LRESULT  OnTabSetSelectClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);
    //点击查看软件协议
    LRESULT  OnAgreementClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);
    //点击同意协议
    LRESULT OnChkAgreementClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);
    //点击自定义按钮
    LRESULT OnCustomInstallClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);
    //点击快速安装
    LRESULT OnInstallClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);

    //点击协议确认按钮
    LRESULT OnAgreementSureClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);

    //自定义安装页面-返回上一步
    LRESULT OnChoosePreviousClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);

    //自定义安装页面-安装
    void OnChooseInstallClick();
    //选择安装目录
    void OnChooseChangeDirClick();
    //更新磁盘空闲大小
    LRESULT OnUpdateHDFreeSpaceClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);
    //更新所需空间大小
    LRESULT OnUpdateRequiredSpaceClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);

    LRESULT OnStopMsgLoop(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);

    //卸载第一页-下一页
    LRESULT  OnUnstallStartNext(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);

    void OnTimer(UINT_PTR nIDEvent);

protected:
    //soui消息
    EVENT_MAP_BEGIN()
        EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
        EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
        EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
    EVENT_MAP_END()

    //HostWnd真实窗口消息处理
    BEGIN_MSG_MAP_EX(CMainDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_SIZE(OnSize)
        MSG_WM_TIMER(OnTimer) 
        //自定义消息
        MESSAGE_HANDLER(WM_SJTABSETSEL, OnTabSetSelectClick)

        MESSAGE_HANDLER(WM_SJONECHECKSTATUS, OnChkAgreementClick)
        MESSAGE_HANDLER(WM_SJONELINKAGREEMENT, OnAgreementClick)
        MESSAGE_HANDLER(WM_SJONECUSTOMINSTALL, OnCustomInstallClick)
        MESSAGE_HANDLER(WM_SJONEINSTALL, OnInstallClick)

        MESSAGE_HANDLER(WM_SJTWOSURE, OnAgreementSureClick)

        MESSAGE_HANDLER(WM_SJTHREEPREVIOUS, OnChoosePreviousClick)
        MESSAGE_HANDLER(WM_SJTHREEUPDATEFREESPACE, OnUpdateHDFreeSpaceClick)
        MESSAGE_HANDLER(WM_SJTHREEUPDATEREQUIREDSPACE, OnUpdateRequiredSpaceClick)
        MESSAGE_HANDLER(WM_SJMSGLOOPSTOP, OnStopMsgLoop)

        //卸载
        MESSAGE_HANDLER(WM_SJUNSTALLSTARTNEXT, OnUnstallStartNext)

		REFLECT_NOTIFICATIONS_EX()
		CHAIN_MSG_MAP(SHostWnd)
    END_MSG_MAP()

private:
    BOOL			m_bLayoutInited;
    //安装主导航
    STabCtrl *tab;

    ////////Welcome Page/////////
    //立刻安装按钮
    SImageButton *pbtn_install;
    //自定义安装按钮
    SImageButton *pbtn_custominstall;
    //同意协议按钮
    SCheckBox *pchk_agreement;
    //安装路径
    SEdit *pedit_choose_url;
    //需要空间大小
    SStatic *ptxt_choose_requiredspace;
    //可用空间大小
    SStatic *ptxt_choose_freespace;

    //子控件与外部脚本绑定，方便程序反向调用触发脚本函数
    // wstring : 控件名称	int: 脚本函数地址
    std::map<wstring, int> m_controlCallbackMap;


public:
    SMenu menu;

    //执行NSIS脚本
    bool DoExecuteCodeSegment(EventArgs* evt) {

        std::map<wstring, int >::iterator iter = this->m_controlCallbackMap.find(evt->nameFrom);
        if (iter != m_controlCallbackMap.end()) {
            //MessageBox(NULL, evt->nameFrom, _T(""), MB_OK);
            g_pluginParms->ExecuteCodeSegment(iter->second - 1, g_hwndParent);
        }
        return true;
    }

    //外部调用-添加子控件与外部脚本绑定
    void AddToControlCallbackMap(wstring ctlName, int callback)
    {
        m_controlCallbackMap[ctlName] = callback;
        SWindow *child = FindChildByName(ctlName.c_str());
        child->GetEventSet()->subscribeEvent(EVT_CMD, Subscriber(&CMainDlg::DoExecuteCodeSegment, this));
    }



};
