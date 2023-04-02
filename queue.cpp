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

struct pieceInformation
{
    int shapeNumber;
    int orientationNumber;
};

class queue {

    private:
        int queueSize;
        int queueLength;
        pieceInformation *queueArray;
        std::pair<int,int> queueHead;
        std::pair<int,int> queueTail;

    public:
        queue(int queueSize = 5)
        {
            this->queueSize = queueSize;
            this->queueLength = 0;

            this->queueArray = new pieceInformation[queueSize];

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
                queueArray[queueTail.first].shapeNumber = pieceInfo.first;
                queueArray[queueTail.first].orientationNumber = pieceInfo.second;
                queueTail.first = (queueTail.first + 1) % queueSize;
                queueLength++;
            }
        }
        
        std::pair<int,int> pop()
        {
            if (queueLength > 0)
            {
                std::pair<int,int> pieceInfo{queueArray[queueHead.first].shapeNumber, queueArray[queueHead.first].orientationNumber};
                queueHead.first = (queueHead.first + 1) % queueSize;
                queueLength--;
                return pieceInfo;
            }

            return std::pair<int,int>{-1,-1};
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
            return pieceInfo;
        }

        // Return Methods
        pieceInformation *array()
        {
            return queueArray;
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
                std::cout << queueArray[i].shapeNumber << " " << queueArray[i].orientationNumber << std::endl;
            }
        }

        void printHead()
        {
            std::cout << queueHead.first << " " << queueHead.second << std::endl;
        }

        void printTail()
        {
            std::cout << queueTail.first << " " << queueTail.second << std::endl;
        }

        void printLength()
        {
            std::cout << queueLength << std::endl;
        }

        void printSize()
        {
            std::cout << queueSize << std::endl;
        }

        
};
