# QOsc
Qt based library that implement the OSC protocol

![version](https://img.shields.io/github/v/release/NicoG60/QOsc) ![license](https://img.shields.io/github/license/NicoG60/Qosc) ![C/C++ CI](https://github.com/NicoG60/QOsc/workflows/C/C++%20CI/badge.svg?branch=master)



## Features

- Send and receive OSC packets from the network
- Easily make OSC packets to be sent
- Read/Write OSC packets to en external source

## Building & Integrating

It is a CMake project. you can either build it using:

```bash
cmake -S . -B build
cmake --build build
```

Or use it as a subdirectory

```cmake
add_subdirectory(path/to/QOsc EXCLUDE_FROM_ALL)
target_link_libraries(mytarget PUBLIC QOsc)
```

## Usage

You can use the library for:

- Convert, Read, Write OSC message to and from network byte stream
- Use QOSCInterface to actually write and read from the network

### Example of Server using QOSCInterface

```cpp
#include <QApplication>
#include <QOsc>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Bind the network interface so you can send and get messages
    QOscInterface iface;
    iface.setRemoteAddr(QHostAddress("192.168.0.10"));
    iface.setRemotePort(9000);
    iface.setLocalPort(8000);

    // Connect callbacks to get notified of new messages
    iface.connect("/my/osc/pattern",
    [](const QOscMessage& msg)
    {
        // This is the message callback
        // It'll be called each time a message matching the
        // pattern you set is received.

        for(auto& arg : msg)
        {
            if(!arg.isInt())
                continue;

            int i = arg.toInt();

            // do stuff with i

            Q_UNUSED(i);
        }
    });

    // Alternatively you can use any QObject slots
    QObject obj;
    iface.connect("/my/other/pattern", &obj, SLOT(mySlot(const QOscMessage& msg)));

    // Or get all messages and dispatch them yourself
    QObject dispatcher;

    QObject::connect(&iface,      SIGNAL(messageReceived(const QOscMessage&)),
                     &dispatcher, SLOT(dispatchMessage(const QOscMessage&)));

    QObject::connect(&iface,      SIGNAL(bundleReceived(const QOscBundle&)),
                     &dispatcher, SLOT(dispatchBundle(const QOscBundle&)));

    return a.exec();
}
```

### Exemple of Client using QOSCInterface

```cpp
#include <QApplication>
#include <qosc.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Bind the network interface so you can send and get messages
    QOscInterface iface;
    iface.setRemoteAddr(QHostAddress("192.168.0.10"));
    iface.setRemotePort(9000);
    iface.setLocalPort(8000);

    // Craft the message you want to send
    QOscMessage msg("/my/osc/pattern", QString("Some random string"));

    iface.send(msg);

    // More complex messages with several values
    QOscValue myInt(10);
    QOscValue myMidi = QOscValue::midiValue(/*port:   0       */ 0x00,
                                            /*status: note on */ 0x90,
                                            /*data1:  note A4 */ 0x45,
                                            /*data2:  note vel*/ 0x10);
    QOscValue myColor(QColor(0, 255, 0));

    QOscMessage msg2("/my/osc/pattern");
    msg2 << myInt << myMidi << myColor;

    iface.send(msg2);

    // You can also create bundles
    // They are useful to deliver several messages to different patterns
    // They can also be timed, You can specify an absolute Date and Time when you want them to be executed

    QOscBundle bundle;
    bundle.setTime(QDateTime::currentDateTime().addSecs(15)); // The bundle is for 15sec later
    bundle << QOscMessage("/osc/pattern1", QString("Some random string"))
           << QOscMessage("/osc/pattern2", 125)
           << QOscMessage("/osc/pattern3", true)
           << QOscMessage("/osc/pattern4", 3.14);

    iface.send(bundle);
    
    return a.exec();
}
```

### Example parsing OSC from an external source

```cpp
void processOSC(const QByteArray& data)
{
    switch(QOsc::detectType(data))
    {
    case QOsc::OscMessage:
    {
        auto msg = QOscMessage::read(data);
        if(!msg.isValid())
            break;
        
        // Do something with the message
        break;
    }

    case QOsc::OscBundle:
    {
        auto bundle = QOscBundle::read(data);
        if(!bundle.isValid())
            break;
        
        // Do something with the bundle
        break;
    }

    default:
        // Handle error where `data` is not an osc message
        break;
    }
}
```

