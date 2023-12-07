#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QPixmap>
#include <QLabel>

// Member variable for the flashing lights
QTimer *flashTimer;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {

    ui->setupUi(this);
//    ui->battery->setVisible(false);
    ui->batterySlot->raise();

    this->setWindowTitle("AED Plus");
    powerStatus = 0;
    // 1 = Pass, 0 = Fail
    statusIndicator = 1;
    step = 0;
    shockCount = 0;
    batteryLevel = 100;

    // Create a timer for updating the elapsed time in the QTextBrowser
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateElapsedTime);

    // Connect the updateBatteryLevel slot to the updateTimer timeout signal
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateBatteryLevel);

    // Turn graph off since it lays on top of LCD
    ui->graph->setVisible(false);

    //Make window darker
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(150, 150, 150));  // Set the dark grey color
    this->centralWidget()->setAutoFillBackground(true);
    this->centralWidget()->setPalette(darkPalette);

    //Used for the flashing lights
    flashTimer = new QTimer(this);
    connect(flashTimer, &QTimer::timeout, this, [this]() { toggleFlash(ui->step1, Qt::yellow); });
    connect(flashTimer, &QTimer::timeout, this, [this]() { toggleFlash(ui->step2, Qt::yellow); });


    //Initialize image on power button
    initializePowerButtonImage(ui->powerButton, ":/res/Buttons/powerButton.svg", 50, 50);

    powerButtonTimer = new QTimer(this);
    connect(powerButtonTimer, &QTimer::timeout, this, &MainWindow::checkPowerButtonPress);

    connect(ui->powerButton, &QPushButton::pressed, this, &MainWindow::powerButtonPressed);
    connect(ui->powerButton, &QPushButton::released, this, &MainWindow::powerButtonReleased);
    // Connect the button's clicked signal to the powerButtonClicked slot
    connect(ui->powerButton, &QPushButton::clicked, this, &MainWindow::powerButtonClicked);



    // Initialize images on labels
    initializeLabelImage(ui->step1, ":/res/AEDSteps/Step1.png", 150, 150);
    initializeLabelImage(ui->step2, ":/res/AEDSteps/Step2.png", 150, 150);
    initializeLabelImage(ui->step3, ":/res/AEDSteps/Step3.png", 150, 150);
    initializeLabelImage(ui->step4, ":/res/AEDSteps/Step4.png", 150, 150);
    initializeLabelImage(ui->step5, ":/res/AEDSteps/Step5.png", 200, 150);
    initializeLabelImage(ui->shockInd, ":/res/AEDSteps/ShockInd.png", 78, 78);
    initializeLabelImage(ui->statInd, ":/res/AEDSteps/statInd0.png", 135, 135);

    ui->shockInd->setStyleSheet("QLabel { border: 2px solid black; }");
    ui->shockInd->setStyleSheet("QLabel { border: 2px solid black; }");


    // Create the hRhythm QComboBox and populate
    ui->hRhythm->addItem("NORMAL");
    ui->hRhythm->addItem("VF");
    ui->hRhythm->addItem("VT");

    // Create the selfTestStatus QComboBox
    ui->selfTestStatus->addItem("PASS");
    ui->selfTestStatus->addItem("FAIL");

    // Connect the currentIndexChanged signal to the updateStatusIndicator slot
    connect(ui->selfTestStatus, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::updateStatusIndicator);

    //------- Electrodes ---------
    // Create an instance of Electrodes as a member variable
    electrodesWidget = new Electrodes(ui->electrodeFrame);
//    setStepBackgroundColor(ui->shockInd, Qt::red);


    //------- Electrodes ---------
    // Create an instance of Electrodes as a member variable
    electrodesWidget = new Electrodes(ui->electrodeFrame);

    // Create an instance of the VoicePrompt as a member variable
    voicePromptWidget = new VoicePrompt(ui->promptFrame);

    connect(ui->hRhythm, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateGraph);

}

void MainWindow::updateBatteryLevel() {
    // If battery is lower than 15%
    if (batteryLevel <= 15) {
        // Display warning if battery falls below 15%
        ui->battery->setStyleSheet("QProgressBar::chunk { background-color: red }");

        if(batteryLevel <= 0){
            ui->graph->setVisible(false);
            ui->LCDScreen->setText("Device is shutting down");

            // Schedule closing the program after 5 seconds
            QTimer::singleShot(5000, this, &MainWindow::close);
        }
    }

    // Update the QProgressBar
    ui->battery->setValue(batteryLevel);
}


