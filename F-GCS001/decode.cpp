#include "decode.h"
#include "qdebug.h"
#include "QThread"

Decode::Decode()
{
     //QByteArray F_Start(2,0xaa);//定义帧头

    connect(this,SIGNAL(GET()),
            this,SLOT(DataThread()));


}

/*
void Decode::run()
{
    qDebug() << "run";
}
*/

bool Decode::SUMcheck(QByteArray data)
{
    static quint8 Count = 0;
     bool ReturnVelue;
     quint8 i;
     quint8 sum = 0;//变量必须初始化
      //qDebug() << ((quint8)data[3] + 4);
      for(i = 0;i<(((quint8)data[3])+4);i++)
      {
              sum = sum + ((quint8)data[i]);
      }

      if(((quint8)data[i]) == ((quint8)(sum&0xff)))
      {
          ReturnVelue = true;
      }
      else
      {
          Count ++;
          //qDebug() << Count;
          //qDebug() << sum;
          //qDebug() << (quint8)data[i];
          //qDebug() << (quint8)data[3];
          ReturnVelue = false;
      }
      return ReturnVelue;
}

quint16 CRC16_TABLE[] = {
0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

quint16 Decode::CRCcheck(QByteArray data)
{
     quint16 crc = 0;
     qint32 i = 0;
     quint8 temp = 0;
     quint8 len = (quint8)(data[3] + 4 - 1);//除了校验码以外所有的值
    while (len-- != 0)
    {
            temp = (quint8)(crc >> 8);
            crc <<= 8;
            crc ^= CRC16_TABLE[temp ^ (quint8)data[i]];
            i++;
    }
    return crc;
}

QByteArray RecieveDataBuff;//所有收到的数据都在这里面

void Decode::GetSerialData(QByteArray rawdata)
{
    //qDebug() << "GET";
    RecieveDataBuff.append(rawdata);
    emit GET();
}




QByteArray F_Start(2,0xaa);//定义帧头
QByteArray RecieveData;
QByteArray FuntiongAndLen;
quint8 CmdOfData;
quint8 LenOfData;
QByteArray mData;
void Decode::DataThread(void)
{
     //qDebug() << QThread::currentThreadId();
        //qDebug() << "收到1";
     QByteArray rawdata = RecieveDataBuff;
     RecieveDataBuff = 0;

     if(rawdata.size() > 0)
     {
         //qDebug() << rawdata;
         RecieveData.append(rawdata);
         //qDebug() << RecieveData;
         //qDebug() << "收到";
         if((RecieveData.size() >= 1024)&&(!RecieveData.contains(F_Start)))//当存的数据多了没有可用数据时清除缓存
         {
             RecieveData.clear();
         }

         while(RecieveData.contains(F_Start))
         {

             RecieveData = RecieveData.mid(RecieveData.indexOf(F_Start));//从帧头开始取数
             if(RecieveData.size() < 6)
             {
                 //qDebug() << "data数小于10";
                 break;//结束循环，因为数据长度不够
             }
             FuntiongAndLen = RecieveData.mid(RecieveData.indexOf(F_Start),4);
             CmdOfData = (quint8(FuntiongAndLen.data()[2]));
             LenOfData = (quint8(FuntiongAndLen.data()[3]));
             //qDebug() << LenOfData;
             //有可能出现误码导致命令和长度发生改变，此时应该丢弃
             if(RecieveData.size() < (LenOfData+5))
             {
                 //qDebug() << "长度不足";
                 break;
             }


             mData = RecieveData.mid(RecieveData.indexOf(F_Start),LenOfData+5);//外加帧头和功能码，rssi 长度，校验码，一共5个字节
             RecieveData = RecieveData.mid(RecieveData.indexOf(F_Start)+5);//把已经用过的数据从缓存里面除去
             //qDebug() << (mData.size()-5) << LenOfData << CmdOfData;
             //qDebug() << mData;
             if(mData.size() != (LenOfData + 5))
             {
                 qDebug() << "数据长度不符合";
                 continue;//防止数据出错，导致switch无法运行，这是一个bug
             }
             //qDebug() << "进入校验和";
             if((CmdOfData != 0x41)&&SUMcheck(mData))
             {
                 //qDebug() << "进入解码";
                 //qDebug() << CmdOfData;
                 switch(CmdOfData)
                 {
                     case 0x00:{ Version(mData);    }break;
                     case 0x01:{ Echo(mData);       }break;
                     case 0x02:{ Echo(mData);       }break;
                     case 0x03:{ Echo(mData);       }break;
                     case 0x04:{ Echo(mData);       }break;
                     case 0x05:{ Echo(mData);       }break;
                     case 0x06:{ Echo(mData);       }break;
                     case 0x07:{ Echo(mData);       }break;
                     case 0x08:{ Echo(mData);       }break;
                     case 0x0B:{ Remote(mData);     }break;
                     case 0x31:{ PID(mData);        }break;
                     case 0x32:{ PID(mData);        }break;
                     case 0x33:{ PID(mData);        }break;
                     case 0x34:{ PID(mData);        }break;
                     case 0x51:{ TaskSet(mData);    }break;
                     case 0x52:{ TaskSet(mData);    }break;
                     case 0x53:{ TaskSet(mData);    }break;
                     case 0x54:{ TaskSet(mData);    }break;
                     case 0x55:{ TaskSet(mData);    }break;
                     case 0x56:{ TaskSet(mData);    }break;
                     case 0x57:{ TaskSet(mData);    }break;
                     case 0x58:{ TaskSet(mData);    }break;
                     case 0xA0:{ Calibration(mData);}break;
                     case 0xA1:{ BiasParmeter(mData);}break;
                     case 0xB2:{ ServoPolar(mData); }break;
                     case 0xB3:{ ServoPoly(mData);  }break;
                     default  : mData=0 ; break;
                 }
             }
             else if(CmdOfData == 0x41)
             {
                 union {quint8 B[2];quint16 D;}src;
                 src.D = CRCcheck(mData);
                 if(((quint8)mData.at(LenOfData + 3) == src.B[0])&&
                    ((quint8)mData.at(LenOfData + 4) == src.B[1]))
                 {
                     switch(CmdOfData)
                     {
                         case 0x41:{ ReceiveDot(mData);     }break;
                         default  : mData=0 ; break;
                     }
                 }
             }
             else
             {
                 LenOfData = 0;
                 //qDebug() << LenOfData;
             }




         }
     }
     else
     {
         //QThread::msleep(5);//如果没有数，那么这个线程就休息5ms
     }
}

void Decode::Version(QByteArray data)
{
     quint16 HardwareVER;
     quint16 SoftwareVER;
     quint16 ProtocolVER;
     quint16 AirplaneVER;
     QString AirplaneName;
     quint8  DataCount = 5;
     union {quint8 B[2];qint16 D;}src;
     if(data.contains(F_Start))
     {
            src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];   HardwareVER = src.D *0.01f;
            src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];   SoftwareVER = src.D *0.01f;
            src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];   ProtocolVER = src.D *0.01f;
            src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];   AirplaneVER = src.D *0.01f;
            AirplaneName.append(data.mid(DataCount++,8));
            emit ShowVersion(HardwareVER,SoftwareVER,ProtocolVER,AirplaneVER,AirplaneName);
     }
}

