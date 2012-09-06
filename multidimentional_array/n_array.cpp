#include "n_array.h"
#include <iostream>

using namespace std;

int main ()
{
    n_array<int, 1> one = {1};
    //one[0] = 1;

    cout << one[0] << ": " << one.size() << endl;

    for (auto e : one)
    {
        cout << e << endl;
    }

    n_array<int, 1, 2> two = {0, 1};
    //two[0][0] = 0;
    //two[0][1] = 1;

    cout << two[0][0] << " " << two[0][1] << ": " << two.size() << endl;


    n_array<int, 3, 2, 1> three = { 0, 1, 2, 3, 4, 5 };

    cout << three[0][0][0] << " "
         << three[0][1][0] << " "
         << three[1][0][0] << " "
         << three[1][1][0] << " "
         << three[2][0][0] << " "
         << three[2][1][0] << ": " << three.size() << endl;
           
    for (auto e : three)
    {
        cout << e << " ";
    }
    cout << endl;

    for (auto e : three.at(1))
    {
        cout << e << " ";
    }
    cout << endl;

    for (auto e : three.at(2)[1])
    {
        cout << e << " ";
    }
    cout << endl;


    try
    {
        three.at(3);
        cout << "Ooops... failed to catch out of range exception" << endl;
    }
    catch (const std::out_of_range &)
    {
        cout << "Caught out of range exception" << endl;
    }

    cout << "Three front: " << three.front() << endl;
    cout << "Three back: " << three.back() << endl;


    n_array<int, 0, 0, 0, 0, 0, 0> none;

    cout << "Three is empty? " << three.empty() << endl;
    cout << "None is empty? " << none.empty() << endl;


    three.fill(9);
    three[0].fill(8);
    three[2][0].fill(7);

    for (auto e : three)
    {
        cout << e << " ";
    }
    cout << endl;

    return 0;
}

