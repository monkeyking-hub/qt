#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("Flight System");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Flight Systems Inc.");
    
    // 设置应用程序样式
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // 加载自定义样式表
    QFile styleFile(":/styles/darkstyle.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream textStream(&styleFile);
        QString style = textStream.readAll();
        app.setStyleSheet(style);
    }
    
    MainWindow window;
    window.show();
    
    return app.exec();
}