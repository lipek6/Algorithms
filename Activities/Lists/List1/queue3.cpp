#include <stdexcept>
#include <optional>
#include <iostream>
#include "../../../AlgsLib/DataStructures/Includes/BinaryTree.h"
#include "../../../AlgsLib/DataStructures/Includes/SingleLinkedList.h"
#include "../../../AlgsLib/DataStructures/Includes/Queue.h"



template <typename T, typename E>
class TeamQueue
{
private:
    struct InternalTeamQueue
    {
        T team;
        Queue<E> queue;

        void push(const E& newData) { queue.push(newData); }
        void pop() { queue.pop(); }

        E& front() { return queue.front(); }
        E& back() { return queue.back(); }

        const E& front() const { return queue.front(); }
        const E& back() const { return queue.back(); }

        size_t size() const { return queue.size(); }
        bool empty() const { return queue.empty(); }

        // Will allow us to search a queue made of SubQueues
        bool operator==(const InternalTeamQueue& other) const { return this->team == other.team; }
        bool operator!=(const InternalTeamQueue& other) const { return this->team != other.team; }
    };


    SingleLinkedList<InternalTeamQueue> teamList;
    
    size_t numTeams;
    T* elementToTeam;
    bool* presentTeams;

public:
    teamQueue(size_t maxNumTeams, size_t maxNumElements) : numTeams(0)
    { 
        presentTeams = new bool[maxNumTeams]{false}; 
        elementToTeam = new T[maxNumElements];
    }
    ~teamQueue() { delete[] presentTeams; delete[] elementToTeam}


    void enqueue(const T& team, const E& element)
    {
        if (presentTeams[team] == true)
        {
            InternalTeamQueue
            teamList.find()
        }
    }

    pop()

    numTeams()

    numElements()

    front()


};


int main(void)
{
    int testCase = 1;
    int numTeams; std::cin >> numteams;

    while(numTeams > 0)
    {
        std::cout << "Scenario #" << testCase << "\n";
        //





        std::cout << "\n";
        std::cin >> numTeams;
        testCase++;
    }




}