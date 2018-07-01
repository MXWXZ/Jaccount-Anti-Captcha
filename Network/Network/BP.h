#pragma once

//#define TRAINING
//#define GOON
#define TEST

#define LAYER    3            // 神经网络层数
#define NUM      300          // 每层最多节点数

#define ITERS    10000          // 最大训练次数
#define ETA_W    0.00003      // 权值调整率
#define ETA_B    0.001          // 阀值调整率
#define ERROR    0.005        // 单个样本允许的误差
#define ACCU     0.0001       // 每次迭代允许的误差

#define Vector std::vector 

struct Data {
    bool x[300];      // 输入数据
    double y[26];     // 输出数据
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
    void InitNetWork();           // 初始化网络
    void GetNums();               // 获取输入、输出和隐含层节点数
    void ForwardTransfer();       // 正向传播
    void ReverseTransfer(int);    // 反向传播
    void CalcDelta(int);          // 计算w和b的调整量
    void UpdateNetWork();         // 更新权值和阀值
    double GetError(int);         // 计算单个样本的误差
    double GetAccu();             // 计算所有样本的精度
    double Sigmoid(const double); // 计算Sigmoid的值

private:
    int in_num;                   // 输入层节点数
    int ou_num;                   // 输出层节点数
    int hd_num;                   // 隐含层节点数

    Vector<Data> data;            // 输入输出数据

    double w[LAYER][NUM][NUM];    // BP网络的权值
    double b[LAYER][NUM];         // BP网络节点的阀值

    double x[LAYER][NUM];         // 每个神经元的输出值
    double d[LAYER][NUM];         // delta值
};