/**
 * CMSC 341 - Spring 2021
 * Project 2 - Binary Trees
 * DiscriminatorTree.cpp
 * Implementation for the DTree class.
 */

#include "dtree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
DTree::~DTree() {
    clear();
}

/**
 * Overloaded assignment operator, makes a deep copy of a DTree.
 * @param rhs Source DTree to copy
 * @return Deep copy of rhs
 */
DTree& DTree::operator=(const DTree& rhs) {
    AssistCopy(_root, this->_root);
    // Returns a pointer to a DTree object
    return* this;
}

/**
 * Dynamically allocates a new DNode in the tree.
 * Should also update heights and detect imbalances in the traversal path
 * an insertion.
 * @param newAcct Account object to be contained within the new DNode
 * @return true if the account was inserted, false otherwise
 */
bool DTree::insert(Account newAcct) {
    // If statement to determine that a node of that discriminator doesn't exist
    if(_root == nullptr){
        // This code should run only for the creation of a new tree
        _root = new DNode(newAcct);
        return true;
    }else if(retrieve(newAcct._disc) == nullptr){
        // This function is recursive, and will navigate to the next open node

            // Evaluates the AssistInsert to determine whether a new node or old node was used
            AssistInsert(_root, newAcct);
        // Return True at function completion
        return true;
    }else{
        // Returns False if the account discriminator already exists
        return false;
    }
}

/**
 * Removes the specified DNode from the tree.
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool DTree::remove(int disc, DNode*& removed) {
    if(retrieve(disc) == nullptr){
        // Check for the existence of the node, and then delete.
        return false;
    }else{
        AssistRemove(_root, disc, removed);
        return true;
    }
}

/**
 * Retrieves the specified Account within a DNode.
 * @param disc discriminator int to search for
 * @return DNode with a matching discriminator, nullptr otherwise
 */
DNode* DTree::retrieve(int disc) {
    // Uses the retrieve helper to allow for the retrieve to be done recursively
    return AssistRetrieve(_root, disc);
}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void DTree::clear() {
    // Recursive Deletion of the tree starting at the _root
    AssistClear(_root);
    _root = nullptr;
}

/**
 * Prints all accounts' details within the DTree.
 */
void DTree::printAccounts() const {
    // Inorder Traversal Function
    AssistPrint(_root, 0);
}

/**
 * Dump the DTree in the '()' notation.
 */
void DTree::dump(DNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getAccount().getDiscriminator() << ":" << node->getSize() << ":" << node->getNumVacant();
    dump(node->_right);
    cout << ")";
}

/**
 * Returns the number of valid users in the tree.
 * @return number of non-vacant nodes
 */
int DTree::getNumUsers() const {
    // Return the number of non vacant nodes
    return _root->_size - _root->_numVacant;
}

/**
 * Updates the size of a node based on the immediate children's sizes
 * @param node DNode object in which the size will be updated
 */
void DTree::updateSize(DNode* node) {
    // Combines the value of Right and Left node _size for the new value
    if(node->_right == nullptr){
        if(node->_left == nullptr){
            node->_size = DEFAULT_SIZE;
        }else{
            node->_size = node->_left->_size + DEFAULT_SIZE;
        }

    }else if(node->_left == nullptr){
        node->_size = node->_right->_size + DEFAULT_SIZE;

    }else{
        node->_size = node->_right->_size + node->_left->_size + DEFAULT_SIZE;
    }
}

/**
 * Updates the number of vacant nodes in a node's subtree based on the immediate children
 * @param node DNode object in which the number of vacant nodes in the subtree will be updated
 */
void DTree::updateNumVacant(DNode* node) {
    if(node->_right == nullptr && node->_left == nullptr){
        if(node->isVacant()){
            node->_numVacant = 1;
        }else{
            node->_numVacant = 0;
        }
    }
    else if(node->_right == nullptr){
        // Handles only Left when Right doesn't exist
        node->_numVacant = node->_left->_numVacant;
    }else if(node->_left == nullptr){
        // Handles only Right when Left doesn't exist
        node->_numVacant = node->_right->_numVacant;
    }else{
        // Handles the existence of a left and right node
        node->_numVacant = node->_left->_numVacant + node->_right->_numVacant;
    }
}

