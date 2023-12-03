
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QPixmap>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {

    ui->setupUi(this);
    this->setWindowTitle("AED Plus");

    ui->powerButton->setIcon(QIcon(":/res/Buttons/powerButton.svg"));
    ui->powerButton->setIconSize(QSize(50,50));

    QPixmap step1Img(":/res/AEDSteps/Step1.png");
    ui->step1->setPixmap(step1Img.scaled(150,150,Qt::KeepAspectRatio));

    QPixmap step2Img(":/res/AEDSteps/Step2.png");
    ui->step2->setPixmap(step2Img.scaled(150,150,Qt::KeepAspectRatio));

    QPixmap step3Img(":/res/AEDSteps/Step3.png");
    ui->step3->setPixmap(step3Img.scaled(150,150,Qt::KeepAspectRatio));

    QPixmap step4Img(":/res/AEDSteps/Step4.png");
    ui->step4->setPixmap(step4Img.scaled(150,150,Qt::KeepAspectRatio));

    QPixmap step5Img(":/res/AEDSteps/Step5.png");
    ui->step5->setPixmap(step5Img.scaled(200,150,Qt::KeepAspectRatio));

    QPixmap shockIndImg(":/res/AEDSteps/ShockInd.png");
    ui->shockInd->setPixmap(shockIndImg.scaled(78,78,Qt::KeepAspectRatio));
    makeLabelRound(ui->shockInd);
    ui->shockInd->setStyleSheet("QLabel { border: 2px solid black; }");

    ui->statInd->setStyleSheet("QLabel { border: 2px solid black; }");

    setStepBackgroundColor(ui->step1, Qt::yellow);

    setStepBackgroundColor(ui->shockInd, Qt::red);

}

void MainWindow::makeLabelRound(QLabel *label) {
    // Set a circular shape for the QLabel using style sheets
    label->setStyleSheet("QLabel { border-radius: 40px; }");
}

void MainWindow::setStepBackgroundColor(QLabel *label, const QColor &color) {
    QString styleSheet = QString("background-color: %1;").arg(color.name());
    label->setStyleSheet(styleSheet);
}

MainWindow::~MainWindow() {
    delete ui;
}
