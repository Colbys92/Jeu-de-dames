#ifndef TREE_H
#define TREE_H

#include <vector>
#include <queue>
#include <string>

/**
 * Node of a tree containing a type T information at each node.
 * @author Matthieu Moreau
 */
template <typename T> class Tree {
private :
    /**
     * Node information
     */
    T data;
    /**
     * Sequence of sons (empty if none)
     */
    std::vector<Tree*> sons;

    int maxDepthRec();
public:
    /**
     * Constructor. Create a node with given information.
     * @param d information on this node
     */
    Tree(T d){data=d;}
    /**
     * Copy constructor. Create a node with same information as given node.
     * @param t node to copy
     */
    Tree(const Tree<T>& t) {
        data=t.data;
        for(unsigned int i=0;i<t.sons.size();i++) {
            sons.push_back(new Tree<T>(*t.sons[i]));
        }
    }

    /**
     * Destructor. Destruct a node and all its descendants
     */
    ~Tree(){
        for(unsigned int i=0;i<sons.size();i++) {
            delete sons[i];
        }
    }
    /**
     * Return the information of this node.
     * @return data
     */
    T getData() {return data;}
    /**
     * Set the information of this node.
     * @param d information on this node
     */
    void setData(T d) {data=d;}
    /**
     * Return the number of sons of this node.
     * @return nbSons
     */
    int nbSons() {return sons.size();}
    /**
     * Return the son at position pos, if any.
     * @param pos position of the son (considering left-most son is at position 0)
     * @return son at position pos
     * @throws out_of_range if pos is not a valid position (between 0 and nbSons-1)
     */
    Tree<T>* getSon(int pos);
    /**
     * Replace the exisiting son at position pos with newSon (left-most son at position 0) , if pos is valid.
     * @param pos position of the newSon (considering left-most son is at position 0)
     * @param newSon new son to set
     * @throws out_of_range if pos is not a valid position (between 0 and nbSons-1)
     */
    void setSon(int pos,Tree<T>* newSon);
    /**
     * Add newSon as supplementary right-most son of this node
     * @param newSon new son to add.
     */
    void addAsLastSon(Tree<T>* newSon) {sons.push_back(newSon);}
    /**
     * Remove right-most son of this node.
     * @throws length_error if node has no son
     */
    void removeLastSon();
    /**
     * Displays the information of the nodes following depth-first search order (from the root to the leafs), printing prefix before each node and print printing indent for each level of the tree.
     * @param prefix string to print before each node
     * @param indent string to add to prefix every time you go a level deeper in the tree
     */
    void descendingDisplay(std::string prefix="",std::string indent=" ") const;
    /**
     * Displays the information of the nodes following depth-first search order(from the leafs to the root), printing prefix before each node and print printing indent for each level of the tree.
     * @param prefix string to print before each node
     * @param indent string to add to prefix every time you go a level deeper in the tree
     */
    void ascendingDisplay(std::string prefix="",std::string indent=" ") const;
    /**
     * Displays the information of the nodes following breadth first search order, printing prefix before each node and print printing indent for each level of the tree.
     * @param prefix string to print before each node
     * @param indent string to add to prefix every time you go a level deeper in the tree
     */
    void breadthDisplay(std::string prefix="",std::string indent=" ") const;
    /**
     * Add newSon as supplementary son at position pos of this node, if pos is valid
     * @param pos position to insert the son (considering left-most son is at position 0)
     * @param son new son to add.
     * @throws out_of_range if pos is not a valid position (between 0 and nbSons-1)
     */
    void addSon(int pos,Tree<T>* son);
    /**
     * Remove son at position pos, if any
     * @param pos position to insert the son (considering left-most son is at position 0)
     * @throws out_of_range if pos is not a valid position (between 0 and nbSons-1)
     */
    void removeSon(int pos);

    /**
     * Compute the max depth of a leaf in this tree
     * @return max_depth of the tree
     */
    int maxDepth() const;
    int minDepth() const;

};

template <typename T>
Tree<T>* Tree<T>::getSon(int pos) {
    return sons.at(pos);
}

template <typename T>
void Tree<T>::setSon(int pos,Tree<T>* newSon) {
    if(pos>sons.size()-1)
        throw std::out_of_range("Too few sons : pos >= sons.size()");
    if(pos<0)
        throw std::out_of_range("Position must be greater than 0.");
    sons.erase(sons.begin()+pos);
    sons.insert(sons.begin()+pos,newSon);
}

template <typename T>
void Tree<T>::removeLastSon() {
    if(sons.size()==0)
        throw std::length_error("No son to remove.");
    sons.pop_back();
}

template <typename T>
void Tree<T>::addSon(int pos,Tree<T>* son) {
    if(pos>=sons.size())
        throw std::out_of_range("Too few sons : pos >=sons.size()");
    sons.insert(sons.begin()+pos,son);
}

template <typename T>
void Tree<T>::removeSon(int pos) {
    if(pos>=sons.size())
        throw std::out_of_range("Too few sons : pos >=sons.size()");
    sons.erase(sons.begin()+pos);
}

template <typename T>
void Tree<T>::descendingDisplay(std::string prefix,std::string indent) const {
    std::cout<<prefix<<data<<std::endl;
    for(unsigned int i=0;i<sons.size();i++) {
        sons[i]->descendingDisplay(prefix+indent);
    }
}

template <typename T>
void Tree<T>::ascendingDisplay(std::string prefix,std::string indent) const {
    for(unsigned int i=0;i<sons.size();i++) {
        sons[i]->ascendingDisplay(prefix+indent);
    }
    std::cout<<prefix<<data<<std::endl;
}

template <typename T>
void Tree<T>::breadthDisplay(std::string prefix,std::string indent) const {
    std::queue<Tree<T>*> q;
    std::string currentPrefix=prefix; //Prefix to print before the current node's data.
    q.push(new Tree<T>(*this));
    int nodeToNextLevel=1; //Amount of nodes before we reach next level
    while(!q.empty()) {
        std::cout<<currentPrefix<<q.front()->data<<std::endl;
        for(unsigned int i=0;i<q.front()->sons.size();i++) {
            q.push(q.front()->sons[i]);
        }
        q.pop();
        nodeToNextLevel--;
        if(nodeToNextLevel==0){
            currentPrefix+=indent;
            nodeToNextLevel+=q.size();
        }
    }
}

template <typename T>
int Tree<T>::maxDepth() const{
    Tree<T> t(*this); //we make a copy of this tree since the maxDepthRec function deletes the tree when doing the maxDepth computation
    return t.maxDepthRec(); //we compute the maxDepth on t
}

template <typename T>
int Tree<T>::maxDepthRec() {
    if(sons.size()==0)
        return 1;
    else {
        int lengthLastSon=getSon(sons.size()-1)->maxDepth();
        removeLastSon();
        return( std::max(this->maxDepth(),1+lengthLastSon));
    }
}

template <typename T>
int Tree<T>::minDepth() const{
    std::queue<Tree<T>*> q;
    q.push(new Tree<T>(*this));
    int depth=1;
    int nodesToNextLevel=1;
    while(q.front()->nbSons()!=0) {
        for(unsigned int i=0;i<q.front()->sons.size();i++) {
            q.push(q.front()->sons[i]);
        }
        q.pop();
        nodesToNextLevel--;
        if(nodesToNextLevel==0) {
            depth++;
            nodesToNextLevel+=q.size();
        }
    }
    return depth;
}

#endif // TREE_H