/**
 * Checks for an imbalance, defined by 'Discord' rules, at the specified node.
 * @param checkImbalance DNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
bool DTree::checkImbalance(DNode* node) {
    int leftVal = 1;
    int rightVal = 1;

    if(node->_left != nullptr){
        leftVal = node->_left->_size;
    }
    if(node->_right != nullptr){
        rightVal = node->_right->_size;
    }

    if(rightVal > leftVal){
        // If the right value is more than 50% greater than the left value
        if(rightVal > (leftVal * 2)-leftVal){
            // Right Imbalance Exists
            return true;
        }else{
            // No Imbalance Exists
            return false;
        }
    }else if(leftVal > rightVal){
        // If the left value is more than 50% greater than the right value
        if(leftVal > (rightVal * 2)-rightVal){
            // Left Imbalance Exists
            return true;
        }else{
            // No Imbalance Exists
            return false;
        }
    }else{
        // Left and Right are the same size
        return false;
    }
}

//----------------
/**
 * Begins and manages the rebalancing process for a 'Discrd' tree (pass by reference).
 * @param node DNode root of the subtree to balance
 */
//void DTree::rebalance(DNode*& node) {

//}

// -- OR --

/**
 * Begins and manages the rebalancing process for a 'Discord' tree (returns a pointer).
 * @param node DNode root of the subtree to balance
 * @return DNode root of the balanced subtree
 */
DNode* DTree::rebalance(DNode* node) {
    DNode* tempRoot = _root; // A Node to act as the new _root
    if(!checkImbalance(node)){
        int ArrayMid; // Value to hold
        //int ArraySize = node->_size; // This holds the potential size of the array
        int ArraySize = node->_size;

        // Creates an account array to
        Account* AccArray;
        AccArray = new Account [ArraySize];

        // Moves the members of the tree to an array of accounts to assist in sorting
        AssistRebalance(node, 0, AccArray);

        // Sorts the Array of accounts based on the discriminator
        AssistSort(AccArray,0,ArraySize-1);

        // Determine the middle value of the
        if(ArraySize % 2 == 1){
            ArrayMid = (ArraySize - 1)/ 2;
        }else{
            ArrayMid = ArraySize / 2;
        }

        // Clear existing tree after creation of the new tree
        clear();

        // Sets the root to be
        insert(AccArray[ArrayMid]);
        tempRoot = _root;

        // First Half of the insert
        Recreate(AccArray, tempRoot, 0, ArrayMid-1);

        // Second Half of the insert
        Recreate(AccArray, tempRoot, ArrayMid+1, ArraySize-1);

        // Deletes the array of accounts
        delete[] AccArray;
    }
    return tempRoot;
}
//----------------

/**
 * Overloaded << operator for an Account to print out the account details
 * @param sout ostream object
 * @param acct Account objec to print
 * @return ostream object containing stream of account details
 */
ostream& operator<<(ostream& sout, const Account& acct) {
    sout << "Account name: " << acct.getUsername() <<
         "\n\tDiscriminator: " << acct.getDiscriminator() <<
         "\n\tNitro: " << acct.hasNitro() <<
         "\n\tBadge: " << acct.getBadge() <<
         "\n\tStatus: " << acct.getStatus();
    return sout;
}

// Helper Functions

/**
 * A Function to assist with the insert, it allows recursive traversal
 * @param a pointer to a DNode, used to navigate the tree
 * @param takes the account to be inserted through newAcct
 */
