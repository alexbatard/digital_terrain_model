#include <fstream>
#include <iostream>
#include <list>
#include "data.h"

using namespace std;

// struct Data
// {
//     double lat;
//     double lon;
//     double alt;

//     friend istream &operator>>(istream &input, Data &data)
//     {
//         char space;
//         input >> data.lat >> space >> data.lon >> space >> data.alt;
//         return input;
//     }

//     friend ostream &operator<<(ostream &output, const Data &data)
//     {
//         output << data.lat << " " << data.lon << " " << data.alt;
//         return output;
//     }
// };

void print(list<Data> const &list)
{
    for (auto const &i : list)
    {
        cout.precision(10);
        cout << i << endl;
    }
}

void get_data(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        exit(1);
    }

    list<Data> database;
    Data data;
    while (file >> data)
    {
        database.push_back(data);
    }
    print(database);

    file.close();
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "Error: create_raster takes 2 arguments" << endl;
        cout << "Data file path & image size in px" << endl;
        exit(1);
    };
    string filename = argv[1];
    // int size = int(argv[2]);
    get_data(filename);
    return 0;
}