void Decode::Echo(QByteArray data)
{
    quint8 ID;
    quint8 A,B,C,D,E,F,G,H;
    quint8 DataCount = 5;

    if(data.contains(F_Start))
    {
        ID= data[2];
        A = data[DataCount++];
        B = data[DataCount++];
        C = data[DataCount++];
        D = data[DataCount++];
        E = data[DataCount++];
        F = data[DataCount++];
        G = data[DataCount++];
        H = data[DataCount++];
        qDebug() << A
                 << B
                 << C
                 << D
                 << E
                 << F
                 << G
                 << H;
        emit ShowEcho(ID,A,B,C,D,E,F,G,H);
    }
}

void Decode::Remote(QByteArray data)
{
    //qDebug() <<"解码";
     uint8_t RSSI;
     uint8_t IMUSelect;
     float ACC1_X,ACC1_Y,ACC1_Z,GYRO1_X,GYRO1_Y,GYRO1_Z,ROLL1,PITCH1,YAW1;
     float ACC2_X,ACC2_Y,ACC2_Z,GYRO2_X,GYRO2_Y,GYRO2_Z,ROLL2,PITCH2,YAW2;
     uint8_t GPS1_Status,GPS2_Status,GPS1_Stars,GPS2_Stars;
     double Latitude1,Longitude1,Latitude2,Longitude2;
     float Altitude1,Course1,Altitude2,Course2;
     uint8_t Hour,Minute,Second;
     uint8_t SpeedSelect;
     float AirSpeed;
     float NorthSpeed1,EastSpeed1,DownSpeed1,NorthSpeed2,EastSpeed2,DownSpeed2;
     float VotageMultiRotor,VotageFixedWing,VotageSystem;
     uint8_t isRCValid;
     float ROL,PIT,THR,RUD;
     float RotorThrottle,Rotor1,Rotor2,Rotor3,Rotor4,Throttle1,Throttle2,Aileron,Elevator,Rudder;
     uint16_t CurrentTargetPoint,WaitToFlight;
     float DiffrentDistance,DiffrentAngle,DiffrentHeigh,DiffrentSpeed;
     float TargetRoll,TargetPitch,TargetYaw,TargetHeigh;
     float RelativeHeight;
     //uint8_t Status1,Status2;
     uint8_t Protect;//,Command2;


     typedef struct
            { quint8 CurrentStatus:3;//最低位
              quint8 TurningStatus:2;
              quint8 LockStatus:1;
              quint8 :1;
              quint8 :1;//最高位
            }Status1Def;
     union {Status1Def status; uint8_t data;}Status1;

     typedef struct
            { quint8 :2;//最低位
              quint8 FlightMode:2;//最高位
              quint8 AirPlane:2;//最高位
              quint8 CtrlMode:2;//最高位
            }Status2Def;
     union {Status2Def status; uint8_t data;}Status2;
/*
     typedef struct { quint8 :0;//最低位
              quint8 Landing:1;//最高位
              quint8 TakeOff:1;//最高位
            }Command1Def;
     union {Command1Def command; uint8_t data;}Protect;
*/
     typedef struct { quint8 :0;//最低位
              quint8 Landing:1;//最高位
              quint8 TakeOff:1;//最高位
            }Command2Def;
     union {Command2Def command; uint8_t data;}Command2;

     union {uint8_t B[8];uint16_t D[4];int16_t W[4];float F[2];double DD;}src;

     quint8 DataCount = 4;

    if(data.contains(F_Start))
    {
                                                                        RSSI      = data[DataCount++];
                                                                        IMUSelect = data[DataCount++];
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  ACC1_X  = src.W[0] * 0.001f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  ACC1_Y  = src.W[0] * 0.001f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  ACC1_Z  = src.W[0] * 0.001f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  GYRO1_X = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  GYRO1_Y = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  GYRO1_Z = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  ROLL1   = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  PITCH1  = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  YAW1    = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  ACC2_X  = src.W[0] * 0.001f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  ACC2_Y  = src.W[0] * 0.001f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  ACC2_Z  = src.W[0] * 0.001f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  GYRO2_X = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  GYRO2_Y = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  GYRO2_Z = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  ROLL2   = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  PITCH2  = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];  YAW2    = src.W[0] * 0.1f;
                                                   GPS1_Status = (data[DataCount]&0x80)?0x01:0x00;
                                                   GPS1_Stars  = (data[DataCount++]&0x7F);
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];
          src.B[2] = data[DataCount++];  src.B[3] = data[DataCount++];
          src.B[4] = data[DataCount++];  src.B[5] = data[DataCount++];
          src.B[6] = data[DataCount++];  src.B[7] = data[DataCount++]; Latitude1  = src.DD;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];
          src.B[2] = data[DataCount++];  src.B[3] = data[DataCount++];
          src.B[4] = data[DataCount++];  src.B[5] = data[DataCount++];
          src.B[6] = data[DataCount++];  src.B[7] = data[DataCount++]; Longitude1 = src.DD;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Altitude1  = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Course1    = src.W[0] * 0.1f;
                                                   GPS2_Status = (data[DataCount]&0x80)?0x01:0x00;
                                                   GPS2_Stars  = (data[DataCount++]&0x7F);
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];
          src.B[2] = data[DataCount++];  src.B[3] = data[DataCount++];
          src.B[4] = data[DataCount++];  src.B[5] = data[DataCount++];
          src.B[7] = data[DataCount++];  src.B[7] = data[DataCount++]; Latitude2  = src.DD;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++];
          src.B[2] = data[DataCount++];  src.B[3] = data[DataCount++];
          src.B[4] = data[DataCount++];  src.B[5] = data[DataCount++];
          src.B[6] = data[DataCount++];  src.B[7] = data[DataCount++]; Longitude2 = src.DD;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Altitude2  = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Course2    = src.W[0] * 0.1f;
                                                                       Hour       = data[DataCount++];
                                                                       Minute     = data[DataCount++];
                                                                       Second     = data[DataCount++];
                                                                       SpeedSelect= data[DataCount++];
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; AirSpeed   = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; NorthSpeed1= src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; EastSpeed1 = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; DownSpeed1 = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; NorthSpeed2= src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; EastSpeed2 = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; DownSpeed2 = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; VotageMultiRotor= src.D[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; VotageFixedWing = src.D[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; VotageSystem    = src.D[0] * 0.01f;
                                                                       isRCValid       = data[DataCount++];
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; ROL  = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; PIT  = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; THR  = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; RUD  = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; RotorThrottle  = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Rotor1         = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Rotor2         = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Rotor3         = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Rotor4         = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Throttle1      = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Throttle2      = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Aileron        = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Elevator       = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; Rudder         = src.W[0] * 0.01f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; CurrentTargetPoint = src.D[0];
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; WaitToFlight       = src.D[0];

          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; DiffrentDistance   = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; DiffrentAngle = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; DiffrentHeigh = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; DiffrentSpeed = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; TargetRoll    = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; TargetPitch   = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; TargetYaw     = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; TargetHeigh   = src.W[0] * 0.1f;
          src.B[0] = data[DataCount++];  src.B[1] = data[DataCount++]; RelativeHeight= src.W[0] * 0.1f;
                                                                       Status1.data  = data[DataCount++];
                                                                       Status2.data  = data[DataCount++];
                                                                       Protect       = data[DataCount++];
                                                                       Command2.data = data[DataCount++];

 /*         qDebug() << RSSI
                   << IMUSelect
                   << ACC1_X << ACC1_Y << ACC1_Z << GYRO1_X << GYRO1_Y << GYRO1_Z << ROLL1 << PITCH1 << YAW1
                   << ACC2_X << ACC2_Y << ACC2_Z << GYRO2_X << GYRO2_Y << GYRO2_Z << ROLL2 << PITCH2 << YAW2
                   << GPS1_Status << GPS1_Stars << Latitude1 << Longitude1 << Altitude1 << Course1
                   << GPS2_Status << GPS2_Stars << Latitude2 << Longitude2 << Altitude2 << Course2
                   << Hour << Minute << Second
                   << SpeedSelect << AirSpeed
                   << NorthSpeed1 << EastSpeed1 << DownSpeed1 << NorthSpeed2 << EastSpeed2 << DownSpeed2
                   << VotageMultiRotor << VotageFixedWing << VotageSystem
                   << isRCValid
                   << ROL << PIT << THR << RUD
                   << RotorThrottle << Rotor1 << Rotor2 << Rotor3 << Rotor4 << Throttle1 << Throttle2 << Aileron << Elevator << Rudder
                   << CurrentTargetPoint << WaitToFlight << DiffrentDistance
                   << DiffrentAngle << DiffrentHeigh << DiffrentSpeed
                   << TargetRoll << TargetPitch << TargetYaw << TargetHeigh
                   << RelativeHeight
                   << Status1.status.CtrlMode << Status1.status.AirPlane << Status1.status.LockStatus << Status1.status.TurningStatus << Status1.status.CurrentStatus
                   << Status2.status.CutMode
                   << Command1.command.TakeOff << Command1.command.Landing
                   << Command2.command.TakeOff << Command2.command.Landing;
*/

       /*qDebug() << RSSI
                << IMUSelect
                << ACC1_X << ACC1_Y << ACC1_Z << GYRO1_X << GYRO1_Y << GYRO1_Z << ROLL1 << PITCH1 << YAW1 ;
*/

     //qDebug() << Status2.status.CtrlMode
      //        << Status2.status.AirPlane;
     emit ShowRemote(RSSI,IMUSelect,
                     ACC1_X,ACC1_Y,ACC1_Z,GYRO1_X,GYRO1_Y,GYRO1_Z,ROLL1,PITCH1,YAW1,
                     ACC2_X,ACC2_Y,ACC2_Z,GYRO2_X,GYRO2_Y,GYRO2_Z,ROLL2,PITCH2,YAW2,
                     GPS1_Status,GPS1_Stars,Latitude1,Longitude1,Altitude1,Course1,
                     GPS2_Status,GPS2_Stars,Latitude2,Longitude2,Altitude2,Course2,
                     Hour,Minute,Second,
                     SpeedSelect,
                     AirSpeed,
                     NorthSpeed1,EastSpeed1,DownSpeed1,
                     NorthSpeed2,EastSpeed2,DownSpeed2,
                     VotageMultiRotor,VotageFixedWing,VotageSystem,
                     isRCValid,
                     ROL,PIT,THR,RUD,
                     RotorThrottle,Rotor1,Rotor2,Rotor3,Rotor4,Throttle1,Throttle2,Aileron,Elevator,Rudder,
                     CurrentTargetPoint,WaitToFlight,
                     DiffrentDistance,DiffrentAngle,DiffrentHeigh,DiffrentSpeed,
                     TargetRoll,TargetPitch,TargetYaw,TargetHeigh,RelativeHeight,
                     0,0,Status1.status.LockStatus,Status1.status.TurningStatus,Status1.status.CurrentStatus,
                     Status2.status.CtrlMode,Status2.status.AirPlane,Status2.status.FlightMode,
                     Protect,
                     Command2.data);


     emit ShowAttitude(ROLL1,PITCH1,YAW1);
    }
}