bool DTree::AssistInsert(DNode *node, Account newAcct) {
    bool Insert; // Used to handle the exit recursion for the tree
    if(node->isVacant()){
        // <- INSERT FILL HERE
        node->_account = newAcct;
        node->_vacant = false;
        Insert = true;
    }else{
    // HANDLES RIGHT NAVIGATION
        if(node->_account._disc < newAcct._disc){
            // Statement for right navigation
            if(node->_right == nullptr){
                // Insert of a new node
                node->_right = new DNode(newAcct);
                node->_right->_size = 1;
                Insert = false;
                // HANDLE OTHER
            }else{
                Insert = AssistInsert(node->_right, newAcct);
            }
    // HANDLES LEFT NAVIGATION
        }else if(node->_account._disc > newAcct._disc){
            // Statement for left navigation
            if(node->_left == nullptr){
                // Insert of a new node
                node->_left = new DNode(newAcct);
                Insert = false;
                // HANDLE OTHER
            }else{
                // Moving to next node
                Insert = AssistInsert(node->_left, newAcct);
            }
        }
    }
    updateSize(node);
    updateNumVacant(node);
    if(checkImbalance(node)){
        rebalance(node);
    }

    return Insert; // TEMPORARY STATEMENT
}

/**
 * A Function to recursively navigate the tree to find the node we need to delete and return it
 * @param node a pointer to the starting node, will almost always be _root
 * @param disc used to hold the discriminator of the node to be removed
 * @param removed the node to be removed, will be used to store the node.
 */
void DTree::AssistRemove(DNode *node, int disc, DNode*& removed) {
    // Removes to the Right
    if(node->_account._disc < disc){
        AssistRemove(node->_right, disc, removed);
    // Removes to the Left
    }else if(node->_account._disc > disc){
        AssistRemove(node->_left, disc, removed);
    // Removes from Here
    }else if(node->_account._disc == disc){
        removed = node;
        node->_vacant = true;
        node->_numVacant++;
    }
    updateNumVacant(node);
}
/**
 * A Function that assists in the printing and navigation of the DTree
 * This is done through inorder traversal of the discord tree
 * @param node is used to navigate the tree recursively, passing the next node
 * @param height Track
 */
void DTree::AssistPrint(DNode *node, int height) const {

    // Print Left Subtree first
    if(node->_left != nullptr){
        // Recursive Implementation
        AssistPrint(node->_left, height+1);
    }

    // Print Root Node Second
    if(!node->isVacant()){
        cout << "(HEIGHT: " << height << " )" <<endl;
        cout << node->_account << endl;
    }else{
        cout << endl << "Vacant Node" << endl << endl;
    }

    // Print Right Subtree last
    if(node->_right != nullptr){
        // Recursive Implementation
        AssistPrint(node->_right, height+1);
    }

}

/**
 * This function assists in the creation of a deep copy of the DTree, as it iterates through the O_nodes to create a new
 * tree from them.
 * @param O_node The original node, representing the copied tree
 * @param N_node The New Deep Copy Node, representing the new tree being created for the deep copy
 */
void DTree::AssistCopy(DNode* O_node, DNode* N_node){
    // Creates a deep copy of the node
    N_node = new DNode(N_node->_account);
    N_node->_vacant = O_node->_vacant;
    N_node->_size = O_node->_size;

    N_node->_left = nullptr;
    N_node->_right = nullptr;

    // Navigate To the Right, fills Right First
    if(O_node->_right != nullptr){
        AssistCopy(O_node->_right, N_node->_right);
    }

    // Navigate to the Left, fills Left Second
    if(O_node->_left != nullptr){
        AssistCopy(O_node->_left, N_node->_left);
    }

}

/**
 * A Function to assist in the rebalance of the DTree. This function will pull from the DTree and place nodes into the
 * array, which is passed by pointer.
 * @param node The starting point for the Rebalance, it will never pass the initial node value for the rebalance
 * @param pos postion in the array being filled by the
 * @param TempArray An array to pass the nodes into to be sorted and rebalanced into the tree
 * @return The integer inserted at which an account was inserted, used to assist recursion
 */
