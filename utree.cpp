/**
 * CMSC 341 - Spring 2021
 * Project 2 - Binary Trees
 * UserTree.h
 * Implementation for the UTree class.
 */

#include "utree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
UTree::~UTree() {
    clear();
}

/**
 * Sources a .csv file to populate Account objects and insert them into the UTree.
 * @param infile path to .csv file containing database of accounts
 * @param append true to append to an existing tree structure or false to clear before importing
 */
void UTree::loadData(string infile, bool append) {
    std::ifstream instream(infile);
    string line;
    char delim = ',';
    const int numFields = 5;
    string fields[numFields];

    /* Check to make sure the file was opened */
    if(!instream.is_open()) {
        std::cerr << __FUNCTION__ << ": File " << infile << " could not be opened or located" << endl;
        exit(-1);
    }

    /* Should we append or clear? */
    if(!append) this->clear();

    /* Read in the data from the .csv file and insert into the UTree */
    while(std::getline(instream, line)) {
        std::stringstream buffer(line);

        /* Quick check to make sure each line is formatted correctly */
        int delimCount = 0;
        for(unsigned int c = 0; c < buffer.str().length(); c++) if(buffer.str()[c] == delim) delimCount++;
        if(delimCount != numFields - 1) {
            throw std::invalid_argument("Malformed input file detected - ensure each line contains 5 fields deliminated by a ','");
        }

        /* Populate the account attributes -
         * Each line always has 5 sections of data */
        for(int i = 0; i < numFields; i++) {
            std::getline(buffer, line, delim);
            fields[i] = line;
        }
        Account newAcct = Account(fields[0], std::stoi(fields[1]), std::stoi(fields[2]), fields[3], fields[4]);
        this->insert(newAcct);
    }
}

/**
 * Dynamically allocates a new UNode in the tree and passes insertion into DTree.
 * Should also update heights and detect imbalances in the traversal path after
 * an insertion.
 * @param newAcct Account object to be inserted into the corresponding DTree
 * @return true if the account was inserted, false otherwise
 */
bool UTree::insert(Account newAcct) {
    if(_root == nullptr){ // Handle First Node
        // Create a dynamic root node
        cout << endl <<"<---" << newAcct.getUsername() << endl;
        _root = new UNode();
        // Insert the Account into the new root node
        _root->getDTree()->insert(newAcct);
        return true;

    }else{ // Handle All Future Nodes
        // Calls recursive assist insert
        if(retrieve(newAcct.getUsername()) == nullptr){
            // If the username does not already exist, a node for it must be created
            return AssistInsert(_root, newAcct);
        }else{
            // Inserts the account directly at the tree of this username
            retrieve(newAcct.getUsername())->_dtree->insert(newAcct);
            rebalance(_root);
            return true;
        }
    }
}

/**
 * Removes a user with a matching username and discriminator.
 * @param username username to match
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool UTree::removeUser(string username, int disc, DNode*& removed) {
    return AssistRemove(_root, username, disc, removed);
}

/**
 * Retrieves a set of users within a UNode.
 * @param username username to match
 * @return UNode with a matching username, nullptr otherwise
 */
UNode* UTree::retrieve(string username) {
    return AssistRetrieve(_root, username);
}

/**
 * Retrieves the specified Account within a DNode.
 * @param username username to match
 * @param disc discriminator to match
 * @return DNode with a matching username and discriminator, nullptr otherwise
 */
DNode* UTree::retrieveUser(string username, int disc) {
    return retrieve(username)->_dtree->retrieve(disc);
}

/**
 * Returns the number of users with a specific username.
 * @param username username to match
 * @return number of users with the specified username
 */
int UTree::numUsers(string username) {
    // Retrieve node and return the number of users for the tree
    return retrieve(username)->_dtree->getNumUsers();
}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void UTree::clear() {
    AssistClear(_root);
}

/**
 * Prints all accounts' details within every DTree.
 */
void UTree::printUsers() const {
    AssistPrint(_root);
}

/**
 * Dumps the UTree in the '()' notation.
 */
void UTree::dump(UNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getUsername() << ":" << node->getHeight() << ":" << node->getDTree()->getNumUsers();
    dump(node->_right);
    cout << ")";
}

/**
 * Updates the height of the specified node.
 * @param node UNode object in which the height will be updated
 */
