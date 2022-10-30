#pragma once

#ifndef __CIRCULARBUFFER_FIFO_H__
#define __CIRCULARBUFFER_FIFO_H__

#include <iostream>
#include <string>
#include <stdint.h>

using namespace std;


/*
* push pop:
*   First push data, then increment HEAD(index)
*   Pop data then increment TAIL(index)
* 
* EMPTY:
*   when TAIL and HEAD are at same index and count == 0, check before pop
* 
* FULL:
*   when HEAD and TAIL are at same index and count == LENGTH, check before push
*   
*/

// CircularBuffer_FIFO status and error
typedef enum 
{
    EMPTY        = 'E',
    FULL         = 'F',
    PARTIAL      = 'P',
    INDEX_ERROR  = 'I',
    OTHER_ERROR  = 'O'
}CB_FIFO_STATUS;


template <class T>
class CircularBuffer_FIFO
{

public:
#ifdef CIRCULAR_BUFFER_LENGTH
    const static uint32_t LENGTH = CIRCULAR_BUFFER_LENGTH;
#else
    const static uint32_t LENGTH = 1;
#endif
private:
    T buffer[CircularBuffer_FIFO::LENGTH];
    uint32_t head = 0;
    uint32_t tail = 0;
    CB_FIFO_STATUS CB_fifo_status = CB_FIFO_STATUS::EMPTY;
    uint32_t count = 0;
public:
    // when HEAD and TAIL are at same index and count == LENGTH, check before push
    // First push data, then increment HEAD(index)
    void push(T data);
    void push(T* dataBuf, uint32_t numberOfTimes);

    // when TAIL and HEAD are at same index and count == 0, check before pop
    // Pop data then increment TAIL(index)
    T pop();
    void pop(T* dataBuf, uint32_t numberOfTimes);

    uint32_t getCount() { return count; };
    uint32_t getLength() { return CircularBuffer_FIFO::LENGTH; }
    uint32_t getHeadIndex() { return head; }
    uint32_t getTailIndex() { return tail; };
    T* getBuffer() { return buffer; };

    
    uint8_t checkStatus();
    void increment_index(uint32_t* pIndex);

    template<class U>
    friend ostream& operator << (ostream& out_stream, CircularBuffer_FIFO<U>& cb_fifo);

};





// when HEAD and TAIL are at same index and count == LENGTH, check before push
// First push data, then increment HEAD(index)
template<class T>
void CircularBuffer_FIFO<T>::push(T data)
{
    if (CB_FIFO_STATUS::FULL != checkStatus())
    {
        buffer[head] = data;
        increment_index(&head);
        count++;
    }
    else
    {
        cout << "Buffer is FULL ..." << endl;
    }

}
template<class T>
void CircularBuffer_FIFO<T>::push(T* dataBuf, uint32_t numberOfTimes)
{
    for (uint32_t it = 0; it < numberOfTimes; it++)
    {
        if (CB_FIFO_STATUS::FULL != checkStatus())
        {
            buffer[head] = dataBuf[it];
            increment_index(&head);
            count++;
        }
        else
        {
            // break is also an option here if required!
            cout << "Buffer is FULL, in loop ..." << endl;
        }
    }
}


// when TAIL and HEAD are at same index and count == 0, check before pop
// Pop data then increment TAIL(index)
template<class T>
T CircularBuffer_FIFO<T>::pop()
{
    if (CB_FIFO_STATUS::EMPTY != checkStatus())
    {
        T temp = buffer[tail];
        buffer[tail] = (T)0;
        increment_index(&tail);
        count--;
        return temp;
    }
    else
    {
        cout << "Buffer is EMPTY ..." << endl;
        return (T)0;
    }
}
template<class T>
void CircularBuffer_FIFO<T>::pop(T* dataBuf, uint32_t numberOfTimes)
{
    for (uint32_t it = 0; it < numberOfTimes; it++)
    {
        if (CB_FIFO_STATUS::EMPTY != checkStatus())
        {
            dataBuf[it] = buffer[tail];
            buffer[tail] = (T)0;
            increment_index(&tail);
            count--;
        }
        else
        {
            // break is also an option here if required!
            cout << "Buffer is EMPTY, in loop ..." << endl;
        }
    }
}


template<class T>
uint8_t CircularBuffer_FIFO<T>::checkStatus()
{
    CB_fifo_status = CB_FIFO_STATUS::OTHER_ERROR;

    if ((tail >= LENGTH) || (head >= LENGTH))
    {
        CB_fifo_status = CB_FIFO_STATUS::INDEX_ERROR;
    }
    if ((tail == head) && (count == LENGTH))
    {
        CB_fifo_status = CB_FIFO_STATUS::FULL;
    }
    if ((tail == head) && (count == 0))
    {
        CB_fifo_status = CB_FIFO_STATUS::EMPTY;
    }
    if (tail != head)
    {
        CB_fifo_status = CB_FIFO_STATUS::PARTIAL;
    }

    return CB_fifo_status;
}


template<class T>
void CircularBuffer_FIFO<T>::increment_index(uint32_t* pIndex)
{
    if (*pIndex == (LENGTH - 1))
        *pIndex = 0;
    else
        *pIndex = (*pIndex) + 1;
}


template<class U>
ostream& operator<<(ostream& out_stream, CircularBuffer_FIFO<U>& cb_fifo)
{
    U* pDataBuf = cb_fifo.getBuffer();
    uint32_t tempLength = cb_fifo.getLength();

    out_stream << "---------------------------------------------------------\n";
    out_stream << "*   First push data, then increment HEAD(index)         *\n";
    out_stream << "*   Pop data then increment TAIL(index)                 *\n";
    out_stream << "cb_fifo.getStatus(): " << dec << cb_fifo.checkStatus() << "\n";
    out_stream << "cb_fifo.getLength(): " << dec << tempLength << "\n";
    out_stream << "cb_fifo.getCount(): " << cb_fifo.getCount() << "\n";
    out_stream << "cb_fifo.getHeadIndex(): " << cb_fifo.getHeadIndex() << "\n";
    out_stream << "cb_fifo.getTailIndex(): " << cb_fifo.getTailIndex() << "\n";
    out_stream << "Data: ";
    for (uint32_t it = 0; it < tempLength; it++)
    {
        out_stream << (long long int)pDataBuf[it] << ", ";
    }
    out_stream << "\n---------------------------------------------------------\n" << endl;

    return out_stream;
}

void circularBuffer_test();

#endif // __CIRCULARBUFFER_FIFO_H__