void MainWindow::shockDrain() {
    // Decrease battery level by 1%
    batteryLevel -= 1;

    // Ensure the battery level doesn't go below 0
    batteryLevel = qMax(batteryLevel, 0);
}

void MainWindow::updateGraph(int index) {
    QGraphicsScene* scene = new QGraphicsScene(this);

    // Select the appropriate points based on the heart rhythm
    const QVector<QPointF>& selectedPoints = (index == 0) ? normalPoints :
                                            (index == 1) ? vfPoints :
                                            (index == 2) ? vtPoints :
                                            QVector<QPointF>(); // Default to an empty set

    // Assuming "graph" is the QGraphicsView
    QGraphicsView* graphView = ui->graph;
    graphView->setScene(scene);

    // Connect the points with a line
    for (int i = 0; i < selectedPoints.size() - 1; ++i) {
        const QPointF& startPoint = selectedPoints[i];
        const QPointF& endPoint = selectedPoints[i + 1];
        scene->addLine(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y());
    }
}
void MainWindow::updateElapsedTime() {
    qint64 elapsedMilliseconds = AEDTimer.elapsed();
    qint64 seconds = elapsedMilliseconds / 1000;
    qint64 minutes = seconds / 60;

    // Format the elapsed time as a string
    QString elapsedTimeString = QString("%1:%2")
                                   .arg(minutes, 2, 10, QLatin1Char('0'))
                                   .arg(seconds % 60, 2, 10, QLatin1Char('0'));

    // Set the formatted elapsed time in the QTextBrowser
    ui->timeElapsed->clear();  // Clear existing text
    QTextCursor cursor = ui->timeElapsed->textCursor();

    // Align the text to the right
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setAlignment(Qt::AlignRight);
    cursor.setBlockFormat(blockFormat);

    // Move to the end and insert the text
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(elapsedTimeString);
    ui->timeElapsed->setTextCursor(cursor);
}

void MainWindow::updateStatusIndicator(int index) {
    // Check the selected value and update statusIndicator
    if (index == 0) {
        // "Pass" is selected
        statusIndicator = 1;
    } else if (index == 1) {
        // "Fail" is selected
        statusIndicator = 0;
    }
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

void MainWindow::powerButtonClicked() {
    if (powerStatus == 0) {
        // Add shockcounter
        ui->shocksCounter->setText("SHOCKS: " + QString::number(shockCount));
        ui->powerButton->setStyleSheet("background-color: green");
        ui->LCDScreen->setText("AED is powering on...");
        powerStatus = 1;

        //Show battery
        ui->batterySlot->lower();

        // Restart if the device was rebooted.
        AEDTimer.restart();
        // Initialize the device timer
        AEDTimer.start();

        // Update the timer every second
        updateTimer->start(1000);  // Update every second (adjust as needed)

        // Add a 3-second delay before calling the runSelfTest function
        QTimer::singleShot(3000, this, &MainWindow::runSelfTest);
    } else {
        ui->LCDScreen->append("AED is already on...");
    }
}

//Restart functions
void MainWindow::powerButtonPressed() {
    powerButtonPressDuration = 0;
    powerButtonTimer->start(1000);  // Timer fires every 1000 ms (1 second)
}

void MainWindow::powerButtonReleased() {
    powerButtonTimer->stop();
}

void MainWindow::checkPowerButtonPress() {
    powerButtonPressDuration++;

    if (powerButtonPressDuration == 5) {  // 5 seconds
        if (statusIndicator == 0) {
            // Status is 0 (failed self-test), initiate restart
            restartSystem();
        }
    }
}

void MainWindow::restartSystem() {
    //Clear all screens
    ui->LCDScreen->clear();
    ui->voiceOutput->clear();
    initializeLabelImage(ui->statInd, ":/res/AEDSteps/statInd0.png", 135, 135);
    runSelfTest();
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
        QPixmap statIndImage(":/res/AEDSteps/statIndPass.png");
        ui->statInd->setPixmap(statIndImage.scaled(135, 135, Qt::KeepAspectRatio));
        ui->voiceOutput->setText("Automated Defibrillator OK");

        // Move to the next step after the current step's duration (e.g., 5 seconds)
        QTimer::singleShot(5000, this, &MainWindow::nextStep);
//        nextStep();
    } else {
        // Display a message in the QTextBrowser after the 5-second delay
        ui->LCDScreen->append("Automated Defibrillator FAILED");
        // Update the statInd QLabel with the pass.svg image
        QPixmap statIndImage(":/res/AEDSteps/statIndFail.png");
        ui->statInd->setPixmap(statIndImage.scaled(135, 135, Qt::KeepAspectRatio));
        ui->voiceOutput->setText("Automated Defibrillator FAILED");
    }
}

