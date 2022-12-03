#include "data.h"

using namespace std;

Data::Data()
{
}

Data::~Data()
{
}

istream &operator>>(istream &input, Data &data)
{
    char space;
    input >> data.m_lat >> space >> data.m_lon >> space >> data.m_alt;
    return input;
}

ostream &operator<<(ostream &output, const Data &data)
{
    output << data.m_lat << " " << data.m_lon << " " << data.m_alt;
    return output;
}
