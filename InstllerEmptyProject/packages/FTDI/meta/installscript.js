/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


function Component()
{
//    installer.installationFinished.connect(this, Component.prototype.installationFinishedPageIsShown);
//    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);
}

Component.prototype.createOperations = function()
{
    // call default implementation to actually install README.txt!
    component.createOperations();
    component.addOperation("CreateShortcut", "@TargetDir@/drive/setup.exe", "@StartMenuDir@/安装Rsm炉温测试仪驱动.lnk");

//    QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/drive/setup.exe");
//    console.log("Go to install drive")
//    try {
//        if (systemInfo.currentCpuArchitecture === "i386") {
//            QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/drive/DPInst_Monx86.exe", installer.value("TargetDir")+"/drive/");
//        }
//        else if(systemInfo.currentCpuArchitecture === "x86_64")
//        {
//            component.addOperation("Execute", "{512}",installer.value("TargetDir") + "\\drive\\DPInstx64.exe","workingdirectory=" + installer.value("TargetDir") + "\\drive");
////            QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/drive/DPInstx64.exe");
////            QProcess.start(installer.value("TargetDir") + "/drive/DPInst_Monx64.exe");
//        }
//    } catch(e) {
//        console.log(e);
//    }
}

//Component.prototype.installationFinished = function()
//{
//    console.log("Go to install drive")
//    try {
//        if (installer.isInstaller() && installer.status === QInstaller.Success)
//        {
////            if (systemInfo.currentCpuArchitecture === "i386") {
////                QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/drive/DPInst_Monx86.exe", "workingdirectory=" + installer.value("TargetDir") + "/drive/");
////            }
////            else if(systemInfo.currentCpuArchitecture === "x86_64")
////            {
//////            QProcess.start(installer.value("TargetDir") + "/drive/DPInst_Monx64.exe");
//////                component.addOperation("Execute", installer.value("TargetDir") + "\\drive\\DPInstx64.exe","workingdirectory=" + installer.value("TargetDir") + "\\drive");
//////                    QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/drive/DPInst_Monx64.exe");
////            }
////            component.addOperation("Execute", installer.value("TargetDir") + "\\drive\\setup.exe",
////                                   "workingdirectory=" + installer.value("TargetDir") + "\\drive");
//            QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/drive/setup.exe");
//        }
//    } catch(e) {
//        console.log(e);
//    }
//}
