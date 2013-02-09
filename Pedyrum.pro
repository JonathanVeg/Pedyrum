# -------------------------------------------------
# Project created by QtCreator 2012-05-29T15:23:56
# -------------------------------------------------
QT += core \
    gui \
    sql \
    webkit \
    network


TARGET = Pedyrum
TEMPLATE = app
SOURCES +=  Abstract/backupthread.cpp \
            Abstract/bdtools.cpp \
            Abstract/confnetwork.cpp \
            Abstract/datamodel.cpp \
            Abstract/encryption.cpp \
            Abstract/graphiceffects.cpp \
            Abstract/graphics.cpp \
            Abstract/graphicsreport.cpp \
            Abstract/restorethread.cpp \
            Abstract/serial.cpp \
            Abstract/textprinter.cpp \
            Abstract/tools.cpp \
            Component/betweendate.cpp \
            Component/combobox.cpp \
            Component/doublespinbox.cpp \
            Component/edit.cpp \
            Component/item.cpp \
            Component/label.cpp \
            Component/listview.cpp \
            Component/messagealert.cpp \
            Component/phoneedit.cpp \
            Component/search.cpp \
            Component/seletordate.cpp \
            Component/sidebar.cpp \
            Component/slide.cpp \
            Component/spinbox.cpp \
            Component/tableview.cpp \
            Component/topmenu.cpp \
            Component/yesno.cpp \
            Interface/accountfinanceinterface.cpp \
            Interface/alertinterface.cpp \
            Interface/clientinterface.cpp \
            Interface/configinterface.cpp \
            Interface/counterinterface.cpp \
            Interface/deliveryinterface.cpp \
            Interface/firstadmininterface.cpp \
            Interface/functionaryinterface.cpp \
            Interface/jaguatiinterface.cpp \
            Interface/logininterface.cpp \
            Interface/orderedinterface.cpp \
            Interface/payboxinterface.cpp \
            Interface/paymentview.cpp \
            Interface/pizzamixed.cpp \
            Interface/productinterface.cpp \
            Interface/selectdeliverer.cpp \
            Interface/Table/groupdesk.cpp \
            Interface/tableinterface.cpp \
            Interface/treatmentinterface.cpp \
            Interface/usermenu.cpp \
            Object/alert.cpp \
            Object/backup.cpp \
            Object/bill.cpp\
            Object/client.cpp\
            Object/config.cpp \
            Object/employee.cpp\
            Object/makehtml.cpp \
            Object/messageobject.cpp \
            Object/movementcash.cpp \
            Object/order.cpp \
            Object/ordered.cpp \
            Object/people.cpp\
            Object/pizza.cpp \
            Object/product.cpp\
            Object/size.cpp\
            pedyrum.cpp \
            main.cpp \
    Object/login.cpp \
    Object/createdatabasethread.cpp \
    Interface/Configure/config_manageuser.cpp \
    Interface/Table/treatmenttable.cpp \
    Interface/Table/media.cpp \
    Interface/Table/jointable.cpp \
    Interface/Table/transfer.cpp \
    Interface/Table/itemdesk.cpp \
    Interface/Table/dataitem.cpp \
    Interface/Table/generaldata.cpp \
    Object/debug.cpp \
    Interface/Table/dataorder.cpp \
    Interface/ads.cpp

HEADERS +=              Abstract/backupthread.h \
            Abstract/bdtools.h \
            Abstract/confnetwork.h \
            Abstract/datamodel.h \
            Abstract/encryption.h \
            Abstract/graphiceffects.h \
            Abstract/graphics.h \
            Abstract/graphicsreport.h \
            Abstract/restorethread.h \
            Abstract/serial.h \
            Abstract/textprinter.h \
            Abstract/tools.h \
            Component/betweendate.h \
            Component/combobox.h\
            Component/doublespinbox.h \
            Component/edit.h\
            Component/item.h \
            Component/label.h\
            Component/listview.h \
            Component/messagealert.h \
            Component/phoneedit.h \
            Component/search.h\
            Component/seletordate.h \
            Component/sidebar.h \
            Component/slide.h \
            Component/spinbox.h \
            Component/tableview.h \
            Component/topmenu.h \
            Component/yesno.h \
            Interface/accountfinanceinterface.h \
            Interface/alertinterface.h \
            Interface/clientinterface.h \
            Interface/configinterface.h \
            Interface/counterinterface.h \
            Interface/deliveryinterface.h \
            Interface/firstadmininterface.h \
            Interface/functionaryinterface.h \
            Interface/jaguatiinterface.h \
            Interface/logininterface.h \
            Interface/orderedinterface.h \
            Interface/payboxinterface.h \
            Interface/paymentview.h \
            Interface/pizzamixed.h \
            Interface/productinterface.h \
            Interface/selectdeliverer.h \
            Interface/Table/groupdesk.h \
            Interface/tableinterface.h \
            Interface/treatmentinterface.h \
            Interface/usermenu.h \
            Object/alert.h \
            Object/backup.h \
            Object/bill.h\
            Object/client.h\
            Object/config.h \
            Object/employee.h\
            Object/makehtml.h \
            Object/messageobject.h \
            Object/movementcash.h \
            Object/order.h \
            Object/ordered.h \
            Object/people.h\
            Object/pizza.h \
            Object/product.h \
            Object/size.h\
            pedyrum.h \
    Object/login.h \
    Object/createdatabasethread.h \
    Interface/Configure/config_manageuser.h \
    Interface/Table/treatmenttable.h \
    Interface/Table/media.h \
    Interface/Table/jointable.h \
    Interface/Table/transfer.h \
    Interface/Table/itemdesk.h \
    Interface/Table/dataitem.h \
    Interface/Table/generaldata.h \
    Object/debug.h \
    Interface/Table/dataorder.h \
    Interface/ads.h

RC_FILE += \
        pedyrum-ico.rc

CONFIG += static

FORMS += pedyrum.ui

RESOURCES += Files.qrc

OTHER_FILES += \
    notes.txt

# Defines para diferenciar o S.O.
win32 {
	WINDOWS = 1
}

unix {
	LINUX = 1
}
