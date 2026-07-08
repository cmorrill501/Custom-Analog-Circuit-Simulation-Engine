#ifndef mainwindow_H
#define mainwindow_H

#include <QMainWindow>
#include <QScreen>
#include <QGuiApplication>
#include <QStatusBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QMenu>
#include <QAction>
#include <QDockWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QEvent>
#include <QMouseEvent>

#include "Commands.h"
// for the items in private, will probably want to use structs for organizations
class MainWindow : public QMainWindow {
    Q_OBJECT
    protected:
        // encountered difficulties when attempting to move to mainwindow.cpp but is not as big a priority as for creating the main window 
        bool eventFilter(QObject *isFocus, QEvent *event) override {
            if((event->type() == QEvent::MouseButtonPress) && (terminalDock) && (terminalDock->isVisible()) && (!(terminalDock->isFloating()))) {
                QMouseEvent *mouseClick = static_cast<QMouseEvent*>(event);
                QPoint localPos = terminalDock->mapFromGlobal(mouseClick->globalPosition().toPoint());
                if(!(terminalDock->rect().contains(localPos))) {
                    QPoint menuPos = menuBar()->mapFromGlobal(mouseClick->globalPosition().toPoint());
                    if(menuBar()->rect().contains(menuPos)) return QMainWindow::eventFilter(isFocus,event);
                    terminalDock->setVisible(false);
                }
            }
            if(event->type() == QEvent::KeyPress) {
                QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                if(keyEvent->key() == Qt::Key_Escape) {
                    if((commands->hasFocus()) || (terminalTabs->hasFocus())) {
                        if(centralWidget()) {
                            centralWidget()->setFocus();
                        } else {
                            this->setFocus();
                        }
                        terminalDock->setVisible(false);
                        return true;
                    }
                }
            }
            return QMainWindow::eventFilter(isFocus,event);
        }
    private slots:
        void handleTerminalCommands();
        void toggleTerminal();
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
    private:
        QMenu *filemenu;
        QAction *closeAction;
        QMenu *View;
        QMenu *Simulate;
        QAction *ConfigureAnalysis;

        QAction *toggleTerminalAction;

        QDockWidget *terminalDock;
        QTabWidget *terminalTabs;
        QWidget *terminalTabWidget;
        QWidget *problemsTabWidget;
        QVBoxLayout *terminalLayout;
        QVBoxLayout *problemsLayout;
        QLineEdit *commands;
        QPlainTextEdit *output;
        QPlainTextEdit *warnings;
        QPlainTextEdit *problems;
        QPlainTextEdit *systemlog; // QListWidget is another alternative

        void createMainWindow();

        void createRibbonMenus();
            void createFileMenu();
            void createViewMenu();
            void createSimulateMenu();

        void createConsole();  
            void setupConsoleDock();   
            void createTabs();  
            void generalLayoutSetup(); 
            void setupTerminalLayout();
            void setupProblemsLayout();
            void formatConsole();
            void addChannelsToLayouts();
            void addTabsToConsoleDock();
            void addConsoleFunctionality();
            void loggerPipelineConnection();
};

#endif