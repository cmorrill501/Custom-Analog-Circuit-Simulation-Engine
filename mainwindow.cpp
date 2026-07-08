#include "mainwindow.h"
// For logger, still need to implement warning and error message printing setup
// Want the warnings to be yellow and errors to be red ideally
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    createMainWindow();
    createRibbonMenus();
    createConsole();
    qApp->installEventFilter(this);
}
MainWindow::~MainWindow() {

}

void MainWindow::createMainWindow() {
    setWindowTitle("CircuitTree: Circuit Simulator");
    statusBar()->showMessage("Ready");
    setWindowState(Qt::WindowMaximized);
    this->raise();
    this->activateWindow();
}

#pragma region createRibbonMenus
void MainWindow::createRibbonMenus() {
    createFileMenu();
    createViewMenu();
    createSimulateMenu();
}
// May add more sub-functions to this as more functions are added to the menu
void MainWindow::createFileMenu() {
    filemenu = menuBar()->addMenu(tr("&File"));
    closeAction = new QAction(tr("&Close"),this);
    closeAction->setShortcut(QKeySequence::Quit);
    closeAction->setStatusTip(tr("Exit the application"));
    connect(closeAction, &QAction::triggered,this,&QMainWindow::close);
    filemenu->addAction(closeAction);
}
// May add more sub-functions to this as more functions are added to the menu
void MainWindow::createViewMenu() {
    View = menuBar()->addMenu(tr("&View"));
    toggleTerminalAction = new QAction(this);
    toggleTerminalAction->setText(tr("Toggle Terminal"));
    View->addAction(toggleTerminalAction);
}
// May add more sub-functions to this as more functions are added to the menu
// Incomplete as is
void MainWindow::createSimulateMenu() {
    Simulate = menuBar()->addMenu(tr("&Simulate"));
    ConfigureAnalysis = new QAction(this);
    ConfigureAnalysis->setText(tr("Incomplete")); //Configure Analysis
    //connect()
    Simulate->addAction(ConfigureAnalysis);
}
#pragma endregion

#pragma region createConsole
void MainWindow::createConsole() {
    setupConsoleDock();
    createTabs();
    generalLayoutSetup();
    setupTerminalLayout();
    setupProblemsLayout();
    formatConsole();
    addChannelsToLayouts();
    addTabsToConsoleDock();
    addConsoleFunctionality();
}
void MainWindow::setupConsoleDock() {
    terminalDock = new QDockWidget(tr("CircuitTree Shell v0.17.4"),this);
    terminalDock->setAllowedAreas(Qt::BottomDockWidgetArea);
}
void MainWindow::createTabs() {
    commands = new QLineEdit(this);
    commands->setPlaceholderText("Type commands here, (help) for options or exit to close the program:");
    output = new QPlainTextEdit(this);
    warnings = new QPlainTextEdit(this);
    problems = new QPlainTextEdit(this);
    systemlog = new QPlainTextEdit(this);
    output->setReadOnly(true);
    warnings->setReadOnly(true);
    problems->setReadOnly(true);
    systemlog->setReadOnly(true);
}
void MainWindow::generalLayoutSetup() {
    terminalTabWidget = new QWidget(this);
    problemsTabWidget = new QWidget(this);
    terminalTabs = new QTabWidget(this);
    terminalTabs->setTabPosition(QTabWidget::North);
}
void MainWindow::setupTerminalLayout() {
    terminalLayout = new QVBoxLayout(terminalTabWidget);
    terminalLayout->setContentsMargins(2,2,2,2);
    terminalLayout->setSpacing(2);
}
void MainWindow::setupProblemsLayout() {
    problemsLayout = new QVBoxLayout(problemsTabWidget);
    problemsLayout->setContentsMargins(2,2,2,2);
    problemsLayout->setSpacing(2);
}
void MainWindow::formatConsole() {
    QPalette palette = output->palette();
    palette.setColor(QPalette::Base,Qt::black);
    QFont terminalFont("Courier New",10);
    palette.setColor(QPalette::Text,QColor(0,255,0));
    commands->setPalette(palette);
    commands->setFont(terminalFont);
    output->setPalette(palette);
    output->setFont(terminalFont);
    systemlog->setPalette(palette);
    systemlog->setFont(terminalFont);
    problems->setPalette(palette);
    problems->setFont(terminalFont);
}
void MainWindow::addChannelsToLayouts() {
    terminalLayout->addWidget(output);
    terminalLayout->addWidget(commands);
    terminalTabWidget->setLayout(terminalLayout);
    problemsLayout->addWidget(problems);
    problemsLayout->addWidget(warnings);
    problemsTabWidget->setLayout(problemsLayout);
}
void MainWindow::addTabsToConsoleDock() {
    terminalTabs->addTab(terminalTabWidget,tr("Simulation Terminal"));
    terminalTabs->addTab(problemsTabWidget,tr("Simulation Issues"));
    terminalTabs->addTab(systemlog,tr("System Log"));
    terminalDock->setWidget(terminalTabs);
    addDockWidget(Qt::BottomDockWidgetArea,terminalDock);
}
void MainWindow::addConsoleFunctionality() {
    terminalDock->setVisible(false);
    toggleTerminalAction->setShortcut(QKeySequence(Qt::Key_Slash));
    connect(toggleTerminalAction, &QAction::triggered,this,&MainWindow::toggleTerminal);
    this->addAction(toggleTerminalAction);
    connect(commands,&QLineEdit::returnPressed,this,&MainWindow::handleTerminalCommands);
}
void MainWindow::loggerPipelineConnection() {
    Logger::registerCallback([this](const std::string& message,MessageLevel level) {
        QString qtMessage = QString::fromStdString(message);
        QMetaObject::invokeMethod(this, [this, qtMessage, level]() {
            switch(level) {
                case(MessageLevel::success):
                {
                    output->appendPlainText(QString("%1").arg(qtMessage));
                    break;
                }
                case(MessageLevel::info):
                {
                    systemlog->appendPlainText(QString("%1").arg(qtMessage));
                    break;
                }
                case(MessageLevel::warning):
                {
                    QString color = "#FFBB33";
                    warnings->appendHtml(QString("<span style='color: %1; white-space: pre;'>%2</span>").arg(color,qtMessage));
                    break;
                }
                case(MessageLevel::error):
                {
                    QString color = "#FF4444";
                    problems->appendHtml(QString("<span style='color: %1; white-space: pre;'>%2</span>").arg(color,qtMessage));
                    break;
                }
            }
        }, Qt::QueuedConnection);
    });
}
#pragma endregion

void MainWindow::handleTerminalCommands() {
    QString rawInput = commands->text().trimmed();
    if (rawInput.isEmpty()) return;
    output->appendPlainText("\n"+rawInput);
    instruct(rawInput.toStdString());                        
    commands->clear();
    commands->setFocus();
}
void MainWindow::toggleTerminal() {
    bool isVisible = terminalDock->isVisible();
    terminalDock->setVisible(!isVisible);
    if(!isVisible) {
        if(terminalDock->isFloating()) {
            terminalDock->raise();
            terminalDock->activateWindow();
        }
        commands->setFocus();
    }
}
