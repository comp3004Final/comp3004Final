#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "electrodes.h"
#include "voiceprompt.h"

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QComboBox>
#include <QElapsedTimer>
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateStatusIndicator(int index);

private:
    const QVector<QPointF> normalPoints = {
        QPointF(0, 100),
        QPointF(24, 100),
        QPointF(30, 100),
        QPointF(36, 80),
        QPointF(42, 100),
        QPointF(48, 50),
        QPointF(54, 110),
        QPointF(60, 110),
        QPointF(72, 100),

        QPointF(104, 100),
        QPointF(110, 100),
        QPointF(116, 80),
        QPointF(122, 100),
        QPointF(128, 50),
        QPointF(134, 110),
        QPointF(140, 110),
        QPointF(146, 110),
        QPointF(152, 100),

        QPointF(184, 100),
        QPointF(190, 100),
        QPointF(196, 80),
        QPointF(202, 100),
        QPointF(208, 50),
        QPointF(214, 110),
        QPointF(220, 110),
        QPointF(226, 100),
        QPointF(232, 100),
        QPointF(244, 100),
    };

    const QVector<QPointF> vfPoints = {
        QPointF(0, 100),
        QPointF(6, 81),
        QPointF(12, 92),
        QPointF(18, 63),
        QPointF(24, 105),
        QPointF(30, 76),
        QPointF(36, 96),
        QPointF(42, 65),
        QPointF(48, 104),
        QPointF(54, 73),

        QPointF(60, 100),
        QPointF(66, 80),
        QPointF(72, 89),
        QPointF(78, 58),
        QPointF(84, 97),
        QPointF(90, 66),
        QPointF(96, 88),
        QPointF(102, 60),
        QPointF(108, 100),
        QPointF(114, 71),

        QPointF(120, 103),
        QPointF(126, 85),
        QPointF(132, 98),
        QPointF(138, 65),
        QPointF(144, 108),
        QPointF(150, 74),
        QPointF(156, 93),
        QPointF(162, 62),
        QPointF(168, 98),
        QPointF(174, 66),

        QPointF(180, 96),
        QPointF(186, 78),
        QPointF(192, 89),
        QPointF(198, 60),
        QPointF(204, 100),
        QPointF(210, 72),
        QPointF(216, 93),
        QPointF(222, 60),
        QPointF(228, 102),
        QPointF(234, 63)
    };

    const QVector<QPointF> vtPoints = {
    //Implement this later alligator
    };

    int statusIndicator;
    int step;
    int powerStatus;
    int shockCount;

    //Restart mechanism
    QTimer *powerButtonTimer;
    int powerButtonPressDuration;

    QSet<QPushButton*> clickedElectrodes;

    QElapsedTimer AEDTimer;
    QTimer *updateTimer;
    void updateElapsedTime();

    QTimer *flashTimer;
    Ui::MainWindow *ui;
    Electrodes *electrodesWidget;
    VoicePrompt *voicePromptWidget;

    void toggleFlash(QLabel *label);
    void startFlash(QLabel *label);
    void stopFlash(QLabel *label);
    void stopCurrentStep();
    QLabel* getStepLabel(int step);

    //restart
    void powerButtonPressed();
    void powerButtonReleased();
    void checkPowerButtonPress();
    void restartSystem();

    void runSelfTest();
    void selfTestComplete();

    void setStepBackgroundColor(QLabel *label, const QColor &color);

    void initializeLabelImage(QLabel *label, const QString &imagePath, int width, int height);
    void initializePowerButtonImage(QPushButton *button, const QString &imagePath, int iconWidth, int iconHeight);

    void electrodeClicked();

    void powerButtonClicked();

    void performAEDStep();
    void nextStep();
    void previousStep();

    void updateGraph(int index);
};
#endif
