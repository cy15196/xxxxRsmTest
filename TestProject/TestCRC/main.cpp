#include <QCoreApplication>

#include <QDebug>

void InvertUint8(unsigned char *dBuf,unsigned char *srcBuf)
{
    int i;
    unsigned char tmp[4];
    tmp[0] = 0;
    for(i=0;i< 8;i++)
    {
      if(srcBuf[0]& (1 << i))
        tmp[0]|=1<<(7-i);
    }
    dBuf[0] = tmp[0];

}
void InvertUint16(unsigned short *dBuf,unsigned short *srcBuf)
{
    int i;
    unsigned short tmp[4];
    tmp[0] = 0;
    for(i=0;i< 16;i++)
    {
      if(srcBuf[0]& (1 << i))
        tmp[0]|=1<<(15 - i);
    }
    dBuf[0] = tmp[0];
}



unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)
{
  unsigned short wCRCin = 0xFFFF;
  unsigned short wCPoly = 0x8005;
  unsigned char wChar = 0;

  while (usDataLen--)
  {
        wChar = *(puchMsg++);
        InvertUint8(&wChar,&wChar);
        wCRCin ^= (wChar << 8);
        for(int i = 0;i < 8;i++)
        {
          if(wCRCin & 0x8000)
            wCRCin = (wCRCin << 1) ^ wCPoly;
          else
            wCRCin = wCRCin << 1;
        }
  }
  InvertUint16(&wCRCin,&wCRCin);
  return (wCRCin) ;
}


inline QByteArray hexToString(uchar* numArray, int length = 8)
{
    QByteArray ret;
    for(int i=0; i<length; i++)
    {
        QString temp = QString::number(numArray[i],16);
        if(temp.length()==1)
            temp = "0"+temp;
        ret += temp.toUtf8();
    }

    qDebug()<<ret;

    return QByteArray::fromHex(ret);
}

inline bool stringToHex(const QByteArray& str, uchar*& buffer, int& length)
{
    bool ok;
    length = str.length()/2;
    buffer  = new uchar[length];
    for(int i=0; i< length;i++)
    {
        buffer[i] = str.mid(2*i,2).toUShort(&ok,16);
        if(!ok)
            return false;
    }

    return true;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    int bufferSize = 8;
    uchar* cmdBuf = new uchar[bufferSize];
    memset(cmdBuf,0,sizeof(uchar)*bufferSize);
    int index = 0;

    cmdBuf[index++] = 0xff;
    cmdBuf[index++] = 0x03;
    cmdBuf[index++] = 0x34;
    cmdBuf[index++] = 0x23;
    cmdBuf[index++] = 0x45;
    cmdBuf[index++] = 0x56;

    unsigned short crcCode = CRC16_MODBUS(cmdBuf,index);
    cmdBuf[index++] =  crcCode & 0x00FF;
    cmdBuf[index++] = (crcCode & 0xFF00) / 0x100;

    QByteArray sendStr = hexToString(cmdBuf,bufferSize);

    qDebug()<<"sendStr is"<<sendStr<<sendStr.toHex();

    uchar* reciveBuffer;
    int reciveLength;

    qDebug()<<stringToHex(sendStr.toHex(),reciveBuffer,reciveLength)<<reciveLength;

    qDebug()<<hexToString(reciveBuffer,reciveLength-2).toHex();



    return a.exec();
}