void Decode::PID(QByteArray data)
{
         quint8 ID;
         float PID1_P,PID1_I,PID1_D,PID2_P,PID2_I,PID2_D,PID3_P,PID3_I,PID3_D;
         float PID4_P,PID4_I,PID4_D,PID5_P,PID5_I,PID5_D,PID6_P,PID6_I,PID6_D;
         quint8 DataCount = 5;
         union {quint8 B[4];float F;}src;

         if(data.contains(F_Start))
         {
                                                                                                                                          ID = data[2];
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID1_P = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID1_I = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID1_D = src.F;

              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID2_P = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID2_I = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID2_D = src.F;

              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID3_P = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID3_I = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID3_D = src.F;

              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID4_P = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID4_I = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID4_D = src.F;

              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID5_P = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID5_I = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID5_D = src.F;

              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID6_P = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID6_I = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; PID6_D = src.F;

              //qDebug() << "Get pid" << ID;
              emit ShowPID(ID,PID1_P,PID1_I,PID1_D,PID2_P,PID2_I,PID2_D,PID3_P,PID3_I,PID3_D,
                              PID4_P,PID4_I,PID4_D,PID5_P,PID5_I,PID5_D,PID6_P,PID6_I,PID6_D);
         }
}

