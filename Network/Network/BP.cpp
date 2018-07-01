#include "stdafx.h"
#include "BP.h"

void BP::GetData(const Vector<Data> v_data) {
    data = v_data;
}

void BP::Train() {
    printf("Begin Training!\n");
    GetNums();
    InitNetWork();
    int num = data.size();

    for (int iter = 0; iter <= ITERS; iter++) {
        for (int cnt = 0; cnt < num; cnt++) {
            if (iter == 0 && cnt % 10 == 0)
                printf("Cnt=%d\n", cnt);
            // 第一层输入节点赋值
            for (int i = 0; i < in_num; i++)
                x[0][i] = data.at(cnt).x[i];

            int i = 0;
            for (; i < 10000000;++i) {
                ForwardTransfer();
                //printf("%lf\n", GetError(cnt));
                if (GetError(cnt) < ERROR)    // 如果误差在阈值内
                    break;
                ReverseTransfer(cnt);
            }
            if (i == 10000000) {
                printf("dead loop\n");
                getchar();
                exit(0);
            }
        }
        printf("This is the %d th trainning NetWork !\n", iter);

        double accu = GetAccu();
        printf("All ccuracy is %lf\n", 1-accu);
        if (accu < ACCU)
            break;

        std::ofstream fp("save.txt");
        fp << ITERS << " " << ETA_W << " " << ETA_B << " " << ERROR << " " << ACCU << " ";
        fp << iter << " ";
        for (int i = 0; i < LAYER; ++i)
            for (int j = 0; j < NUM; ++j)
                for (int k = 0; k < NUM; ++k)
                    fp << w[i][j][k] << " ";
        for (int i = 0; i < LAYER; ++i)
            for (int j = 0; j < NUM; ++j)
                fp << b[i][j] << " ";
        for (int i = 0; i < LAYER; ++i)
            for (int j = 0; j < NUM; ++j)
                fp << x[i][j] << " ";
        for (int i = 0; i < LAYER; ++i)
            for (int j = 0; j < NUM; ++j)
                fp << d[i][j] << " ";
        fp.close();
        //getchar();
    }
    printf("Training End!\n");
}

Vector<double> BP::ForeCast(const Vector<bool> v_data) {
    int n = v_data.size();
    for (int i = 0; i < in_num; i++)
        x[0][i] = v_data[i];

    ForwardTransfer();
    Vector<double> v;
    for (int i = 0; i < ou_num; i++)
        v.push_back(x[2][i]);
    return v;
}

void BP::Load() {
    GetNums();
    std::ifstream fp("save.txt");
    int iter;
    double it, etaw, etab, error, acc;
    fp >> it >> etaw >> etab >> error >> acc;
    fp >> iter;
    for (int i = 0; i < LAYER; ++i)
        for (int j = 0; j < NUM; ++j)
            for (int k = 0; k < NUM; ++k)
                fp>>w[i][j][k];
    for (int i = 0; i < LAYER; ++i)
        for (int j = 0; j < NUM; ++j)
            fp>>b[i][j];
    for (int i = 0; i < LAYER; ++i)
        for (int j = 0; j < NUM; ++j)
            fp>>x[i][j];
    for (int i = 0; i < LAYER; ++i)
        for (int j = 0; j < NUM; ++j)
            fp>>d[i][j];
    fp.close();
#ifndef TEST
    printf("PARAM %lf %lf %lf %lf %lf\n", it, etaw, etab, error, acc);
    printf("Training times %d\n", iter);
#endif
}

void BP::GoOn() {
    Load();
    printf("Begin Training!\n");
    int num = data.size();

    for (int iter = 0; iter <= ITERS; iter++) {
        for (int cnt = 0; cnt < num; cnt++) {
            if (iter == 0 && cnt % 10 == 0)
                printf("Cnt=%d\n", cnt);
            // 第一层输入节点赋值
            for (int i = 0; i < in_num; i++)
                x[0][i] = data.at(cnt).x[i];

            int i = 0;
            for (; i < 10000000; ++i) {
                ForwardTransfer();
                //printf("%lf\n", GetError(cnt));
                if (GetError(cnt) < ERROR)    // 如果误差在阈值内
                    break;
                ReverseTransfer(cnt);
            }
            if (i == 10000000) {
                printf("dead loop\n");
                getchar();
                exit(0);
            }
        }
        printf("This is the %d th trainning NetWork !\n", iter);

        double accu = GetAccu();
        printf("All Samples Accuracy is %lf\n", accu);
        if (accu < ACCU)
            break;

        std::ofstream fp("save.txt");
        fp << ITERS << " " << ETA_W << " " << ETA_B << " " << ERROR << " " << ACCU << " ";
        fp << iter << " ";
        for (int i = 0; i < LAYER; ++i)
            for (int j = 0; j < NUM; ++j)
                for (int k = 0; k < NUM; ++k)
                    fp << w[i][j][k] << " ";
        for (int i = 0; i < LAYER; ++i)
            for (int j = 0; j < NUM; ++j)
                fp << b[i][j] << " ";
        for (int i = 0; i < LAYER; ++i)
            for (int j = 0; j < NUM; ++j)
                fp << x[i][j] << " ";
        for (int i = 0; i < LAYER; ++i)
            for (int j = 0; j < NUM; ++j)
                fp << d[i][j] << " ";
        fp.close();
        //getchar();
    }
    printf("Training End!\n");
}

