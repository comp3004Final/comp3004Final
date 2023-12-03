#include "electrodes.h"

Electrodes::Electrodes(QFrame* widget){

    //Electrodes widget and styling
    initializeElectrodes(widget);

}

void Electrodes::initializeElectrodes(QFrame* widget){
    electrodesWidget = widget;
    electrodesWidget->setFrameStyle(QFrame::Box | QFrame::Raised);
    electrodesWidget->setLineWidth(1);
    electrodesWidget->setMidLineWidth(3);

}
