#include <iostream>
#include "dtree.h"

using namespace std;

int main() {
    DTree *NewTree;
    NewTree = new DTree();

    Account TestAcct1("A", 2000, false, "RAT", "NO NITRO NO PEDO");
    Account TestAcct2("B", 3000, false, "BAT", "NO NITRO");
    Account TestAcct3("C", 1000, false, "CAT", "NO PEDO");
    Account TestAcct4("D", 2500, true, "EWW", "WEIRDO ALERT");
    Account TestAcct5("E", 1500, true, "PEW", "NOT A GAMER");
    //Account TestAcct6("REPLACE", 1900, false, "REP", "GAMER");

    NewTree->insert(TestAcct1);
    NewTree->insert(TestAcct2);
    NewTree->insert(TestAcct3);
    NewTree->insert(TestAcct4);
    NewTree->insert(TestAcct5);

    DNode* REMNODE;

    cout << endl << "Testing Creation of the DTREE, and Insert Process " << endl << endl;

    NewTree->printAccounts();
    cout << "ACCOUNTS" << endl;

    //cout << endl << "TESTING REBALANCE!" << endl;
    //NewTree->rebalance(NewTree->retrieve(2000)); // There is an issue here somewhere :(

    //NewTree->printAccounts();

    NewTree->remove(1500, REMNODE); // REMOVE IS WORKING

    //cout << endl << "Vacating an Account to test the Remove Function" << endl << endl;

    //NewTree->printAccounts();

    //cout << endl << "Filling Void with New Inserted Account" << endl << endl;

    //NewTree->insert(TestAcct6); // REPLACING EMPTY NODES WORKS AS LONG AS THE DISC DOESN'T MATCH

    cout << endl << endl;
    NewTree->printAccounts();

    delete NewTree;
    return 0;

    /*
     * Tracking the sources of Errors in testing:
     * Error when inserting a node of a matching discriminator
     * Failure to implement a rebalance correctly
     * Calculate the Size upon recursion exit for Insert and Remove
     */
}

