#include "mainwindow.h"

MainWindow::MainWindow(QMidi & midiIn, QWidget *parent)
: QMainWindow(parent)
, midi(midiIn)
{
    setupUi(this);
    QObject::connect(pushButtonSend, &QPushButton::pressed, this, &MainWindow::onButtonPressed);
    QObject::connect(pushButtonSend, &QPushButton::released, this, &MainWindow::onButtonReleased);

    if(!midi.outputInterface().directions().testFlag(Output)) {
        pushButtonSend->setEnabled(false);
        groupBox->setTitle("Midi note (no midi or not an input)");
    }

}
void MainWindow::onButtonPressed()
{
    qDebug() << "Sending note on...";
    // You can also send MIDI messages using similarly named slots.
    // Both raw message and individual message type slots are available:
    midi.sendNoteOn(0, QMidiNote::ASharp_5, 127);
}
void MainWindow::onButtonReleased()
{
    qDebug() << "Sending note off...";
    midi.sendNoteOff(0, QMidiNote::ASharp_5, 0);
}
