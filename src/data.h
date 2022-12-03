#ifndef __DATA_H__
#define __DATA_H__

#include <fstream>

using namespace std;

class Data
{
public:
    Data();
    ~Data();
    double m_lat;
    double m_lon;
    double m_alt;

    friend istream &operator>>(istream &input, Data &data);
    friend ostream &operator<<(ostream &output, const Data &data);
};

#endif
