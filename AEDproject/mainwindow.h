#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "electrodes.h"
#include "voiceprompt.h"

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    int statusIndicator;
    int step;
    int powerStatus;
    QTimer *flashTimer;
    Ui::MainWindow *ui;
    Electrodes *electrodesWidget;
    VoicePrompt *voicePromptWidget;

    void toggleFlash(QLabel *label);
    void startFlash(QLabel *label);
    void stopFlash(QLabel *label);
    void stopCurrentStep();
    QLabel* getStepLabel(int step);

    void runSelfTest();
    void selfTestComplete();

    void setStepBackgroundColor(QLabel *label, const QColor &color);

    void initializeLabelImage(QLabel *label, const QString &imagePath, int width, int height);
    void initializePowerButtonImage(QPushButton *button, const QString &imagePath, int iconWidth, int iconHeight);

    void powerButtonClicked();

    void performAEDStep();
    void nextStep();
    void previousStep();
};
#endif
