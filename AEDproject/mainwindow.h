#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <electrodes.h>
#include <voiceprompt.h>

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool powerStatus;
    Ui::MainWindow *ui;
    Electrodes *electrodesWidget;
    VoicePrompt *voicePromptWidget;
    void setStepBackgroundColor(QLabel *label, const QColor &color);
    void makeLabelRound(QLabel *label);
    void initializeLabelImage(QLabel *label, const QString &imagePath, int width, int height);
    void initializePowerButtonImage(QPushButton *button, const QString &imagePath, int iconWidth, int iconHeight);

};
#endif // MAINWINDOW_H