void UTree::updateHeight(UNode* node) {
    int left_height = 0;
    int right_height = 0;

    // Gather the height of the left node
    if(node->_left != nullptr){
        left_height = node->_left->getHeight();
    }

    // Gather the height of the right node
    if(node->_right != nullptr){
        right_height = node->_right->getHeight();
    }

    if(node->_right == nullptr && node->_left == nullptr){
        node->_height = DEFAULT_HEIGHT;
    }else{
        // Add both the right and the left node
        node->_height = right_height + left_height + 1;
    }

}

/**
 * Checks for an imbalance, defined by AVL rules, at the specified node.
 * @param node UNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occurred, false otherwise
 */
bool UTree::checkImbalance(UNode* node) {
    int right = 0;
    int left = 0;
    // Case for left hand
    if(node->_right != nullptr){
        right = node->_right->getHeight();
    }

    // Case for right hand
    if(node->_left != nullptr){
        left = node->_left->getHeight();
    }

    // Determine from left and right if the node is balanced
    if(right > 1+left || left > 1+right){
        return true;
    }else{
        return false;
    }
}

//----------------
/**
 * Begins and manages the rebalance procedure for an AVL tree (pass by reference).
 * @param node UNode object where an imbalance occurred
 */
void UTree::rebalance(UNode*& node) {
    AssistRebalance(node);

    /*
    // Determine source of imbalance
    if(node->_right == nullptr || node->_left > node->_right){
        // Left is too large
        cout << "LHS ROTATION NEEDED -------------" << endl;
        node = LeftRotation(node);
        // Somewhere the balanced subtree is not being placed back into the main tree
    }else if(node->_left == nullptr || node->_right > node->_left){
        // Right is too large
        cout << "RHS ROTATION NEEDED -------------" << endl;
        node = RightRotation(node);
        // Somewhere the balanced subtree is not being placed back into the main tree
    }else{
        cout << "Unknown Rebalance Issue" << endl;
    }
     */
}

/**
 * Begins and manages the rebalance procedure for an AVL tree (returns a pointer).
 * @param node UNode object where an imbalance occurred
 * @return UNode object replacing the unbalanced node's position in the tree
 */
//UTree* UTree::rebalance(UNode* node) {

//}
//----------------

// ---------- Private Helper Functions ----------

/**
 * A recursive function to assist in the insertion of an account into a DTree in the UTree, will need to check for
 * the username before adding creation of a new UNode
 * @param node Root node to start, and assist in Recursion, will change at different recursive levels
 * @param newAcct Used to pass the new account to the DTree
 * @return returns a true if the function is successful, otherwise will return false
 */
bool UTree::AssistInsert(UNode* node, Account newAcct){
    bool InsValue;
    if(newAcct.getUsername() == node->getUsername()){
        cout << "eee" << endl;
        node->_dtree->insert(newAcct);
    }else{
        // Navigate Right
        if(newAcct.getUsername() > node->getUsername())
        {
            // Check for NULLPTR
            if(node->_right == nullptr){
                cout << "--" << newAcct.getUsername() << endl;
                node->_right = new UNode;
                node->_right->_dtree->insert(newAcct);
                InsValue = true;
            }else{
                // Continue recursion
                InsValue =AssistInsert(node->_right, newAcct);
            }
        }
        // Navigate Left
        else if(newAcct.getUsername() < node->getUsername())
        {
            // Check for NULLPTR
            if(node->_left == nullptr){
                cout << "--" << newAcct.getUsername() << endl;
                node->_left = new UNode;
                node->_left->_dtree->insert(newAcct);
                InsValue = true;
            }else{
                // Continue recursion
                InsValue = AssistInsert(node->_left, newAcct);
            }
        }
        else{
            InsValue = false;
        }
    }
    // Exit Recursion Operations

    updateHeight(node);
    if(checkImbalance(node)){
        rebalance(node);
    }
    return InsValue;
}

/**
 * Function to handle the recursive removal of an account from a DTree in the UTree
 * @param node Root for the start of recursion
 * @param disc Discriminator value of the desired node
 * @param removed pointer to the node being removed in the process
 * @return bool value, true if the node is removed, false otherwise
 */