void MainWindow::startFlash(QLabel *label, const QColor &flashColor) {
    // Start the flashing timer for the given label
    // Disconnect any previous connections
    flashTimer->disconnect();
    connect(flashTimer, &QTimer::timeout, this, [this, label, flashColor]() {
        toggleFlash(label, flashColor);
    });
    flashTimer->start(800);  // Adjust the interval (800 ms) as needed
}

void MainWindow::stopFlash(QLabel *label) {
    // Stop the flashing timer and reset the label to its original color
    flashTimer->disconnect();
    flashTimer->stop();

    QPalette palette = label->palette();
    palette.setColor(QPalette::Window, QColor(150, 150, 150));
    label->setAutoFillBackground(true);
    label->setPalette(palette);
}

void MainWindow::toggleFlash(QLabel *label, const QColor &flashColor) {
    // Toggle the background color between the flash color and the original color
    QPalette palette = label->palette();
    if (palette.color(QPalette::Window) == flashColor) {
        palette.setColor(QPalette::Window, QColor(150, 150, 150));
    } else {
        palette.setColor(QPalette::Window, flashColor);
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

        // Start the flash for step 1
        startFlash(ui->step1, Qt::yellow);

//        // Wait for 10 seconds before moving to the next step
//        QTimer::singleShot(10000, this, &MainWindow::nextStep);
        //Test
        QTimer::singleShot(10000, this, &MainWindow::nextStep);
        break;

    case 2:
        // Step 2: Call for help for 10 seconds
        ui->voiceOutput->append("Call for help");

        // Stop the flash for step 1 and start the flash for step 2
        stopFlash(ui->step1);
        startFlash(ui->step2, Qt::yellow);

//        // Wait for 10 seconds before moving to the next step
//        QTimer::singleShot(10000, this, &MainWindow::nextStep);

        //Test
        QTimer::singleShot(10000, this, &MainWindow::nextStep);
        break;

    case 3:
        // Step 3: Attach pads on the victim's chest
        ui->voiceOutput->append("Attach defib pads to patient's bare chest");

        connect(ui->electrode1, &QPushButton::clicked, this, &MainWindow::electrodeClicked);
        connect(ui->electrode2, &QPushButton::clicked, this, &MainWindow::electrodeClicked);
        connect(ui->electrode3, &QPushButton::clicked, this, &MainWindow::electrodeClicked);

        // Stop the flash for step 1 and start the flash for step 2
        stopFlash(ui->step2);
        startFlash(ui->step3, Qt::yellow);
        if (clickedElectrodes.size() == 3) {
            // disconnect electrodes (from the code only), since they are no longer needed.
            disconnect(ui->electrode1, &QPushButton::clicked, this, &MainWindow::electrodeClicked);
            disconnect(ui->electrode2, &QPushButton::clicked, this, &MainWindow::electrodeClicked);
            disconnect(ui->electrode3, &QPushButton::clicked, this, &MainWindow::electrodeClicked);
            nextStep();
        } else {
            // Not all electrodes are placed
               }
        break;

    case 4: {
        // Step 4: Don't touch the patient
        ui->voiceOutput->append("Don't touch the patient");
        ui->voiceOutput->append("Analyzing");

        // Stop the flash for step 3 and start the flash for step 4
        stopFlash(ui->step3);
        startFlash(ui->step4, Qt::yellow);


        // Create a timer for a 6-second delay
        QTimer::singleShot(6000, this, [this]() {
            // Check selected rhythm
            QString selectedRhythm = ui->hRhythm->currentText();
            ui->heartRhythm->setText(selectedRhythm);

            // Update the graph based on the selected rhythm
            updateGraph(ui->hRhythm->currentIndex());

            ui->graph->setVisible(true);

        });
        QTimer::singleShot(8000, this, [this]() {
            if (ui->hRhythm->currentText() == "NORMAL") {
                ui->voiceOutput->append("No shock advised");
                }
            QTimer::singleShot(1000, this, &MainWindow::nextStep);
        });

        break;
    }

    case 5:
        // If normal heart rhythm detected, jump to case 6
        if (ui->hRhythm->currentText() == "NORMAL") {
            nextStep();  // Jump to case 6
            }
        else {
            // Step 5: Shock phase
            ui->voiceOutput->append("STAND CLEAR");
            ui->voiceOutput->append("DO NOT touch the patient");

            // Stop the flash for step 1 and start the flash for step 2
            stopFlash(ui->step4);
            startFlash(ui->shockInd, Qt::red);

            QTimer::singleShot(3000, this, [this]() {
                // Schedule shock delivery messages with a 1-second delay
                QTimer::singleShot(1500, this, [this]() {
                    ui->voiceOutput->append("Shock will be delivered in 3");
                });
                QTimer::singleShot(2500, this, [this]() {
                    ui->voiceOutput->append("2");
                });
                QTimer::singleShot(3500, this, [this]() {
                    ui->voiceOutput->append("1");
                });
                QTimer::singleShot(4500, this, [this]() {
                    ui->voiceOutput->append("BEEP!");
                });
                QTimer::singleShot(5500, this, [this]() {
                    // Introduce a random chance for the heart rhythm to turn into normal
                    int randomChance = QRandomGenerator::global()->bounded(1, 5); // Generates a random number between 1 and 4
                    if (randomChance == 1) {
                        ui->voiceOutput->append("Shock delivered");
                        shockCount++;
                        ui->shocksCounter->setText("SHOCKS: " + QString::number(shockCount));
                        ui->hRhythm->setCurrentIndex(0);
                        // Also set the text to Normal
                        ui->hRhythm->setItemText(ui->hRhythm->currentIndex(), "NORMAL");
                    } else {
                        ui->voiceOutput->append("Shock delivered");
                        shockCount++;
                        ui->shocksCounter->setText("SHOCKS: " + QString::number(shockCount));
                    }
                    shockDrain();
                    updateBatteryLevel();
                });

                QTimer::singleShot(9000, this, &MainWindow::nextStep);
            });
        }
        break;

    case 6:

        //Step 6: CPR
        // Stop the flash for step 1 and start the flash for step 2
        stopFlash(ui->shockInd);
        stopFlash(ui->step4);
        startFlash(ui->step5, Qt::yellow);

        // Print "complete" in voiceOutput
            ui->voiceOutput->append("Start CPR");

            // Go back to case 4 after 2 minutes of CPR
            QTimer::singleShot(120000, this, [this]() {
                step = 4;
                performAEDStep();
            });
        break;
    default:
        // Handle unexpected step value
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
            return ui->shockInd;
        case 6:
            return ui->step5;
        default:
            return nullptr; // Handle unexpected step value
    }
}

void MainWindow::nextStep() {
    // Increment the step variable
    step++;

    // Perform actions based on the new step
    performAEDStep();
}

void MainWindow::previousStep() {
    // Decrement the step variable
    step--;

    // Perform actions based on the new step
    performAEDStep();
}

void MainWindow::stopCurrentStep() {
    // Stop the flashing and move to the next step
    stopFlash(ui->step1);
    stopFlash(ui->step2);
    // ... (stop other steps)

    // Move to the next step after the current step's duration (10 seconds)
    QTimer::singleShot(10000, this, &MainWindow::nextStep);
}

void MainWindow::electrodeClicked() {
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton && !clickedElectrodes.contains(clickedButton)) {
        clickedElectrodes.insert(clickedButton);

        if (clickedElectrodes.size() == 3) {
            // Move to the next step
            nextStep();
        }
    }
}

MainWindow::~MainWindow() {
    delete ui;
    delete flashTimer;
    delete powerButtonTimer;
}
