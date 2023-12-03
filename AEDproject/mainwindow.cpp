
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QPixmap>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {

    ui->setupUi(this);
    this->setWindowTitle("AED Plus");


//    ui->powerButton->setIcon(QIcon(":/res/Buttons/powerButton.svg"));
//    ui->powerButton->setIconSize(QSize(50,50));

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
}

//void MainWindow::makeLabelRound(QLabel *label) {
//    // Set a circular shape for the QLabel using style sheets
//    label->setStyleSheet("QLabel { border-radius: 40px; }");
//}

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

MainWindow::~MainWindow() {
    delete ui;
}
