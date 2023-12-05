
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QPixmap>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {

    ui->setupUi(this);
    this->setWindowTitle("AED Plus");
    powerStatus = 0;
    // 1 = Pass, 0 = Fail
    statusIndicator = 1;
    step = 0;

    //Initialize image on power button
    initializePowerButtonImage(ui->powerButton, ":/res/Buttons/powerButton.svg", 50, 50);

    // Initialize images on labels
    initializeLabelImage(ui->step1, ":/res/AEDSteps/Step1.png", 150, 150);
    initializeLabelImage(ui->step2, ":/res/AEDSteps/Step2.png", 150, 150);
    initializeLabelImage(ui->step3, ":/res/AEDSteps/Step3.png", 150, 150);
    initializeLabelImage(ui->step4, ":/res/AEDSteps/Step4.png", 150, 150);
    initializeLabelImage(ui->step5, ":/res/AEDSteps/Step5.png", 200, 150);
    initializeLabelImage(ui->shockInd, ":/res/AEDSteps/ShockInd.png", 78, 78);
//    makeLabelRound(ui->shockInd);

    ui->shockInd->setStyleSheet("QLabel { border: 2px solid black; }");
    ui->shockInd->setStyleSheet("QLabel { border: 2px solid black; }");
    ui->statInd->setStyleSheet("QLabel { border: 2px solid black; }");

    setStepBackgroundColor(ui->step1, Qt::yellow);

    setStepBackgroundColor(ui->shockInd, Qt::red);


    //------- Electrodes ---------
    // Create an instance of Electrodes as a member variable
    electrodesWidget = new Electrodes(ui->electrodeFrame);
    setStepBackgroundColor(ui->shockInd, Qt::red);


    //------- Electrodes ---------
    // Create an instance of Electrodes as a member variable
    electrodesWidget = new Electrodes(ui->electrodeFrame);

    // Create an instance of the VoicePrompt as a member variable
    voicePromptWidget = new VoicePrompt(ui->promptFrame);

    // Connect the button's clicked signal to the powerButtonClicked slot
    connect(ui->powerButton, &QPushButton::clicked, this, &MainWindow::powerButtonClicked);
}

void MainWindow::setStepBackgroundColor(QLabel *label, const QColor &color) {
    QString styleSheet = QString("background-color: %1;").arg(color.name());
    label->setStyleSheet(styleSheet);
}

void MainWindow::initializeLabelImage(QLabel *label, const QString &imagePath, int width, int height) {
    QPixmap img(imagePath);
    label->setPixmap(img.scaled(width, height, Qt::KeepAspectRatio));
}

void MainWindow::initializePowerButtonImage(QPushButton *button, const QString &imagePath, int iconWidth, int iconHeight) {
    QPixmap buttonImg(imagePath);
    button->setIcon(QIcon(buttonImg));
    button->setIconSize(QSize(iconWidth, iconHeight));
}

void MainWindow::runSelfTest() {
    // Display a message in the QTextBrowser
    ui->LCDScreen->append("Initiating self-test...");

    // Wait for 5 seconds before updating the screen again
    QTimer::singleShot(5000, this, &MainWindow::selfTestComplete);
}

void MainWindow::selfTestComplete() {
    if(statusIndicator == 1){
        // Display a message in the QTextBrowser after the 5-second delay
//        ui->LCDScreen->append("Automated Defibrillator OK");
        // Update the statInd QLabel with the pass.svg image
        QPixmap statIndImage(":/res/Buttons/pass.svg");
        ui->statInd->setPixmap(statIndImage.scaled(80, 80, Qt::KeepAspectRatio));
        ui->voiceOutput->setText("Automated Defibrillator OK");
    } else{
        // Display a message in the QTextBrowser after the 5-second delay
//        ui->LCDScreen->append("Automated Defibrillator FAILED");
        // Update the statInd QLabel with the pass.svg image
        QPixmap statIndImage(":/res/Buttons/fail.svg");
        ui->statInd->setPixmap(statIndImage.scaled(80, 80, Qt::KeepAspectRatio));
        ui->voiceOutput->setText("Automated Defibrillator FAILED");
    }
}

// Modify the powerButtonClicked function to call runSelfTest when powering on
void MainWindow::powerButtonClicked() {
    if (powerStatus == 0) {
        ui->powerButton->setStyleSheet("background-color: green");
        ui->LCDScreen->setText("AED is powering on...");
        powerStatus = 1;

        // Add a 3-second delay before calling the runSelfTest function
        QTimer::singleShot(3000, this, &MainWindow::runSelfTest);
    } else {
        ui->LCDScreen->append("AED is already on...");
    }
}

MainWindow::~MainWindow() {
    delete ui;
//    delete electrodes; // Don't forget to clean up resources
}
