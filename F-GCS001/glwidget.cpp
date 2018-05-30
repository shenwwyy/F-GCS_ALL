#include "glwidget.h"
#include <QtGui>
#include <QtOpenGL>
#include <math.h>
GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    XValue    = 0;
    YValue    = 0;
    ZValue    = 0;

    XRotation = 45;
    YRotation = 45;
    ZRotation = 45;

    XZoom = 0;
    YZoom = 0;
    ZZoom = 0;

    XColor = 1.0f;
    YColor = 1.0f;
    ZColor = 1.0f;

    XBackColor = 1.0f;
    YBackColor = 1.0f;
    ZBackColor = 1.0f;

    XScale = 1.0f;
    YScale = 1.0f;
    ZScale = 1.0f;

    Standard_R = 0.5f;

    RawDataCount = 1;

    isCompelect = false;

    MagMAX.x = -32767;
    MagMAX.y = -32767;
    MagMAX.z = -32767;

    MagMIN.x =  32767;
    MagMIN.y =  32767;
    MagMIN.z =  32767;


}

void GLWidget::initializeGL()
{
    glShadeModel(GL_SMOOTH);//设置阴影平滑模式
    glClearColor(XBackColor,YBackColor,ZBackColor, 1.0);//改变窗口的背景颜色，不过我这里貌似设置后并没有什么效果
    glClearDepth(1.0);//设置深度缓存
    glEnable(GL_DEPTH_TEST);//允许深度测试
    glDepthFunc(GL_LEQUAL);//设置深度测试类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//进行透视校正
}

void GLWidget::paintGL()
{
    glClearColor(XBackColor,YBackColor,ZBackColor, 1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glTranslatef(0.0,0.0,ZZoom-20.0);//缩放

    glRotatef(XRotation, 1.0, 0.0, 0.0);
    glRotatef(YRotation, 0.0, 1.0, 0.0);
    glRotatef(ZRotation, 0.0, 0.0, 1.0);

    glBegin(GL_LINE_LOOP);
    glColor3f(XColor, 0.0, 0.0);
    glVertex3f(-XScale,0.0,0.0);
    glVertex3f( XScale,0.0,0.0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(XColor, 0.0, 0.0);
    glVertex3f(XScale - XScale/10.0f,-XScale/10.0f,0.0);
    glVertex3f( XScale,0.0,0.0);
    glVertex3f(XScale - XScale/10.0f,XScale/10.0f,0.0);
    glVertex3f( XScale,0.0,0.0);
    glVertex3f(XScale - XScale/10.0f,0.0,-XScale/10.0f);
    glVertex3f( XScale,0.0,0.0);
    glVertex3f(XScale - XScale/10.0f,0.0, XScale/10.0f);
    glVertex3f( XScale,0.0,0.0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0.0,YColor, 0.0);
    glVertex3f(0.0,-YScale,0.0);
    glVertex3f(0.0, YScale,0.0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0, YColor, 0.0);
    glVertex3f(0.0,YScale - YScale/10.0f,-YScale/10.0f);
    glVertex3f(0.0,YScale,0.0);
    glVertex3f(0.0,YScale - YScale/10.0f,YScale/10.0f);
    glVertex3f(0.0,YScale,0.0);
    glVertex3f(-YScale/10.0f,YScale - YScale/10.0f,0.0);
    glVertex3f(0.0,YScale,0.0);
    glVertex3f(YScale/10.0f,YScale - YScale/10.0f,0.0);
    glVertex3f(0.0,YScale,0.0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0,ZColor);
    glVertex3f(0.0,0.0,-ZScale);
    glVertex3f(0.0,0.0, ZScale);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, ZColor);
    glVertex3f(-ZScale/10.0f,0.0,ZScale - ZScale/10.0f);
    glVertex3f( 0.0,0.0,ZScale);
    glVertex3f( ZScale/10.0f,0.0,ZScale - ZScale/10.0f);
    glVertex3f( 0.0,0.0,ZScale);
    glVertex3f( 0.0,-ZScale/10.0f,ZScale - ZScale/10.0f);
    glVertex3f( 0.0,0.0,ZScale);
    glVertex3f( 0.0, ZScale/10.0f,ZScale - ZScale/10.0f);
    glVertex3f( 0.0,0.0,ZScale);
    glEnd();

    DrawRawData();
    if(isCompelect == true)
    {
        DrawCalData();
    }
}

void GLWidget::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((width() - side) / 2, (height() - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.2, 1.2, -1.2, 1.2, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -40.0);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
   event = event;
   updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
   event= event;

   XRotation = -(event->pos().ry());
   YRotation = (event->pos().rx());
   ZRotation = 0;
   updateGL();

}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    QPoint Point =event->angleDelta();
    if(Point.y() > 0)
    {
        XZoom++;
        YZoom++;
        ZZoom++;
        //if(ZZoom >= 15) ZZoom = 15;
    }
    else
    {
        XZoom--;
        YZoom--;
        ZZoom--;
        //if(ZZoom <= -40) ZZoom = -40;
    }
    updateGL();
}


