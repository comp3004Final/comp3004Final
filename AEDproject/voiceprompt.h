#ifndef VOICEPROMPT_H
#define VOICEPROMPT_H
#include <QFrame>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QObject>
#include <QTextBrowser>

class VoicePrompt : public QObject{
    Q_OBJECT
private:
    QFrame* voicePromptWidget;
    // Initializer for the electrodes panel
    void initializeVoicePrompt(QFrame* widget);

public:
    VoicePrompt(QFrame* widget);

    QTextBrowser* textPrompt;
};

#endif // VOICEPROMPT_H
