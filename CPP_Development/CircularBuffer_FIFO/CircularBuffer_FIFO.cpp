
#define CIRCULAR_BUFFER_LENGTH 10

#include "CircularBuffer_FIFO.h"

void circularBuffer_test()
{
    const unsigned int TOTAL_NUMBER = 7;
    CircularBuffer_FIFO<uint8_t> cb_fifo_1;
    uint8_t data[TOTAL_NUMBER] = { 11, 12, 13, 14 , 15, 16, 17 };
    uint8_t data1[TOTAL_NUMBER];
    uint8_t tempData;

    cout << "### Number 1 :: START ###" << endl;
    cout << cb_fifo_1 << endl;
    cb_fifo_1.pop();
    cb_fifo_1.pop(data, TOTAL_NUMBER);
    cout << "--- Number 1 :: END --- " << endl;


    cout << "### Number 2 :: START ###" << endl;
    cb_fifo_1.push(2);
    cout << cb_fifo_1 << "--- Number 2 :: END ---" << endl;


    cout << "### Number 3 :: START ###" << endl;
    cb_fifo_1.push(3);
    cout << cb_fifo_1 << "--- Number 3 :: END ---" << endl;


    cout << "### Number 4 :: START ###" << endl;
    tempData = cb_fifo_1.pop();
    cout << "tempData: " << (long long int)tempData << endl;
    cout << cb_fifo_1 << "--- Number 4 :: END ---" << endl;


    cout << "### Number 5 :: START ###" << endl;
    cb_fifo_1.push(data, TOTAL_NUMBER);
    cout << cb_fifo_1 << "--- Number 5 :: END ---" << endl;

    cout << "### Number 6 :: START ###" << endl;
    tempData = cb_fifo_1.pop();
    cout << "tempData: " << (long long int)tempData << endl;
    cb_fifo_1.push(4);
    cb_fifo_1.push(5);
    cout << cb_fifo_1 << "--- Number 6 :: END ---" << endl;

    cout << "### Number 7 :: START ###" << endl;
    cb_fifo_1.pop(data1, TOTAL_NUMBER);
    cout << cb_fifo_1 << "--- Number 7 :: END ---" << endl;

    cout << "### Number 8 :: START ###" << endl;
    cb_fifo_1.pop(data, TOTAL_NUMBER);
    cout << cb_fifo_1 << "--- Number 8 :: END ---" << endl;

    cout << "### Number 9 :: START ###" << endl;
    cb_fifo_1.push(data1, TOTAL_NUMBER);
    cout << cb_fifo_1 << "--- Number 9 :: END ---" << endl;

}
