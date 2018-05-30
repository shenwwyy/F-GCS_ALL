#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "QGLWidget"
#include "QOpenGLWidget"
#include <QOpenGLFunctions>
#include "QColor"
#include "calibration.h"
class GLWidget : public QGLWidget
{
    Q_OBJECT

#define ABS(x) ( (x)>0?(x):-(x) )
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


    typedef struct {
        float x;
        float y;
        float z;
    }_xyz_f;



public:
    explicit GLWidget(QWidget *parent = 0);

    void setValue(double x,double y,double z);
    void setRotation(double Pitch,double Roll,double Yaw);
    void setScale(double x,double y,double z);
    void setColor(double x,double y,double z);
    void setBackColor(double x,double y,double z);
    void setStandard_R(double R);

    void CalOffset(float *BiasX, float *BiasY, float *BiasZ, float *ScaleX, float *ScaleY, float *ScaleZ);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void DrawRawData(void);
    void DrawCalData(void);
    void DrawABall(void);
private:
    double XValue,YValue,ZValue;
    double XRotation,YRotation,ZRotation;
    float XZoom,YZoom,ZZoom;
    float XColor,YColor,ZColor;
    float XBackColor,YBackColor,ZBackColor;
    float XScale,YScale,ZScale;
    float Standard_R;
    quint64 RawDataCount;
    float RawData[3][8640000];//这个数组的大小可以校准100Hz频率数据 24小时

    _xyz_f MagOffset;
    _xyz_f Maggain;
    _xyz_f MagSum;
    _xyz_f MagMAX,MagMIN;//一个反向数组，用于保证得到数据以后一定会是正确的
    bool isCompelect;

};

#endif // GLWIDGET_H
