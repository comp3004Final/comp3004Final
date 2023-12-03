#ifndef ELECTRODES_H
#define ELECTRODES_H
#include <QFrame>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QObject>

class Electrodes :public QObject{
    Q_OBJECT

private:
    QFrame* electrodesWidget;
    // Initializer for the electrodes panel
    void initializeElectrodes(QFrame* widget);

public:
    Electrodes(QFrame* widget);

    QPushButton* electrode1;
    QPushButton* electrode2;
    QPushButton* electrode3;
};

#endif