void Decode::ReceiveDot(QByteArray data)
{
    quint8 Function,Groups;
    quint16 TotalPoint,CurrentPoint;
    double Longitude,Latitude;
    float  Altitude;
    quint16 Velocity,Radius,Action;

    quint8 DataCount = 5;
     union {quint8 B[8];quint16 D[4];qint16 W[4];double DB;}src;

     if(data.contains(F_Start))
     {
           src.B[0] = data[DataCount++];                                Function     = src.B[0];
           src.B[0] = data[DataCount++];                                Groups       = src.B[0];
           src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];  TotalPoint   = src.D[0];
           src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];  CurrentPoint = src.D[0];

           src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];
           src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++];
           src.B[4] = data[DataCount++]; src.B[5] = data[DataCount++];
           src.B[6] = data[DataCount++]; src.B[7] = data[DataCount++];  Longitude = src.DB;

           src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];
           src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++];
           src.B[4] = data[DataCount++]; src.B[5] = data[DataCount++];
           src.B[6] = data[DataCount++]; src.B[7] = data[DataCount++];  Latitude = src.DB;

           src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];  Altitude  = src.W[0] * 0.1f;
           src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];  Velocity  = src.D[0];
           src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];  Radius    = src.D[0];
           src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++];  Action    = src.B[0];
           /*qDebug() << Function
                    << Groups
                    << TotalPoint
                    << CurrentPoint
                    << Longitude
                    << Latitude
                    << Altitude
                    << Velocity
                    << Radius
                    << Action;*/
           emit ShowDot(Function,Groups,TotalPoint,CurrentPoint,Longitude,Latitude,Altitude,Velocity,Radius,Action);
     }

}

void Decode::TaskSet(QByteArray data)
{
         quint8 ID;
         quint8 DataCount = 5;
         float A,B,C,D,E,F,G,H;
         union {quint8 B[4];float F;}src;

         if(data.contains(F_Start))
         {
                                                                                                                                     ID = data[2];
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; A = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; B = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; C = src.F;

              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; D = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; E = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; F = src.F;

              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; G = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; H = src.F;
              qDebug() << ID
                       << A
                       << B
                       << C
                       << D
                       << E
                       << F
                       << G
                       << H;
              emit ShowTaskSet(ID,A,B,C,D,E,F,G,H);
         }
}


