#include "voiceprompt.h"

VoicePrompt::VoicePrompt(QFrame* widget){

    //VoicePrompt widget and styling
    initializeVoicePrompt(widget);
}

void VoicePrompt::initializeVoicePrompt(QFrame *widget){
    voicePromptWidget = widget;
    voicePromptWidget->setFrameStyle(QFrame::Box | QFrame::Raised);
    voicePromptWidget->setLineWidth(1);
    voicePromptWidget->setMidLineWidth(3);
}
