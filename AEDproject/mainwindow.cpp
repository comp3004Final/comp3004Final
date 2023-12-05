
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QPixmap>
#include <QLabel>

// Member variable for the flashing lights
QTimer *flashTimer;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {

    ui->setupUi(this);
    this->setWindowTitle("AED Plus");
    powerStatus = 0;
    // 1 = Pass, 0 = Fail
    statusIndicator = 1;
    step = 0;

    //Used for the flashing lights
    flashTimer = new QTimer(this);
    connect(flashTimer, &QTimer::timeout, this, [this]() { toggleFlash(ui->step1); });
    connect(flashTimer, &QTimer::timeout, this, [this]() { toggleFlash(ui->step2); });


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



//    setStepBackgroundColor(ui->shockInd, Qt::red);


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
    if (statusIndicator == 1) {
        // Display a message in the QTextBrowser after the 5-second delay
        ui->LCDScreen->append("Automated Defibrillator OK");
        // Update the statInd QLabel with the pass.svg image
        QPixmap statIndImage(":/res/Buttons/pass.svg");
        ui->statInd->setPixmap(statIndImage.scaled(80, 80, Qt::KeepAspectRatio));
        ui->voiceOutput->setText("Automated Defibrillator OK");

        // Move to the next step after the self-test is complete
        nextStep();
    } else {
        // Display a message in the QTextBrowser after the 5-second delay
        ui->LCDScreen->append("Automated Defibrillator FAILED");
        // Update the statInd QLabel with the pass.svg image
        QPixmap statIndImage(":/res/Buttons/fail.svg");
        ui->statInd->setPixmap(statIndImage.scaled(80, 80, Qt::KeepAspectRatio));
        ui->voiceOutput->setText("Automated Defibrillator FAILED");
    }
}

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

void MainWindow::startFlash(QLabel *label) {
    // Start the flashing timer for the given label
    flashTimer->disconnect();  // Disconnect any previous connections
    connect(flashTimer, &QTimer::timeout, this, [this, label]() { toggleFlash(label); });
    flashTimer->start(800);  // Adjust the interval (e.g., 800 ms) as needed
}

void MainWindow::stopFlash(QLabel *label) {
    // Stop the flashing timer and reset the label to its original color
    flashTimer->disconnect();
    flashTimer->stop();

    QPalette palette = label->palette();
    palette.setColor(QPalette::Window, Qt::white);
    label->setAutoFillBackground(true);
    label->setPalette(palette);
}

void MainWindow::toggleFlash(QLabel *label) {
    // Toggle the background color between yellow and the original color
    QPalette palette = label->palette();
    if (palette.color(QPalette::Window) == Qt::yellow) {
        palette.setColor(QPalette::Window, Qt::white);
    } else {
        palette.setColor(QPalette::Window, Qt::yellow);
    }
    label->setAutoFillBackground(true);
    label->setPalette(palette);
}


void MainWindow::performAEDStep() {
    switch (step) {
    case 1:
            // Step 1: Display "Stay calm." on voiceOutput for 10 seconds
            ui->voiceOutput->append("Stay calm");
            ui->voiceOutput->append("Check responsiveness");
            toggleFlash(ui->step1);  // Toggle the flash for step 1

            // Start the flashing timer for 10 seconds
            flashTimer->start(10000);

            // Move to the next step after the current step's duration (e.g., 10 seconds)
            QTimer::singleShot(10000, this, &MainWindow::nextStep);
            break;

        case 2:
            // Step 2: Call for help for 10 seconds
            ui->voiceOutput->append("Call for help");
            stopFlash(ui->step1);  // Stop the flash for step 1
            startFlash(ui->step2);  // Start the flash for step 2
            break;

        // ... (update for other steps)

        default:

            break;
    }
}

QLabel *MainWindow::getStepLabel(int step) {
    switch (step) {
        case 1:
            return ui->step1;
        case 2:
            return ui->step2;
        case 3:
            return ui->step3;
        case 4:
            return ui->step4;
        case 5:
            return ui->step5;
        default:
            return nullptr;
    }
}

void MainWindow::nextStep() {
    // Increment the step variable
    step++;

    // Perform actions based on the new step
    performAEDStep();
}

void MainWindow::stopCurrentStep() {
    // Stop the flashing and move to the next step
    stopFlash(ui->step1);
    stopFlash(ui->step2);
    // ... (stop other steps)

    // Move to the next step after the current step's duration ( 10 seconds)
    QTimer::singleShot(10000, this, &MainWindow::nextStep);
}

void MainWindow::previousStep() {
    // Decrement the step variable
    step--;

    // Perform actions based on the new step
    performAEDStep();
}

MainWindow::~MainWindow() {
    delete ui;
    delete flashTimer;
}
