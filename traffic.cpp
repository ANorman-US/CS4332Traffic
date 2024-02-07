//Alexander Norman 
//CS4332-traffic problem
//solve by implementing BFS and keeping track of previous edges taken
//Notes: Clean up this code later if I have time
#include <iostream>
#include <list>
#include <vector>
//#include <fstream>
#include <set>
#include <queue>

using namespace std;

struct Node{
    int num;
    //vector<int> nodePath;//keeps track of path (not necessary for desired output. have tested it and it works everytime though)
    set<int> nodeSet;//keeps track of which nodes have been reached already
    vector<set<int>> edgePair;//keeps track of edges taken
};

vector<set<int>> BFS(const vector<list<int>>&, const int&, const int&);
bool containsAll(const Node&, const int&);

int main()
{
    //ifstream inFile("input.txt");//read from file
    int n, m;//n is # intersections, m is # streets
    //inFile >> n >> m;//read first line
    cout << "Input:\n";
    cin >> n >> m;
    vector<list<int>> aList;//adjacency matrix
    vector<vector<int>> bVector;//holds all bidirectional streets for future removal
    aList.resize(n+1);//size is n. we ignore index 0 since we started numbering nodes from 1.
    for(int i=0;i<m;i++)//adding to adjacency matrix. has all possible actions
    {
        int a,b,c;
        //inFile >> a >> b >> c;
        cin >> a >> b >> c;
        aList[a].push_back(b);
        if(c == 2)//if bidirectional add both to adjacency matrix but also add to special case vector
        {
            aList[b].push_back(a);
            bVector.push_back({a,b,c});
        }
    }
    //inFile.close();
    
    int startingNum = 4;//set starting num to any number between 1 and n
    vector<set<int>> numPairs = BFS(aList, n, startingNum);//BFS will return all edges on path(fromNode, toNode)

    //test for all pairs
    /*output is 
    0:
    1: 2 3
    2: 4
    3: 1
    4: 1*/
    /*for(int i=0;i<numPairs.size();i++)
    {
        cout << i << ": ";
        for(const auto&a : numPairs[i])
            cout << a << " ";
        cout << endl;
    }
    cout << endl;
    */

    //ofstream outFile("output.txt");
    //now we check to remove unnecessary bidirectional streets
    cout << endl << "Output:\n";
    for(int i=0;i<bVector.size();i++)
    {
        int a = bVector[i][0];
        int b = bVector[i][1];
        int c = bVector[i][2];
        if(!numPairs[a].count(b))//i should clean this all up later to be more efficient
        {
            a = bVector[i][1];
            b = bVector[i][0];
            c=1;
        }
        else if (!numPairs[b].count(a))
        {
            c=1;
        }
        cout << a << " " << b << " " << c << endl;
        //outFile << a << " " << b << " " << c << endl;
    }
    //outFile.close();

    return 0;
}

vector<set<int>> BFS(const vector<list<int>>& aList, const int &n, const int &goalNum)
{
    //we have adjacency matrix, now we perform BFS
    //goal state is a node with same num as starting node, but with a nodeContains having 1 through n
    queue <Node> frontier;//holds frontier
    Node nodeStart;
    nodeStart.num=goalNum;//initial node, set num to 0 < num <= n
    //nodeStart.nodePath.push_back(goalNum);
    nodeStart.nodeSet.insert(goalNum);
    nodeStart.edgePair.resize(n+1);
    frontier.push(nodeStart);//starting up queue

    while(!frontier.empty())
    {
        Node currentNode = frontier.front();
        frontier.pop();
        for (const auto& edge : aList[currentNode.num])//consults adjacency matrix to generate children
        {
            Node newNode = currentNode;
            newNode.num=edge;
            newNode.nodeSet.insert(edge);
            //newNode.nodePath.push_back(edge);
            newNode.edgePair[currentNode.num].insert(edge);
            if(newNode.num==goalNum)//if node = startNum = goalNum
            {
                if(containsAll(newNode, n))//checks if we have already visited all other node (checks the set)
                    return newNode.edgePair;
            }
            frontier.push(newNode);
        }
    }
    exit(1);
}

bool containsAll(const Node& currentNode, const int& n)
{
        for(int i=1;i<=n;i++)//from 1 to n
        {
            if(!currentNode.nodeSet.count(i))//checks set
                return false;
        }
        return true;
    
}