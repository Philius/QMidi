#include <QApplication>
#include <QDebug>
#include <QMidi>

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QMidi & midiIn, QWidget * parent = nullptr)
    : QMainWindow(parent)
    , midi(midiIn)
    {
        if(midi.outputInterface().directions().testFlag(Output)) {
            qDebug() << "Sending note on...";
            // You can also send MIDI messages using similarly named slots.
            // Both raw message and individual message type slots are available:
            midi.sendNoteOn(0, QMidiNote::ASharp_5, 127);
        }

    }
    QMidi & midi;
public:
    void closeEvent(QCloseEvent *event)
    {
        Q_UNUSED(event);
        qDebug() << "Sending note off...";
        midi.sendNoteOff(0, QMidiNote::ASharp_5, 0);
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // The QMidi object represents a pair of in & out midi ports
    // you can create as many as you wish if you want more ports.

    // The default constructor chooses the first available API for your OS
    // and should be ready to use in most cases
    QMidi midi;

    // If you're on a unix-base system and want to use an alternative API
    // say jackd, you can explicitly set it like so:
    midi.setApi(QMidiApi::JACK);

    // You can query available APIs using:
    for(auto api : QMidi::availableApi())
        qDebug() << api;

    // You can also set a custom client name, so you're application
    // has the correct name displayed
    midi.setClientName("Super Midi 3000");

    // After each call, you can check whether there's been an error using:
    if(midi.hasError())
        qWarning() << midi.error() << midi.errorString();

    // You can query available MIDI interfaces using the following:
    QMidiInterface interfaceCopy;
    const QMidiInterface * pmi = & interfaceCopy;
    for(const auto& i : midi.availableInterfaces())
    {
        qDebug() << "Index:" << i.index();
        qDebug() << "Name:" << i.name();
        qDebug() << "API:" << i.api();
        qDebug() << "Directions (in/out):" << i.directions();
        if(!i.name().contains("Midi Through Port"))
            interfaceCopy = i;
        // Or simply
        qDebug() << i;
    }
    bool found = pmi == & interfaceCopy;
    // Say the first interface is the one you want to use
    // you need to set it using:
    const auto& i = found ? interfaceCopy : midi.availableInterfaces().constFirst();
    qDebug() << "Using midi port" << i.name();
    midi.setInputInterface(i);
    midi.setOutputInterface(i);

    // Now you can open the midi ports and start using them
    midi.open();

    // Alternatively, on unix-based system you can open
    // Virtual ports that will allow your application to
    // be detected by others as MIDI interface
    //midi.openVirtual("My Virtual Port");

    // From there you can use the set of signals and slots to
    // send and recieve midi data

    if(i.directions().testFlag(Input)) {
        // you can get the raw message and do the processing yourself
        QObject::connect(&midi, &QMidi::messageReceived, [](const QByteArray& data)
                         {
                             qDebug() << "Message received:" << data;
                         });

        // but you can get all the different message types with the appropriate signals
        QObject::connect(&midi, &QMidi::midiNoteOff, [](quint8 chan, quint8 note, quint8 vel)
                         {
                             qDebug() << "Note On received on channel" << chan;
                             qDebug() << "Node:" << static_cast<QMidiNote>(note);
                             qDebug() << "Velocity:" << vel;
                         });
    }
    MainWindow win(midi);
    win.show();
    // All those methods are slots for easily connect with other Qt objects
    // For example, you can do an easy pass-through with the following:
    QObject::connect(&midi, &QMidi::messageReceived,
                     &midi, &QMidi::sendMessage);

    a.exec();
}

#include "usage.moc"
