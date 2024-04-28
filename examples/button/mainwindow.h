#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMidi>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QMidi & midiIn, QWidget *parent = nullptr);
public Q_SLOTS:
    void onButtonPressed();
    void onButtonReleased();
protected:
    QMidi & midi;
};

#endif // MAINWINDOW_H
