// We want to create a class for a queue of size n and call member functions such as push, pop, see all.
// 
#include <iostream>

int rngGenerator(int min = 0, int max = 6)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(min, max);
    return uni(rng);
}

class queue {

    private:
        int queueSize;
        int queueLength;
        std::pair<int,int> *queueArray;
        std::pair<int,int> queueHead;
        std::pair<int,int> queueTail;

    public:
        queue(int queueSize = 5)
        {
            this->queueSize = queueSize;
            this->queueLength = 0;

            this->queueArray = new std::pair<int,int>[queueSize];

            this->queueHead.first = 0;
            this->queueHead.second = 0;
            this->queueTail.first = 0;
            this->queueTail.second = 0;
        }

        // Fundamental Queue Methods
        void push(std::pair<int,int> pieceInfo)
        {
            if (queueLength < queueSize)
            {
                queueArray[queueTail.first].first = pieceInfo.first;
                queueArray[queueTail.first].second = pieceInfo.second;
                queueTail.first = (queueTail.first + 1) % queueSize;
                queueLength++;
            }
        }
        
        std::pair<int,int> pop()
        {
            if (queueLength > 0)
            {
                std::pair<int,int> pieceInfo{queueArray[queueHead.first].first, queueArray[queueHead.first].second};
                queueHead.first = (queueHead.first + 1) % queueSize;
                queueLength--;
                return pieceInfo;
            }

            return std::pair<int,int>{-1,-1};
        }

        // Queue arrangement
        // Always push head to the front
        void rearrange()
        {
            std::pair<int,int> *tempArray = new std::pair<int,int>[queueSize];
            int tempIndex = 0;

            for (int i = queueHead.first; i < queueSize; i++)
            {
                tempArray[tempIndex].first = queueArray[i].first;
                tempArray[tempIndex].second = queueArray[i].second;
                tempIndex++;
            }

            for (int i = 0; i < queueHead.first; i++)
            {
                tempArray[tempIndex].first = queueArray[i].first;
                tempArray[tempIndex].second = queueArray[i].second;
                tempIndex++;
            }

            queueHead.first = 0;
            queueTail.first = queueLength;

            delete[] queueArray;
            queueArray = tempArray;
        }

        // Tetris Methods     
        std::pair<int,int> generate()
        {
            return {rngGenerator(0,6), rngGenerator(0,3)};            
        }

        void generateMax()
        {
            while(queueLength < queueSize)
            {
                push(generate());
            }
        }

        std::pair<int,int> popAndGenerate()
        {
            std::pair<int,int> pieceInfo = pop();
            push(generate());
            rearrange();
            return pieceInfo;
        }

        // Return Methods
        std::pair<int,int> *array()
        {
            return queueArray;
        }

        std::pair<int,int> peek(int index)
        {
            return queueArray[index];
        }

        int length()
        {
            return queueLength;
        }

        int size()
        {
            return queueSize;
        }        

        // Print Methods
        void print()
        {
            for (int i = 0; i < queueSize; i++)
            {
                std::cout << queueArray[i].first << " " << queueArray[i].second << std::endl;
            }
        }
        
};