void GLWidget::setValue(double x,double y,double z)
{
    XValue = x;
    YValue = y;
    ZValue = z;

    RawData[0][RawDataCount] = x;
    RawData[1][RawDataCount] = y;
    RawData[2][RawDataCount] = z;
    RawDataCount ++;
    updateGL();
}

void GLWidget::setRotation(double Pitch,double Roll,double Yaw)
{
    XRotation = Pitch;
    YRotation = Roll;
    ZRotation = Yaw;
    updateGL();
}

void GLWidget::setScale(double x,double y,double z)
{
    XScale = x;
    YScale = y;
    ZScale = z;
    updateGL();
}

void GLWidget::setColor(double x,double y,double z)
{
    XColor = x;
    YColor = y;
    ZColor = z;
    updateGL();
}


void GLWidget::setBackColor(double x,double y,double z)
{
    XBackColor = x;
    YBackColor = y;
    ZBackColor = z;
    updateGL();
}




void GLWidget::setStandard_R(double R)
{
    Standard_R = R;
    updateGL();
}





void GLWidget::DrawRawData(void)
{
    //设置颜色

    glColor3f(0.5,0.0,0.5);
    glBegin(GL_POINTS);
    for(quint64 i = 1;i<RawDataCount;i++)
    {
       glVertex3f(RawData[0][i],RawData[1][i],RawData[2][i]);
    }
    glEnd();

}

void GLWidget::DrawCalData(void)
{
    //设置颜色

    glColor3f(0.5,1.0,0.0);
    glBegin(GL_POINTS);
    for(quint64 i = 1;i<RawDataCount;i++)
    {
       glVertex3f((RawData[0][i] - MagOffset.x) * Maggain.x,
                  (RawData[1][i] - MagOffset.y) * Maggain.y,
                  (RawData[2][i] - MagOffset.z) * Maggain.z);
    }
    glEnd();

}

void GLWidget::CalOffset(float *BiasX,float *BiasY,float *BiasZ,float *ScaleX,float *ScaleY,float *ScaleZ)
{
     for(quint64 i = 3;i<RawDataCount;i++)
     {
        if(ABS(RawData[0][i])<32768&&ABS(RawData[1][i])<32768&&ABS(RawData[2][i])<32768)
        {
            MagMAX.x = MAX(RawData[0][i], MagMAX.x);
            MagMAX.y = MAX(RawData[1][i], MagMAX.y);
            MagMAX.z = MAX(RawData[2][i], MagMAX.z);

            MagMIN.x = MIN(RawData[0][i], MagMIN.x);
            MagMIN.y = MIN(RawData[1][i], MagMIN.y);
            MagMIN.z = MIN(RawData[2][i], MagMIN.z);
        }
     }

      MagOffset.x = (float)((MagMAX.x + MagMIN.x) * 0.5f);
      MagOffset.y = (float)((MagMAX.y + MagMIN.y) * 0.5f);
      MagOffset.z = (float)((MagMAX.z + MagMIN.z) * 0.5f);

      MagSum.x = (MagMAX.x - MagMIN.x);
      MagSum.y = (MagMAX.y - MagMIN.y);
      MagSum.z = (MagMAX.z - MagMIN.z);

      Maggain.x = 1;
      Maggain.y = MagSum.x / MagSum.y;
      Maggain.z = MagSum.x / MagSum.z;

      qDebug() << endl << "gl" << endl
               << MagOffset.x << MagOffset.y << MagOffset.z << endl
               << Maggain.x   << Maggain.y   << Maggain.z   << endl
               << MagSum.x    << MagSum.y    << MagSum.z    << endl
               << MagMAX.x    << MagMAX.y    << MagMAX.z    << endl
               << MagMIN.x    << MagMIN.y    << MagMIN.z    << endl;

      *BiasX = MagOffset.x;
      *BiasY = MagOffset.y;
      *BiasZ = MagOffset.z;

      *ScaleX = Maggain.x;
      *ScaleY = Maggain.y;
      *ScaleZ = Maggain.z;

      isCompelect = true;
}


void GLWidget::DrawABall(void)
{
    float R = Standard_R;
    float X,Y,Z;
    float r;
    //设置颜色
    glColor3f(0.6,0.4,0.1);
    glBegin(GL_POINTS);
    for(X= -R;X <= R;X += (R/100.0f))
    {
       r = sqrt(R*R - X*X);
       for(Y = -r;Y<r;Y += (r/100.0f))
       {
           Z = sqrt(r*r - Y*Y);
           glVertex3f(X,Y,Z);
           glVertex3f(X,Y,-Z);
       }
    }
    glEnd();
}