bool UTree::AssistRemove(UNode* node,string username, int disc, DNode*& removed){
    UNode* ToRemove = retrieve(username);
    if(ToRemove != nullptr){
        ToRemove->getDTree()->remove(disc, removed);
        return true;
    }else{
        return false;
    }
}

/**
 * Performs the Left rotation of a AVL tree subtree, will be used in a Rebalance function
 * @param node pointer to the root of the subtree to be rotated left
 */
UNode* UTree::LeftRotation(UNode* node){
    cout << endl << "L1=";
    // May need handle for NullPTR
    UNode* Left_temp = node->_left;
    //UNode* Right_temp = node->_right;
    UNode* Temp = node;

    Temp->_left = Left_temp->_right;
    cout << "L2=";
    Left_temp->_right = Temp;

    cout << "L3" << endl;
    //dump(Left_temp);
    //cout << endl;
    return Left_temp;
    // Needs to determine new heights
}

/**
 * Performs the Right rotation for an AVL subtree, will be used by the rebalance function
 * @param node pointer to the root of a subtree, which will be rotated to the right
 */
UNode* UTree::RightRotation(UNode* node){
    /*
     * Somewhere here and in the Left Rotation I am loosing nodes
     * I will need to evaluate this again
    */
    cout << endl <<  "R1-";
    UNode* Right_temp = node->_right;
    //UNode* Left_temp = node->_left;
    UNode* Temp = node;

    Temp->_right = Right_temp->_left;
    cout << "R2-";
    Right_temp->_left = node;

    cout << "R3" << endl;
    //dump(Right_temp);
    //cout << endl;
    return Right_temp;
    // Need to determine new heights

}

/**
 * Assists in the retrieval of the UNode with the username passed to the function
 * @param node the starting node / recursive starting node for the traversal
 * @param username this username will be found within the tree
 * @return returns recursively the pointer to the node
 */
UNode* UTree::AssistRetrieve(UNode* node, string username){
    if(username == node->getUsername()){
        return node;
    }else{
        if(username > node->getUsername()){
            // Handles Right Progression
            if(node->_right != nullptr){
                // Recursion to the right
                return AssistRetrieve(node->_right, username);
            }else{
                // If this Username doesn't exist
                return nullptr;
            }
        }else if(username < node->getUsername()){
            // Handles Left Progression
            if(node->_left != nullptr) {
                // Recursion to the left
                return AssistRetrieve(node->_left, username);
            }else{
                // If this Username doesn't exist
                return nullptr;
            }
        }
    }
    // Typically will return anything other than this, however this is present to catch outliers
    return nullptr;
}

/**
 * This is a shot in the dark, but this may work as a recursive rebalance
 * What it should do is iterate through the tree, and at any existing imbalance it will call the appropriate
 * rotation
 * @param node the node passed for use in recursion
 */
void UTree::AssistRebalance(UNode* node){
    int right = 0;
    int left = 0;
    if(node->_left != nullptr){
        AssistRebalance(node->_left);
        left = node->_left->getHeight();
    }
    if(node->_right != nullptr){
        AssistRebalance(node->_right);
        right = node->_right->getHeight();
    }

    if(checkImbalance(node)){
        //cout << "IMBALANCE PRESENT" << endl;
        // Imbalance will be repaired here
        if(right > left+2){
            //cout << "right rotation needed" << endl;
            // RightRotation(node);
        }else if(left > right+2){
            //cout << "left rotation needed" << endl;
            // LeftRotation(node);
        }
    }else{
        return;
    }
    updateHeight(node);
}

/**
 * Recursively clears the tree, will delete the node after it's children
 * @param node passed for recursion.
 */
void UTree::AssistClear(UNode* node){
    if(node->_right != nullptr){
        AssistClear(node->_right);
    }
    if(node->_left != nullptr){
        AssistClear(node->_left);
    }
    delete node;
}

/**
 * Print all nodes and the Accounts for their DTrees, this should be done using in-order traversal
 * @param node the Node used for recursion, and should start at the _root
 */
void UTree::AssistPrint(UNode* node) const{
    if(node->_left != nullptr){
        AssistPrint(node->_left);
    }

    cout << node->getUsername() << " : " << endl;
    node->getDTree()->printAccounts();

    if(node->_right != nullptr){
        AssistPrint(node->_right);
    }
}