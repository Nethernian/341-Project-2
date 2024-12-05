#include "utree.h"
#include <random>

#define NUMACCTS 20
#define RANDDISC (distAcct(rng))

std::mt19937 rng(10);
std::uniform_int_distribution<> distAcct(0, 9999);

class Tester {
public:
    bool testBasicDTreeInsert(DTree& dtree);

    bool testBasicUTreeInsert(UTree& utree);

    bool testBasicDTreeRemove(DTree& dtree);

    bool testBasicDTreeRebalance(DTree& dTree);


};

// TESTERS FOR DTREE

bool Tester::testBasicDTreeInsert(DTree& dtree) {
    bool allInserted = true;
    for(int i = 0; i < NUMACCTS; i++) {
        int disc = RANDDISC;
        Account newAcct = Account("", disc, 0, "", "");
        if(!dtree.insert(newAcct)) {
            cout << "Insertion on node " << disc << " did not return true" << endl;
            allInserted = false;
        }
    }
    return allInserted;
}

bool Tester::testBasicDTreeRemove(DTree& dtree){
    DNode* temp;
    bool removed = true;
    Account newAcct = Account("!", 100, false, "", "");
    dtree.insert(newAcct);
    dtree.remove(100, temp);
    if(!dtree.retrieve(100)->isVacant()){
        removed = false;
    }
    return removed;
}

bool Tester::testBasicDTreeRebalance(DTree& dTree){
    int disc = 1;
    for(int i = 0; i < 10; i++){
        Account newAcct = Account("!!", disc, 0, "!", "!");
        disc += 4;
    }
    return true;
}



// TESTERS FOR UTREE

bool Tester::testBasicUTreeInsert(UTree& utree) {
    string dataFile = "accounts.csv";
    try {
        utree.loadData(dataFile);
    } catch(std::invalid_argument e) {
        std::cerr << e.what() << endl;
        return false;
    }
    return true;
}

int main() {
    Tester tester;

    /* Basic dtree tests */
    DTree dtree;

    cout << "Testing DTree insertion...";
    if(tester.testBasicDTreeInsert(dtree)) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    cout << "Resulting DTree:" << endl;
    dtree.dump();
    cout << endl << endl;

    cout << "Testing DTree removal...";
    if(tester.testBasicDTreeRemove(dtree)){
        cout << "test passed" << endl;
    }else{
        cout << "test failed" << endl;
    }
    cout << "Resulting DTree:" << endl;
    dtree.dump();
    cout << endl << endl;

    cout << "Testing DTree rebalance...";
    if(tester.testBasicDTreeRebalance(dtree)){
        cout << "test passed" << endl;
    }else{
        cout << "test failed" << endl;
    }
    dtree.dump();

    /* Basic UTree tests */
    UTree utree;

    cout << "\n\nTesting UTree insertion...";
    if(tester.testBasicUTreeInsert(utree)) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    cout << "Resulting UTree:" << endl;
    utree.dump();
    cout << endl;

    return 0;
}