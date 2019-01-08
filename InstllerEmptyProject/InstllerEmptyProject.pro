TEMPLATE = aux

INSTALLER = Rsm_install.exe

QT_BIN_DIR= F:\Qt\Qt5.12.0\5.12.0\msvc2015_64\bin

INPUT = $$PWD/config/config.xml $$PWD/packages
makeInstaller.input = INPUT
makeInstaller.output = $$INSTALLER
makeInstaller.commands += lrelease $$PWD/../TopTest.pro
makeInstaller.commands +=& windeployqt.exe $$OUT_PWD/../bin/TopTestApp.exe
makeInstaller.commands +=& copy /y  $$replace(OUT_PWD,/,\\)\..\bin\*.* $$replace(PWD,/,\\)\packages\cn.rsm.toptest\data
makeInstaller.commands +=& del  $$replace(PWD,/,\\)\packages\cn.rsm.toptest\data\RSMConfigure*.*
makeInstaller.commands +=& copy /y $$replace(PWD,/,\\)\..\*.qm  $$replace(PWD,/,\\)\packages\cn.rsm.toptest\data\translations
makeInstaller.commands +=& copy /y $$replace(PWD,/,\\)\..\*.qm  $$replace(OUT_PWD,/,\\)\..\bin\translations
makeInstaller.commands +=& windeployqt.exe $$PWD/packages/cn.rsm.toptest/data/TopTestApp.exe
#makeInstaller.commands +=& xcopy /y $$QT_BIN_DIR\Qt5PrintSupport.dll $$install_data_dir
#makeInstaller.commands +=& xcopy /y $$QT_BIN_DIR\Qt5Charts.dll $$install_data_dir
makeInstaller.commands +=& copy /y $$QT_BIN_DIR\Qt5Network.dll $$replace(PWD,/,\\)\packages\cn.rsm.toptest\data
makeInstaller.commands +=& H:/Qt/QtIFW-3.0.4/bin/binarycreator.exe -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT} -v

#make reposity
makeInstaller.commands +=& H:/Qt/QtIFW-3.0.4/bin/repogen.exe --update-new-components -p $$PWD/packages $$PWD/repository
makeInstaller.CONFIG += target_predeps no_link combine

QMAKE_EXTRA_COMPILERS += makeInstaller

OTHER_FILES = README
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DISTFILES += \
    packages/cn.rsm.toptest/meta/installscript.js \
    packages/cn.rsm.toptest/meta/package.xml \
    packages/cn.rsm.toptest/data/README \
    config/config.xml \
    packages/cn.rsm.toptest/meta/Rsm1.0.lic \
    packages/FTDI/meta/installscript.js \
    packages/FTDI/meta/package.xml \
    packages/FTDI/meta/FTDI2.0.lic

#QT_BIN_DIR=G:\Qt\Qt5.9.3\5.9.3\msvc2017_64\bin


####update language
##QMAKE_POST_LINK += lupdate -silent $$ClabsoSource_Dir $$Clabso_Dir -ts $$ClabsoSource_Dir/clabso_ft_en.ts &
##QMAKE_POST_LINK += lupdate -silent $$ClabsoSource_Dir $$Clabso_Dir -ts $$ClabsoSource_Dir/clabso_ft_zh_CN.ts &
#QMAKE_POST_LINK += lrelease $$replace(PWD,/,\\)\..\TopTest.pro

#install_meta_dir = $$replace(OUT_PWD,/,\\)\..\installer\packages\cn.rsm.toptest\meta
#install_data_dir = $$replace(OUT_PWD,/,\\)\..\installer\packages\cn.rsm.toptest\data
#install_configure_dir = $$replace(OUT_PWD,/,\\)\..\installer\configure
#install_package_dir = $$replace(OUT_PWD,/,\\)\..\installer\packages
####depoly and copy dlls
#QMAKE_POST_LINK +=& echo $$install_data_dir
#QMAKE_POST_LINK +=& md $$install_data_dir & md $$install_meta_dir & md $$install_configure_dir
#QMAKE_POST_LINK +=& xcopy /y $$replace(OUT_PWD,/,\\)\..\bin\*.* $$install_data_dir
#QMAKE_POST_LINK +=& windeployqt.exe $$install_data_dir\TopTestApp.exe
#QMAKE_POST_LINK +=& xcopy /y $$replace(PWD,/,\\)\..\*.qm $$install_data_dir\translations
#QMAKE_POST_LINK +=& xcopy /y $$QT_BIN_DIR\Qt5PrintSupport.dll $$install_data_dir
#QMAKE_POST_LINK +=& xcopy /y $$QT_BIN_DIR\Qt5Charts.dll $$install_data_dir
#QMAKE_POST_LINK +=& xcopy /y $$QT_BIN_DIR\Qt5SerialPort.dll $$install_data_dir



#####create installer
#QMAKE_POST_LINK +=& xcopy /y $$replace(PWD,/,\\)\configure.xml  $$install_configure_dir
#QMAKE_POST_LINK +=& xcopy /y $$replace(PWD,/,\\)\package.xml  $$install_meta_dir
#QMAKE_POST_LINK +=& xcopy /y $$replace(PWD,/,\\)\installscript.js  $$install_meta_dir
#QMAKE_POST_LINK +=& H:\Qt\QtIFW-3.0.4\bin\binarycreator.exe --offline-only -c $$install_configure_dir\configure.xml -p $$install_package_dir Rsm_install.exe -v

#QMAKE_POST_LINK +=& echo end backup package






