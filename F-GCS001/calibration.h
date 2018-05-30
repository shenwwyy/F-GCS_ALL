#ifndef CALIBRATION_H
#define CALIBRATION_H


#include "QObject"
#include "QString"
#include "QSettings"
#include "QDir"
class Calibration:public QObject
{
    Q_OBJECT
public:
    Calibration();
   ~Calibration();

    void Calc_Process(double radius);
    void CalcData_Input(double x , double y , double z);

private slots:

    double Abs(double a);
    quint8 Equal(double a,double b);
    void ResetMatrix(void);

    void SwapRow(int row1 , int row2);
    void MoveBiggestElement2Top(int s_row , int s_column);
    quint8 Matrix_GaussElimination(void);
    int  Matrix_RowSimplify(void);
    void Matrix_Solve(double* C , double* sol);

private:






};



#endif // CALIBRATION_H