void Decode::Calibration(QByteArray data)
{
         quint8 SenserType;
         quint8 DataCount = 5;
         float X,Y,Z;
         union {quint8 B[4];float F;}src;
         if(data.contains(F_Start))
         {
              SenserType = data[DataCount++];
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; X = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; Y = src.F;
              src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; Z = src.F;
             /* qDebug() << SenserType
                       << X
                       << Y
                       << Z;*/
              emit ShowCalibration(SenserType,X,Y,Z);

         }

         //qDebug() << data;
}

void Decode::BiasParmeter(QByteArray data)
{
    quint8 SenserType;
    quint8 DataType;
    quint8 DataCount = 5;
    float X,Y,Z;
    union {quint8 B[4];float F;}src;
    if(data.contains(F_Start))
    {
         SenserType = data[DataCount++];
         DataType = data[DataCount++];
         src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; X = src.F;
         src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; Y = src.F;
         src.B[0] = data[DataCount++]; src.B[1] = data[DataCount++]; src.B[2] = data[DataCount++]; src.B[3] = data[DataCount++]; Z = src.F;

         qDebug() << SenserType << DataType
                  << X << Y << Z;
         //emit ShowCalibration(SenserType,X,Y,Z);

    }
}


//舵机校准和极性

void Decode::ServoPolar(QByteArray data)
{
    qint8 Polar[20];
    quint8 ID;
    quint8 DataCount = 5;
    if(data.contains(F_Start))
    {
        ID = data[DataCount++];
        ID = ID;

        Polar[0] = data[DataCount++];
        Polar[1] = data[DataCount++];
        Polar[2] = data[DataCount++];
        Polar[3] = data[DataCount++];

        Polar[4] = data[DataCount++];
        Polar[5] = data[DataCount++];
        Polar[6] = data[DataCount++];
        Polar[7] = data[DataCount++];

        Polar[8] = data[DataCount++];
        Polar[9] = data[DataCount++];
        Polar[10] = data[DataCount++];
        Polar[11] = data[DataCount++];

        Polar[12] = data[DataCount++];
        Polar[13] = data[DataCount++];
        Polar[14] = data[DataCount++];
        Polar[15] = data[DataCount++];

        Polar[16] = data[DataCount];
        Polar[17] = data[DataCount];
        Polar[18] = data[DataCount];
        Polar[19] = data[DataCount++];

        //qDebug() << "ok";

        emit ShowServoPolar(Polar[0],Polar[1],Polar[2],Polar[3],
                            Polar[4],Polar[5],Polar[6],Polar[7],
                            Polar[8],Polar[9],Polar[10],Polar[11],
                            Polar[12],Polar[13],Polar[14],Polar[15],
                            Polar[16],Polar[17],Polar[18],Polar[19]);


     }
}