void BP::GetNums() {
    in_num = 300;
    ou_num = 26;
    hd_num = (int)sqrt((in_num + ou_num) * 1.0) + 5;
    if (hd_num > NUM)
        hd_num = NUM;
}

double BP::GetRand() {
    return 2.0 * rand() / RAND_MAX - 1.0;
}

void BP::InitNetWork() {
    for (int i = 0; i < LAYER; ++i)
        for (int j = 0; j < NUM; ++j)
            for (int k = 0; k < NUM; ++k)
                w[i][j][k] = GetRand();
    memset(b, 0, sizeof(b));
}

void BP::ForwardTransfer() {
    // 计算隐含层各个节点的输出值
    for (int j = 0; j < hd_num; j++) {
        double t = 0;
        for (int i = 0; i < in_num; i++)
            t += w[1][i][j] * x[0][i];
        t += b[1][j];
        x[1][j] = Sigmoid(t);
    }

    // 计算输出层各节点的输出值
    for (int j = 0; j < ou_num; j++) {
        double t = 0;
        for (int i = 0; i < hd_num; i++)
            t += w[2][i][j] * x[1][i];
        t += b[2][j];
        x[2][j] = Sigmoid(t);
    }
}

double BP::GetError(int cnt) {
    double ans = 0;
    for (int i = 0; i < ou_num; i++)
        ans += 0.5 * (x[2][i] - data.at(cnt).y[i]) * (x[2][i] - data.at(cnt).y[i]);
    return ans;
}

void BP::ReverseTransfer(int cnt) {
    CalcDelta(cnt);
    UpdateNetWork();
}

double BP::GetAccu() {
    double ans = 0;
    int num = data.size();
    for (int i = 0; i < num; i++) {
        int m = 300;
        for (int j = 0; j < m; j++)
            x[0][j] = data.at(i).x[j];
        ForwardTransfer();
        int n = 26;
        for (int j = 0; j < n; j++)
            ans += 0.5 * (x[2][j] - data.at(i).y[j]) * (x[2][j] - data.at(i).y[j]);
    }
    return ans / num;
}

#define A 30.0
#define B 10.0

void BP::CalcDelta(int cnt) {
    // 计算输出层的delta值
    for (int i = 0; i < ou_num; i++)
        d[2][i] = (x[2][i] - data.at(cnt).y[i]) * x[2][i] * (A - x[2][i]) / (A * B);
    // 计算隐含层的delta值
    for (int i = 0; i < hd_num; i++) {
        double t = 0;
        for (int j = 0; j < ou_num; j++)
            t += w[2][i][j] * d[2][j];
        d[1][i] = t * x[1][i] * (A - x[1][i]) / (A * B);
    }
}

void BP::UpdateNetWork() {
    // 隐含层和输出层之间权值和阀值调整
    for (int i = 0; i < hd_num; i++) {
        for (int j = 0; j < ou_num; j++)
            w[2][i][j] -= ETA_W * d[2][j] * x[1][i];
    }
    for (int i = 0; i < ou_num; i++)
        b[2][i] -= ETA_B * d[2][i];

    // 输入层和隐含层之间权值和阀值调整
    for (int i = 0; i < in_num; i++) {
        for (int j = 0; j < hd_num; j++)
            w[1][i][j] -= ETA_W * d[1][j] * x[0][i];
    }
    for (int i = 0; i < hd_num; i++)
        b[1][i] -= ETA_B * d[1][i];
}

double BP::Sigmoid(const double x) {
    return A / (1 + exp(-x / B));
}
