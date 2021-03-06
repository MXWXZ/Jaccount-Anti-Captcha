// Network.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BP.h"

struct Res {
    char chr;
    double value;
};

int main()
{
    srand((unsigned)time(0));
#ifndef TEST
    FILE* fp = _tfopen(_T("./data.txt"), _T("r"));
    Vector<Data> data;
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        Data t;
        for (int i = 0; i < 26; ++i)
            if (i != ch - 'a')
                t.y[i]=0;
            else
                t.y[i]=1;
        for (int i = 0; i < 300; ++i)
            t.x[i]=fgetc(fp)-'0';
        data.push_back(t);
        fgetc(fp);     // eat /n
    }

    fclose(fp);
#endif

    BP *bp = new BP();
#ifndef TEST
    bp->GetData(data);
#ifdef GOON
    bp->GoOn();
#else
    bp->Train();
#endif
#endif

#ifdef TEST
    bp->Load();
    FILE* fp = _tfopen(_T("./test.txt"), _T("r"));
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        Vector<bool> in;
        in.push_back(ch - '0');
        for (int i = 1; i < 300; ++i)
            in.push_back(fgetc(fp) - '0');
        Vector<double> out;
        out = bp->ForeCast(in);
        Res res[26];
        for (int i = 0; i < 26; ++i) {
            res[i].chr = 'a' + i;
            res[i].value = out[i];
        }
        std::sort(res, res + 26, [](const Res& r1, const Res& r2) { return r1.value > r2.value; });
        printf("%c",res[0].chr);  
        fgetc(fp);     // eat /n
    }
    fclose(fp);
    printf("\n");
#endif
#ifndef TEST
    getchar();
#endif
    return 0;
}

