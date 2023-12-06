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
#include <cmath>
#include <QRandomGenerator>

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
    // Since normal rhythm is so particular, we were not able to come up with a function that would consistently draw this type of rhythm. Instead we manually traced the heartbeat.
    // We chose not do add randomness to the points themselves, because again sometimes we endedup with unconventional "normal" rhythms
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

    // Pseudorandom sinwave funcition that will generate the VF rhythm, by adding some extra points which will make the function less smooth, resulting in a more accurate VT rhythm.
    const QVector<QPointF> vfPoints = [] {
        QVector<QPointF> points;
        QRandomGenerator random(QRandomGenerator::securelySeeded());
        for (int x = 0; x <= 240; x += 5) {
            double y = 100 + 15 * std::sin(0.8 * x)  + random.bounded(-4, 6);
            points.append(QPointF(x, y));
        }
        return points;
    }();

    //Pseudo random sinwave function that will generate the VT rhythm.
    const QVector<QPointF> vtPoints = [] {
        QVector<QPointF> points;
        QRandomGenerator random(QRandomGenerator::securelySeeded());

        for (double x = 0; x <= 240; x += 1) {
            double y = 100 + random.bounded(20, 25) * std::sin(0.2 * x);
            points.append(QPointF(x, y));
        }
        return points;
    }();

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
