#pragma once

//#define TRAINING
//#define GOON
#define TEST

#define LAYER    3            // ���������
#define NUM      300          // ÿ�����ڵ���

#define ITERS    10000          // ���ѵ������
#define ETA_W    0.00003      // Ȩֵ������
#define ETA_B    0.001          // ��ֵ������
#define ERROR    0.005        // ����������������
#define ACCU     0.0001       // ÿ�ε�����������

#define Vector std::vector 

struct Data {
    bool x[300];      // ��������
    double y[26];     // �������
};

class BP {
public:
    void GetData(const Vector<Data> v_data);
    void Train();
    Vector<double> ForeCast(const Vector<bool> v_data);
    void Load();
    void GoOn();

private:
    double GetRand();
    void InitNetWork();           // ��ʼ������
    void GetNums();               // ��ȡ���롢�����������ڵ���
    void ForwardTransfer();       // ���򴫲�
    void ReverseTransfer(int);    // ���򴫲�
    void CalcDelta(int);          // ����w��b�ĵ�����
    void UpdateNetWork();         // ����Ȩֵ�ͷ�ֵ
    double GetError(int);         // ���㵥�����������
    double GetAccu();             // �������������ľ���
    double Sigmoid(const double); // ����Sigmoid��ֵ

private:
    int in_num;                   // �����ڵ���
    int ou_num;                   // �����ڵ���
    int hd_num;                   // ������ڵ���

    Vector<Data> data;            // �����������

    double w[LAYER][NUM][NUM];    // BP�����Ȩֵ
    double b[LAYER][NUM];         // BP����ڵ�ķ�ֵ

    double x[LAYER][NUM];         // ÿ����Ԫ�����ֵ
    double d[LAYER][NUM];         // deltaֵ
};