void Decode::ServoPoly(QByteArray data)
{

    float  Poly[21][3];
    quint8 ID;
    union {quint8 B[2];qint16 W;}src;

    quint8 DataCount = 5;
    if(data.contains(F_Start))
    {
        ID = data[DataCount++];
        ID = ID;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[0][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[0][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[0][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[1][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[1][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[1][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[2][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[2][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[2][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[3][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[3][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[3][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[4][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[4][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[4][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[5][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[5][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[5][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[6][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[6][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[6][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[7][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[7][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[7][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[8][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[8][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[8][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[9][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[9][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[9][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[10][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[10][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[10][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[11][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[11][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[11][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[12][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[12][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[12][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[13][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[13][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[13][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[14][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[14][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[14][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[15][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[15][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[15][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[16][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[16][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[16][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[17][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[17][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[17][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[18][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[18][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[18][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[19][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[19][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[19][2] = src.W * 0.001f;
        //==============================
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[20][0] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[20][1] = src.W * 0.001f;
        src.B[0] = data[DataCount++];
        src.B[1] = data[DataCount++];
        Poly[20][2] = src.W * 0.001f;

        emit ShowServoPoly(Poly[0][0],Poly[0][1],Poly[0][2],
                           Poly[1][0],Poly[1][1],Poly[1][2],
                           Poly[2][0],Poly[2][1],Poly[2][2],
                           Poly[3][0],Poly[3][1],Poly[3][2],
                           Poly[4][0],Poly[4][1],Poly[4][2],
                           Poly[5][0],Poly[5][1],Poly[5][2],
                           Poly[6][0],Poly[6][1],Poly[6][2],
                           Poly[7][0],Poly[7][1],Poly[7][2],
                           Poly[8][0],Poly[8][1],Poly[8][2],
                           Poly[9][0],Poly[9][1],Poly[9][2],
                           Poly[10][0],Poly[10][1],Poly[10][2],
                           Poly[11][0],Poly[11][1],Poly[11][2],
                           Poly[12][0],Poly[12][1],Poly[12][2],
                           Poly[13][0],Poly[13][1],Poly[13][2],
                           Poly[14][0],Poly[14][1],Poly[14][2],
                           Poly[15][0],Poly[15][1],Poly[15][2],
                           Poly[16][0],Poly[16][1],Poly[16][2],
                           Poly[17][0],Poly[17][1],Poly[17][2],
                           Poly[18][0],Poly[18][1],Poly[18][2],
                           Poly[19][0],Poly[19][1],Poly[19][2],
                           Poly[20][0],Poly[20][1],Poly[20][2]);

     }
}

































//===========发送
void Decode::SendBeat(void)
{
     static quint8 Beat = 0;
     static quint8 RSSI = 0;
     quint8 SendCount = 0;
     QByteArray SendBuff = 0;


     SendBuff[SendCount++] = 0xEB;
     SendBuff[SendCount++] = 0x90;
     SendBuff[SendCount++] = 0x00;
     SendBuff[SendCount++] = 0x00;
     SendBuff[SendCount++] = RSSI++;

     SendBuff[SendCount++] = Beat;

     if(Beat == 1)  Beat = 0;
     else           Beat = 1;

     SendBuff[3] = SendCount - 4;
     quint8 sum = 0;
     for(int i=0;i<SendCount;i++)
     {
         sum += (quint8)SendBuff[i];
     }
     SendBuff[SendCount++] = sum;//0x06
     emit SendFrame(SendBuff);
}

void Decode::SendCMD(quint8 ID,quint8 A,quint8 B,quint8 C,quint8 D,quint8 E,quint8 F,quint8 G,quint8 H)
{
    QByteArray DataToSend = 0;
    quint16 DataCount = 0;

     DataToSend[DataCount++] = 0xEB;
     DataToSend[DataCount++] = 0x90;
     DataToSend[DataCount++] = 0x00 + ID;
     DataToSend[DataCount++] = 0x00;//LEN
     DataToSend[DataCount++] = 0x00;//RSSI

     DataToSend[DataCount++] = (quint8)A;
     DataToSend[DataCount++] = (quint8)B;
     DataToSend[DataCount++] = (quint8)C;
     DataToSend[DataCount++] = (quint8)D;
     DataToSend[DataCount++] = (quint8)E;
     DataToSend[DataCount++] = (quint8)F;
     DataToSend[DataCount++] = (quint8)G;
     DataToSend[DataCount++] = (quint8)H;

     //==========LEN================
     DataToSend[3] =DataCount - 4 ;
     //==========SUM================
     quint8 sum = 0;
     for(int i=0;i<DataCount;i++)
     {
         sum += (quint8)DataToSend[i];
     }
     DataToSend[DataCount++] = sum;
     //=========Send=================
     qDebug() << DataToSend;
     emit SendFrame(DataToSend);
}

void Decode::SendPID( quint8 ID,
                      float PID1_P,float PID1_I,float PID1_D,
                      float PID2_P,float PID2_I,float PID2_D,
                      float PID3_P,float PID3_I,float PID3_D,
                      float PID4_P,float PID4_I,float PID4_D,
                      float PID5_P,float PID5_I,float PID5_D,
                      float PID6_P,float PID6_I,float PID6_D)
{
     union {quint8 B[4];float F;}src;
     QByteArray DataToSend = 0;
     quint16 DataCount = 0;

     DataToSend[DataCount++] = 0xEB;
     DataToSend[DataCount++] = 0x90;
     DataToSend[DataCount++] = 0X30 + ID;//ID
     DataToSend[DataCount++] = 0x00;//LEN
     DataToSend[DataCount++] = 0x00;//RSSI

     src.F = PID1_P;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID1_I;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID1_D;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];

     src.F = PID2_P;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID2_I;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID2_D;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];

     src.F = PID3_P;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID3_I;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID3_D;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];

     src.F = PID4_P;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID4_I;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID4_D;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];

     src.F = PID5_P;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID5_I;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID5_D;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];

     src.F = PID6_P;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID6_I;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];
     src.F = PID6_D;
     DataToSend[DataCount++] = src.B[0];
     DataToSend[DataCount++] = src.B[1];
     DataToSend[DataCount++] = src.B[2];
     DataToSend[DataCount++] = src.B[3];

     //==========LEN================
     DataToSend[3] =DataCount - 4 ;
     //==========SUM================
     quint8 sum = 0;
     for(quint8 i=0;i<DataCount;i++)
      sum += DataToSend[i];
     DataToSend[DataCount++]=sum;
     //=========Send=================
     emit SendFrame(DataToSend);
}

void Decode::SendDot(quint8 Function,quint8 Groups,
                       quint16 TotalPoint,quint16 CurrentPoint,
                       double Longitude,double Latitude,float Altitude,
                       quint16 Velocity,quint16 Radius,quint16 Action)
{
       union {quint8 B[8];quint16 D[4];quint16 W[4];double DD;}src;
       QByteArray DataToSend;
       quint8 DataCount = 0;

       //Function;
       DataToSend[DataCount++] = 0xEB;//
       DataToSend[DataCount++] = 0x90;//
       DataToSend[DataCount++] = 0x41;//ID
       DataToSend[DataCount++] = 0x00;//LEN
       DataToSend[DataCount++] = 0x00;//RSSI
       //Data
       DataToSend[DataCount++] = Function;
       DataToSend[DataCount++] = Groups;
       src.D[0] = TotalPoint;
       DataToSend[DataCount++] = src.B[0];
       DataToSend[DataCount++] = src.B[1];
       src.D[0] = CurrentPoint;
       DataToSend[DataCount++] = src.B[0];
       DataToSend[DataCount++] = src.B[1];
       src.DD = Longitude;
       DataToSend[DataCount++] = src.B[0];
       DataToSend[DataCount++] = src.B[1];
       DataToSend[DataCount++] = src.B[2];
       DataToSend[DataCount++] = src.B[3];
       DataToSend[DataCount++] = src.B[4];
       DataToSend[DataCount++] = src.B[5];
       DataToSend[DataCount++] = src.B[6];
       DataToSend[DataCount++] = src.B[7];
       src.DD = Latitude;
       DataToSend[DataCount++] = src.B[0];
       DataToSend[DataCount++] = src.B[1];
       DataToSend[DataCount++] = src.B[2];
       DataToSend[DataCount++] = src.B[3];
       DataToSend[DataCount++] = src.B[4];
       DataToSend[DataCount++] = src.B[5];
       DataToSend[DataCount++] = src.B[6];
       DataToSend[DataCount++] = src.B[7];
       src.W[0] = Altitude * 10.0f;
       DataToSend[DataCount++] = src.B[0];
       DataToSend[DataCount++] = src.B[1];
       src.D[0] = Velocity;
       DataToSend[DataCount++] = src.B[0];
       DataToSend[DataCount++] = src.B[1];
       src.D[0] = Radius;
       DataToSend[DataCount++] = src.B[0];
       DataToSend[DataCount++] = src.B[1];
       src.D[0] = Action;
       DataToSend[DataCount++] = src.B[0];
       DataToSend[DataCount++] = src.B[1];
       //LEN
       DataToSend[3] = DataCount -4 + 1;
       //CRC
       src.D[0] = CRCcheck(DataToSend);
       DataToSend[DataCount++] = src.B[0];
       DataToSend[DataCount++] = src.B[1];
       //Send
       //qDebug() << "Send";
       emit SendFrame(DataToSend);
}

void Decode::SendTaskSetting(quint8 ID,float A, float B, float C, float D, float E, float F, float G, float H)
{
    union {quint8 B[4]; float F;}src;
    QByteArray DataToSend = 0;
    quint16 DataCount = 0;

    DataToSend[DataCount++] = 0xEB;//0
    DataToSend[DataCount++] = 0x90;
    DataToSend[DataCount++] = 0x50 + ID;//ID
    DataToSend[DataCount++] = 0x00;//LEN
    DataToSend[DataCount++] = 0x10;//RSSI

    src.F = A;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = B;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = C;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = D;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = E;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = F;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = G;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = H;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    //==========LEN================
    DataToSend[3] =DataCount - 4 ;
     //==========SUM================
   quint8 sum = 0;
   for(quint8 i=0;i<DataCount;i++)
     sum += DataToSend[i];
   DataToSend[DataCount++]=sum;
     //=========Send=================

   //qDebug() << DataToSend;
   emit SendFrame(DataToSend);
}

void Decode::SendScale(quint8 ID, float A, float B, float C, float D, float E, float F)
{
    union {quint8 B[4]; float F;}src;
    QByteArray DataToSend = 0;
    quint16 DataCount = 0;

    DataToSend[DataCount++] = 0xEB;//0
    DataToSend[DataCount++] = 0x90;
    DataToSend[DataCount++] = 0XA1;//ID
    DataToSend[DataCount++] = 0x00;//LEN
    DataToSend[DataCount++] = 0x00;//RSSI

    DataToSend[DataCount++] = ID;//ID

    src.F = A;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = B;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = C;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = D;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = E;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    src.F = F;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    DataToSend[DataCount++] = src.B[2];
    DataToSend[DataCount++] = src.B[3];

    //==========LEN================
    DataToSend[3] =DataCount - 4 ;
     //==========SUM================
   quint8 sum = 0;
   for(quint8 i=0;i<DataCount;i++)
     sum += DataToSend[i];
   DataToSend[DataCount++]=sum;
     //=========Send=================
   emit SendFrame(DataToSend);
}

void Decode::SendPWM(quint8 ID, quint16 *PWM)
{
    union {quint8 B[2]; quint16 D;}src;
    QByteArray DataToSend = 0;
    quint16 DataCount = 0;

    DataToSend[DataCount++] = 0xAA;//0
    DataToSend[DataCount++] = 0xAA;
    DataToSend[DataCount++] = 0X10;//ID
    DataToSend[DataCount++] = 0x00;//LEN
    DataToSend[DataCount++] = 0x00;//RSSI

    DataToSend[DataCount++] = 1;//ID

    for(int i = 0;i<16;i++)
    {
        src.D = PWM[i];
        DataToSend[DataCount++] = src.B[0];
        DataToSend[DataCount++] = src.B[1];
    }

    //==========LEN================
    DataToSend[3] =DataCount - 4 ;
     //==========SUM================
   quint8 sum = 0;
   for(quint8 i=0;i<DataCount;i++)
     sum += DataToSend[i];
   DataToSend[DataCount++]=sum;
     //=========Send=================
   emit SendFrame(DataToSend);
}


void Decode::SendServoPolar(quint8 ID, quint8 PWM1, quint8 PWM2, quint8 PWM3,
                            quint8 PWM4, quint8 PWM5, quint8 PWM6, quint8 PWM7,
                            quint8 PWM8, quint8 PWM9, quint8 PWM10, quint8 PWM11,
                            quint8 PWM12, quint8 PWM13, quint8 PWM14, quint8 PWM15,
                            quint8 PWM16, quint8 Freq1, quint8 Freq2, quint8 Freq3, quint8 Freq4)
{
    QByteArray DataToSend = 0;
    quint16 DataCount = 0;

    DataToSend[DataCount++] = 0xEB;//0
    DataToSend[DataCount++] = 0x90;
    DataToSend[DataCount++] = 0XB2;//ID
    DataToSend[DataCount++] = 0x00;//LEN
    DataToSend[DataCount++] = 0x00;//RSSI

    DataToSend[DataCount++] = ID;//ID

    DataToSend[DataCount++] = PWM1;
    DataToSend[DataCount++] = PWM2;
    DataToSend[DataCount++] = PWM3;
    DataToSend[DataCount++] = PWM4;
    DataToSend[DataCount++] = PWM5;
    DataToSend[DataCount++] = PWM6;
    DataToSend[DataCount++] = PWM7;
    DataToSend[DataCount++] = PWM8;
    DataToSend[DataCount++] = PWM9;
    DataToSend[DataCount++] = PWM10;
    DataToSend[DataCount++] = PWM11;
    DataToSend[DataCount++] = PWM12;
    DataToSend[DataCount++] = PWM13;
    DataToSend[DataCount++] = PWM14;
    DataToSend[DataCount++] = PWM15;
    DataToSend[DataCount++] = PWM16;
    DataToSend[DataCount++] = Freq1;
    DataToSend[DataCount++] = Freq2;
    DataToSend[DataCount++] = Freq3;
    DataToSend[DataCount++] = Freq4;

    //==========LEN================
    DataToSend[3] =DataCount - 4 ;
     //==========SUM================
   quint8 sum = 0;
   for(quint8 i=0;i<DataCount;i++)
     sum += DataToSend[i];
   DataToSend[DataCount++]=sum;
     //=========Send=================
   //qDebug() << DataToSend;
   emit SendFrame(DataToSend);
}

void Decode::SendServoPolynomial(quint8 ID,
                                 float Poly_A_PWM1, float Poly_B_PWM1, float Poly_C_PWM1,
                                 float Poly_A_PWM2, float Poly_B_PWM2, float Poly_C_PWM2,
                                 float Poly_A_PWM3, float Poly_B_PWM3, float Poly_C_PWM3,
                                 float Poly_A_PWM4, float Poly_B_PWM4, float Poly_C_PWM4,
                                 float Poly_A_PWM5, float Poly_B_PWM5, float Poly_C_PWM5,
                                 float Poly_A_PWM6, float Poly_B_PWM6, float Poly_C_PWM6,
                                 float Poly_A_PWM7, float Poly_B_PWM7, float Poly_C_PWM7,
                                 float Poly_A_PWM8, float Poly_B_PWM8, float Poly_C_PWM8,
                                 float Poly_A_PWM9, float Poly_B_PWM9, float Poly_C_PWM9,
                                 float Poly_A_PWM10, float Poly_B_PWM10, float Poly_C_PWM10,
                                 float Poly_A_PWM11, float Poly_B_PWM11, float Poly_C_PWM11,
                                 float Poly_A_PWM12, float Poly_B_PWM12, float Poly_C_PWM12,
                                 float Poly_A_PWM13, float Poly_B_PWM13, float Poly_C_PWM13,
                                 float Poly_A_PWM14, float Poly_B_PWM14, float Poly_C_PWM14,
                                 float Poly_A_PWM15, float Poly_B_PWM15, float Poly_C_PWM15,
                                 float Poly_A_PWM16, float Poly_B_PWM16, float Poly_C_PWM16,
                                 float Poly_A_ROTOR1, float Poly_B_ROTOR1, float Poly_C_ROTOR1,
                                 float Poly_A_ROTOR2, float Poly_B_ROTOR2, float Poly_C_ROTOR2,
                                 float Poly_A_ROTOR3, float Poly_B_ROTOR3, float Poly_C_ROTOR3,
                                 float Poly_A_ROTOR4, float Poly_B_ROTOR4, float Poly_C_ROTOR4,
                                 float Poly_A_ROTOR5, float Poly_B_ROTOR5, float Poly_C_ROTOR5)
{
    union {quint8 B[2]; qint16 W;}src;
    QByteArray DataToSend = 0;
    quint16 DataCount = 0;

    DataToSend[DataCount++] = 0xEB;//0
    DataToSend[DataCount++] = 0x90;
    DataToSend[DataCount++] = 0xB3;//ID
    DataToSend[DataCount++] = 0x00;//LEN
    DataToSend[DataCount++] = 0x00;//RSSI

    DataToSend[DataCount++] = ID;//ID

    //=================================
    src.W = Poly_A_PWM1 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM1 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM1 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM2 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM2 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM2 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM3 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM3 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM3 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM4 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM4 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM4 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM5 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM5 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM5 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM6 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM6 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM6 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM7 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM7 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM7 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM8 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM8 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM8 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM9 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM9 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM9 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM10 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM10 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM10 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM11 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM11 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM11 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM12 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM12 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM12 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM13 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM13 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM13 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM14 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM14 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM14 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM15 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM15 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM15 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_PWM16 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_PWM16 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_PWM16 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_ROTOR1 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_ROTOR1 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_ROTOR1 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_ROTOR2 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_ROTOR2 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_ROTOR2 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_ROTOR3 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_ROTOR3 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_ROTOR3 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_ROTOR4 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_ROTOR4 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_ROTOR4 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    //=================================
    src.W = Poly_A_ROTOR5 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_B_ROTOR5 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];
    src.W = Poly_C_ROTOR5 * 1000;
    DataToSend[DataCount++] = src.B[0];
    DataToSend[DataCount++] = src.B[1];


    //==========LEN================
    DataToSend[3] =DataCount - 4 ;
    //==========SUM================
   quint8 sum = 0;
   for(quint8 i=0;i<DataCount;i++)
     sum += DataToSend[i];
   DataToSend[DataCount++]=sum;
     //=========Send=================

   //qDebug() << DataToSend;
   emit SendFrame(DataToSend);
}





















