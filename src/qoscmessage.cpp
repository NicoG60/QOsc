#include <qoscmessage.h>
#include <types/qosctypes.h>
#include <QBuffer>

bool QOSCMessage::isValid() const
{
    return _match.isValid();
}

bool QOSCMessage::match(const QString& addr) const
{
    return _match.match(addr).hasMatch();
}

void QOSCMessage::write(QIODevice* dev) const
{
    QOSCString address(pattern);
    address.writeData(dev);

    QBuffer buf;
    buf.open(QIODevice::WriteOnly);
    buf.putChar(',');

    for(auto& arg : args)
        arg->writeTypeTag(&buf);

    QOSCString tags(buf.buffer());
    tags.writeData(dev);

    for(auto& arg : args)
        arg->writeData(dev);
}

void QOSCMessage::load(QIODevice* dev)
{
    QOSCString tmp;

    tmp.readData(dev);
    pattern = tmp;
    compilePattern();

    tmp.readData(dev);
    QString tmptags = tmp;

    // does not support arrays yet
    static QRegularExpression reg(R"(^,[ifsbhtdScrmTFNI]*$)");
    if(reg.match(tmptags).hasMatch())
    {
        for(auto& t : tmptags.midRef(1))
        {
            switch (t.toLatin1())
            {
            case 'i':
                loadArg<QOSCInt32>(dev);
                break;

            case 'f':
                loadArg<QOSCFloat32>(dev);
                break;

            case 's':
                loadArg<QOSCString>(dev);
                break;

            case 'b':
                loadArg<QOSCBlob>(dev);
                break;

            case 'h':
                loadArg<QOSCInt64>(dev);
                break;

            case 't':
                loadArg<QOSCTimeTag>(dev);
                break;

            case 'd':
                loadArg<QOSCFloat64>(dev);
                break;

            case 'S':
                loadArg<QOSCSymbol>(dev);
                break;

            case 'c':
                loadArg<QOSCChar>(dev);
                break;

            case 'r':
                loadArg<QOSCColor>(dev);
                break;

            case 'm':
                loadArg<QOSCMidi>(dev);
                break;

            case 'T':
                loadArg<QOSCTrue>(dev);
                break;

            case 'F':
                loadArg<QOSCFalse>(dev);
                break;

            case 'I':
                loadArg<QOSCInfinitum>(dev);
                break;

            case 'N':
                loadArg<QOSCNil>(dev);
                break;
            }
        }
    }
    else
    {
        QByteArray data = dev->readAll();
        args << QOSC::make(data);
    }
}

void QOSCMessage::compilePattern()
{
    QString copy = pattern;

    copy.replace('.', "\\.");
    copy.replace('(', "\\(");
    copy.replace(')', "\\)");
    copy.replace('*', "[^/]*");
    copy.replace("[!", "[^");
    copy.replace('?',  '.');

    QRegularExpression reg(R"({(([^,]+)(,([^,]+))+)})");
    QRegularExpressionMatch m;
    while((m = reg.match(copy)).hasMatch())
    {
        auto content = QRegularExpression::escape(m.capturedView(1));
        content.replace("\\,", "|");
        copy.replace(m.capturedStart(), m.capturedLength(), "(" + content + ")");
    }

    copy.prepend('^');
    copy.append('$');

    _match.setPattern(copy);
}