int DTree::AssistRebalance(DNode* node, int pos, Account TempArray[]){
    // Handles Self First as the Array will need to be sorted anyway
    if(!node->isVacant()){
        TempArray[pos] = node->_account;
    }

    // Handles Right
    if(node->_right != nullptr){
        pos = AssistRebalance(node->_right, pos+1, TempArray);
    }

    // Handles Left
    if(node->_left != nullptr){
        pos = AssistRebalance(node->_left, pos+1, TempArray);
    }
    return pos;
}

/**
 * Quicksort algorithm for an array of Account objects, the array will be sorted based on the size of the discriminator
 * @param TempArray the array to be sorted and used to recreate the DTree
 * @param large holds the position of the largest discriminator in the array
 * @param small holds the position of the smallest discriminator in the array
 */
void DTree::AssistSort(Account TempArray[], int small, int large) {
    if(small < large){
        // Creates an index for partition
        int partition = divide(TempArray, small, large);

        AssistSort(TempArray, small, partition-1);
        AssistSort(TempArray, partition+1, large);
    }
}

/**
 * A recursive function to implement quicksort and perform the partition
 * @param TempArray the array containing all of the Accounts
 * @param small the smallest value in the partition
 * @param large the largest value in the partition
 */
int DTree::divide(Account TempArray[], int small, int large){
    int p_point =  TempArray[large]._disc;
    int x = (small-1);

    for(int i = small; i <= large - 1; i++){
        if(TempArray[i]._disc <= p_point){
            x++;
            swap(&TempArray[i], &TempArray[x]);
        }
    }
    swap(&TempArray[x+1], &TempArray[large]);
    return(x+1);
}

/**
 * A function to swap two account objects
 * @param first is a pointer to an account and will be used to swap the account
 * @param second is a pointer to the second, will be swapped with the first
 */
void DTree::swap(Account* first, Account* second){
    Account temp = *first;
    *first = *second;
    *second = temp;
}

/**
 * This function is called to recreate the DTree from an Array of Accounts, it assumes that the tree doesn't exist.
 * The function operates under the assumption that the _root of the new tree has already been established, and that the
 * root will be passed as node at the start of recursion.
 * @param AccArr an array of account objects to be inserted into the new tree
 * @param node the root node to be passed to the
 * @param start
 * @param end
 */
void DTree::Recreate(Account AccArr[], DNode* node, int start, int end){
    // Insert the node in it's place int the Tree
    insert(AccArr[start]);
    // Recursion for the function
    if(start != end){
        Recreate(AccArr, node, start+1, end);
    }

}

/**
 * Allows for the DTree to be cleared recursively. Operate regardless of vacancy
 * @param node used in recursion deleted after all children
 */
void DTree::AssistClear(DNode* node){
    // Clear Left First
    if(node->_left != nullptr){
        AssistClear(node->_left);
    }
    // Clear Right Second
    if(node->_right != nullptr){
        AssistClear(node->_right);
    }
    // Clear Self Last
    delete node;
}

/**
 * Helper function for the retrieve function, should operate recursively
 * @param node used to allow recursion, should start at _root
 * @param disc the discriminator used to check for the existence of a node with the same discriminator
 * @return A pointer to the node being retrieved
 */
DNode* DTree::AssistRetrieve(DNode* node, int disc){
    // Halt recursion when a match is found
    if(node->getDiscriminator() == disc){
        return node;
    }
    // Checks for left existence
    if(node->_left != nullptr){
        // Checks for appropriate discriminators
        if(node->getAccount().getDiscriminator() > disc ) {
            // Recursive to the right
            return AssistRetrieve(node->_left, disc);
        }
    }
    // Checks for right existence
    if(node->_right != nullptr){
        // Checks for appropriate discriminators
        if(node->getAccount().getDiscriminator() < disc ){
            // Recursive to the right
            return AssistRetrieve(node->_right, disc);
        }
    }
    // Base case to return nullptr
    return nullptr;
}