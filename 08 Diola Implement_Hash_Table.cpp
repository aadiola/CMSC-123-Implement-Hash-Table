/*****************************************************************************************
start of documentation:
                                    CODE DOCUMENTATION

REQUIREMENT #8
A C++ code demonstrating an implementation of a Hash Table in  fulfillment of the course 
Data Strctures.

Code Written By: Alex Diola, BSCS-II, UP-CEBU

Classes:
Student -- represents a student in the system. Contains appropriate student methods.
Node:Student -- represents a node in the system; a subclass of student. Contains Node methods.
HashTable -- represent a database as an array(chain) of simple singly-linked lists

Hashing Function:
    1) Use the student.familyName as key
    2) Convert all lowercase characters to uppercase
    3) Add the sum of the ASCII values of each character
    Ex. --  1) Student.familyName == "Diola"
        --  2) key == "DIOLA"
        --  3) DataID == (D)+(I)+(O)+(L)+(A) = 68 + 73 + 79 + 76 + 65  == 361
Hash Mapping Function:
    1) Uses a modulo operator
    2) Takes advantage of power-of-2 table size, making modulo operation very cheap
    Ex. 361 % 2^4 == 361 % 16 == 361 & (16-1) == 361 & 15 == 101101001 & 00001111 = 000001001 = 9 
Expand Function:
    1) Automated to increase in powers of 2
    2) Rehashes each data since the module power is now different
Update Data:
    1) Delete and reinserts Data, because updates in family name MAY REQUIRE rehashing

Program Flow:
Main:
    ->Insert Data
    ->Delete Data
    ->Search Data
    ->Update Data
    ->Print Table
    ->Print Map
    ->Print Data Table

Important Notes:

*Collisions are handled using chained linked lists.
*Datas are user inputted and data with the family name will result in the same key.
**To separate similar instances data, a unique secondary key is used.
**The student number is the unique secondary key. It depends on the program and not on
** user input unlike the family name which is a user input.
***Ex. "Diola, Alex, M, 20" and "Diola, Alex, M, 21" will have the same DataID.
*** But can still be separately indentified using their unique studentNumber.
****Mutual family names in the same school are not unique instances here in the Ph.

                                                                    :end of documentation
******************************************************************************************/
#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

class Student{

    private:
        int studentNumber;  //Secondary Key (since family names are common here in PH)
        string familyName;
        string firstName;
        int age;
        string gender;
        string middleName;
        string contactNumber;
        string address;

    public:
        Student(int studentNum, string _familyName, string _firstName, int _age, string _gender, string _middleName ="", string _contactNumber = "", string _address =""):
        studentNumber(studentNum), familyName(_familyName), firstName(_firstName), age(_age), gender(_gender), middleName(_middleName), contactNumber(_contactNumber), address(_address)
        {}
        string getFamilyName(void) const{
            return familyName;
        }
        string getFirstName(void) const{
            return firstName;
        }
        int getAge(void) const{
            return age;
        }
        string getGender(void) const{
            return gender;
        }
        string getMiddleName(void) const{
            return middleName;
        }
        string getContactNumber(void) const{
            return contactNumber;
        }
        string getAddress(void) const{
            return address;
        }
        int getStudentNumber(void) const{
            return studentNumber;
        }
        int setFamilyName(string newName){
            familyName = newName;
            return 0;
        }
        int setFirstName(string newName){
            firstName = newName;
            return 0;
        }
        int setMiddleName(string newName){
            middleName = newName;
            return 0;
        }
        int setContactNumber(string newNum){
            contactNumber = newNum;
            return 0;
        }
        int setAddress(string newAddress){
            address = newAddress;
            return 0;
        }

};

class Node: public Student{

    private:
        int dataID; //PrimaryKey (hashed family name)
        Node *nextNode = nullptr;

    public:
        Node(int dataId, int studNum, string _familyName, string _firstName, int _age, string _gender, string _middleName ="", string _contactNumber = "", string _address ="") : 
        Student(studNum, _familyName, _firstName,_age, _gender,  _middleName, _contactNumber, _address), dataID(dataId)
        {
        }
        Node *getNext(void) const{
            return nextNode;
        }
        int setNext(Node *next){
            nextNode = next;
            return 0;
        }
        int getDataID(void) const{
            return dataID;
        }

};

class HashTable{

    private:

        int numStudents = 0;
        int studentNumber = 0;
        int modPower = 4;
        int tableSize = pow(2,modPower);
        int collisionCounter = 0;
        Node **table;

        void printTableNoSkip(void) const{
            for(int i=0; i<tableSize; i++){
                cout << "Table[" << i << "]: ";
                if(table[i] == nullptr){
                    cout << "Empty" << endl;
                } else{
                    Node *currentNode = table[i];
                    do{
                        cout << "\nData ID:\t" << currentNode->getDataID() << endl;
                        cout << "Student Number:\t" << currentNode->getStudentNumber() << endl;
                        cout << "Family Name:\t" << currentNode->getFamilyName() << endl;
                        cout << "First Name:\t" << currentNode->getFirstName() << endl;
                        cout << "Middle Name:\t" << currentNode->getMiddleName() << endl;
                        cout << "Age:\t" << currentNode->getAge() << endl;
                        cout << "Gender:\t" << currentNode->getGender() << endl;
                        cout << "Contact Number:\t" << currentNode->getContactNumber() << endl;
                        cout << "Address:\t" << currentNode->getAddress() << endl; 
                        currentNode = currentNode->getNext();

                    }while(currentNode!=nullptr);
                }
            }
        }        
        Node *find(string key, int studentNum=-1){
            int dataID = hash(key);
            int position = hashMap(dataID);
            Node* currentNode = table[position];
            while(currentNode!= nullptr){
                if(currentNode->getDataID() == dataID){
                    if(studentNum != -1){
                        if(currentNode->getStudentNumber() == studentNum){
                            return currentNode;
                        } else {
                            currentNode = currentNode->getNext();
                            continue;
                        }
                    } else {
                        return currentNode;
                    }
                }
                currentNode = currentNode->getNext();
            }
            return currentNode;
        }
        Node *find(int subkey) const{   //Expensive
            Node *currentNode;
            for(int i=0; i<tableSize; i++){
                currentNode = table[i];
                if(currentNode == nullptr){
                    continue;
                }
                while(currentNode != nullptr){
                    if(currentNode->getStudentNumber() == subkey){
                        return currentNode;
                    }
                    currentNode = currentNode->getNext();
                }
            }
            return nullptr;
        }
        int forceDeleteData(string key, int subkey=-1){
            Node *toDelete = find(key, subkey);
            if(toDelete==nullptr){
                return -1;
            }
            int dataID = hash(key);
            int position = hashMap(dataID);
            Node* binNode = table[position];
            if(binNode == toDelete){
                    table[position] = toDelete->getNext();
            } else {
                while(binNode->getNext() != toDelete){
                    binNode = binNode->getNext();
                }
                binNode->setNext(toDelete->getNext());
                collisionCounter--;
            }
            delete toDelete;
            numStudents--;
            return 0;
        }
        int occurences(string key){
            int dataID = hash(key);
            int position = hashMap(dataID);
            int occurCount = 0;
            Node* currentNode = table[position];
            if(currentNode == nullptr){
                return 0;
            }
            while(currentNode!= nullptr){
                if(currentNode->getDataID() == dataID){
                    occurCount++;
                }
                currentNode = currentNode->getNext();
            }
            return occurCount;
        }
        int reinsertData(int studentNum, string familyName, string firstName, int age, string gender, string middleName="", string contactNumber="", string address=""){
            int dataID = hash(familyName);
            int position = hashMap(dataID);
            if(table[position] == nullptr){
                table[position] = new Node(dataID, studentNum, familyName, firstName, age, gender, middleName, contactNumber, address);
            } else{
                Node *currentNode = table[position];
                while(currentNode->getNext() != nullptr){
                    currentNode = currentNode->getNext();
                }
                Node *newNode = new Node(dataID, studentNum, familyName, firstName, age, gender, middleName, contactNumber, address);
                currentNode->setNext(newNode);
                collisionCounter++;
            }
            numStudents++;
            return position;
        }
        int expand(void){
            int oldSize = tableSize;
            numStudents = 0;
            modPower++;
            tableSize = pow(2, modPower);
            Node **oldTable = table;
            table = new Node*[tableSize]();
            for(int i=0; i<tableSize; i++){
                *(table+i) = nullptr;
            }
            Node *currentNode;
            for(int i=0; i<oldSize; i++){
                currentNode = oldTable[i];
                while(currentNode!=nullptr){
                    int studentNum = currentNode->getStudentNumber();
                    string familyName = currentNode->getFamilyName();
                    string firstName = currentNode->getFirstName();
                    int age = currentNode->getAge();
                    string gender = currentNode->getGender();
                    string middleName = currentNode->getMiddleName();
                    string contactNumber = currentNode->getContactNumber();
                    string address = currentNode->getAddress();
                    reinsertData(studentNum, familyName, firstName, age, gender, middleName, contactNumber, address);
                    currentNode = currentNode->getNext();
                }
            }
            delete [] oldTable;
            return 0;
        } 
        int hash(string key){
            int stringLength = key.length();
            int hash = 0;
            for(int i=0; i<stringLength; i++){
                if(key[i]>= 'a' && key[i]<= 'z') key[i] -=32;
            }
            for(int i=0; i<stringLength; i++){
                hash += key[i];
            }
            return hash;
        }
        int hashMap(int hashKey){
            int power = pow(2, modPower) - 1;
            int index = hashKey & power;
            return index;
        }
        int deleteData(string key, int subkey){
            Node *toDelete = find(key, subkey);
            if(toDelete==nullptr){
                cout << "Key invalid. Node not found." << endl;
                return -1;
            }
            cout << "Data to Delete: " << endl;
            cout << "\nData ID:\t" << toDelete->getDataID() << endl;
            cout << "Student Number:\t" << toDelete->getStudentNumber() << endl;
            cout << "Family Name:\t" << toDelete->getFamilyName() << endl;
            cout << "First Name:\t" << toDelete->getFirstName() << endl;
            cout << "Middle Name:\t" << toDelete->getMiddleName() << endl;
            cout << "Age:\t" << toDelete->getAge() << endl;
            cout << "Gender:\t" << toDelete->getGender() << endl;
            cout << "Contact Number:\t" << toDelete->getContactNumber() << endl;
            cout << "Address:\t" << toDelete->getAddress() << endl;
            char answer = 'a';
            while(answer!='Y' && answer!='N'){
                cout << "Confirm Data deletion? [Y\\N]:";
                cin >> answer;
            }
            if(answer == 'N'){
                cout << "Data not deleted." << endl;
                return -1;
            }
            int dataID = hash(key);
            int position = hashMap(dataID);
            Node* binNode = table[position];
            if(binNode == toDelete){
                    table[position] = toDelete->getNext();
            } else {
                while(binNode->getNext() != toDelete){
                    binNode = binNode->getNext();
                }
                binNode->setNext(toDelete->getNext());
                collisionCounter--;
            }
            delete toDelete;
            cout << "Data deleted." << endl;
            return 0;
        }
        int search(int dataID, int subkey){
            int position = hashMap(dataID);
            Node *currentNode = table[position];
            if(currentNode == nullptr){
                cout << "Bin[" << position <<"] is empty." << endl;
                return -1;
            }
            while(currentNode!=nullptr){
                if(currentNode->getDataID() == dataID){
                    if(subkey == -1){
                        break;
                    }
                    else if(subkey == currentNode->getStudentNumber()){
                        break;
                    }
                }
                currentNode = currentNode->getNext();
                if(currentNode==nullptr){
                    cout << "Data with ID " << dataID << " not found." << endl;
                    return -1;
                }
            }
            cout << "\nData ID:\t" << currentNode->getDataID() << endl;
            cout << "Student Number:\t" << currentNode->getStudentNumber() << endl;
            cout << "Family Name:\t" << currentNode->getFamilyName() << endl;
            cout << "First Name:\t" << currentNode->getFirstName() << endl;
            cout << "Middle Name:\t" << currentNode->getMiddleName() << endl;
            cout << "Age:\t\t" << currentNode->getAge() << endl;
            cout << "Gender:\t\t" << currentNode->getGender() << endl;
            cout << "Contact Number:\t" << currentNode->getContactNumber() << endl;
            cout << "Address:\t" << currentNode->getAddress() << endl;
            return 0;
        }

    public:

        HashTable(){
            table = new Node*[tableSize]();
            for(int i=0; i<tableSize; i++){
                *(table+i) = nullptr;
            }
        }      
        void printTable(void) const{
            if(numStudents == 0){
                cout << "Hash Table is empty.\n" << endl;
                return;
            }
            cout << "Empty bins are skipped.\n" << endl;
            cout << "------------------------------------" << endl;
            for(int i=0; i<tableSize; i++){
                if(table[i] == nullptr){
                    continue;
                } else{
                    cout << "Bin[" << i << "]: " << endl;
                    Node *currentNode = table[i];
                    do{
                        cout << "\nData ID:\t" << currentNode->getDataID() << endl;
                        cout << "Student Number:\t" << currentNode->getStudentNumber() << endl;
                        cout << "Family Name:\t" << currentNode->getFamilyName() << endl;
                        cout << "First Name:\t" << currentNode->getFirstName() << endl;
                        cout << "Middle Name:\t" << currentNode->getMiddleName() << endl;
                        cout << "Age:\t\t" << currentNode->getAge() << endl;
                        cout << "Gender:\t\t" << currentNode->getGender() << endl;
                        cout << "Contact Number:\t" << currentNode->getContactNumber() << endl;
                        cout << "Address:\t" << currentNode->getAddress() << endl; 
                        currentNode = currentNode->getNext();

                    }while(currentNode!=nullptr);
                    cout << "------------------------------------" << endl;
                }
                
            }
        }    
        int insertData(string familyName, string firstName, int age, string gender, string middleName="", string contactNumber="", string address=""){
            if(numStudents == tableSize){   //Does not necessarily mean all the bins are occupied
                char answer = 'a';          //->But may be a sign that the table is growing big
                while(answer!='Y' && answer!='N'){   //->and so th array map should also adapt. This may also lessen collisions.
                    cout << "Table is full, would you like to expand table? [Y\\N]:";
                    cin >> answer;
                }
                if(answer == 'N'){
                    cout << "Data not inserted." << endl;
                    return -1;
                }
            }
            int dataID = hash(familyName);
            int position = hashMap(dataID);
            if(table[position] == nullptr){
                table[position] = new Node(dataID, ++studentNumber, familyName, firstName, age, gender, middleName, contactNumber, address);
            } else{
                Node *currentNode = table[position];
                while(currentNode->getNext() != nullptr){
                    currentNode = currentNode->getNext();
                }
                Node *newNode = new Node(dataID, ++studentNumber, familyName, firstName, age, gender, middleName, contactNumber, address);
                currentNode->setNext(newNode);
                collisionCounter++;
            }
            cout << "\nData inserted at index: " << position << endl;
            if(collisionCounter!=0){
                cout << "Current Collision Count: " << collisionCounter << endl;
            }
            numStudents++;
            return position;
        }
        int search(string key){
            int occurTimes = occurences(key);
            int subkey = -1;
            if(occurTimes==0){
                cout << "Key invalid. Node not found." << endl;
                return -1;
            }
            if(occurTimes > 1){
                cout <<"Multiple entries found. Please enter student number: ";
                cin >> subkey;
                cin.ignore(1024,'\n');
            } 
            int dataID = hash(key);
            if(search(dataID, subkey) == -1){
                cout << "Key \"" << key << "\" ";
                if(subkey != -1){
                    cout << " and subkey \"" << subkey << "\" ";
                }
                cout <<  "does not exist in the table." << endl;
                return -1;
            }
            return 0;
        }    
        int updateData(string key){      
            int occurTimes = occurences(key);
            int subkey = -1;
            if(occurTimes==0){
                cout << "Key invalid. Node not found." << endl;
                return -1;
            }
            if(occurTimes > 1){
                cout <<"Multiple entries found. Please enter student number: ";
                cin >> subkey;
                cin.ignore(1024,'\n');
            } 
            Node *toUpdate = find(key, subkey);
            if(toUpdate==nullptr){
                cout << "Key ";
                if(occurTimes>1){
                    cout << " and subkey ";
                }
                cout << "invalid. Node not found." << endl;
                return -1;
            }
            string familyName, firstName, gender, middleName, contactNumber, address;
            int age;
            int studentNumber = toUpdate->getStudentNumber();
            cout << "Current Data: " << endl;
            cout << "\nData ID:\t" << toUpdate->getDataID() << endl;
            cout << "Student Number:\t" << toUpdate->getStudentNumber() << endl;
            cout << "Family Name:\t" << toUpdate->getFamilyName() << endl;
            cout << "First Name:\t" << toUpdate->getFirstName() << endl;
            cout << "Middle Name:\t" << toUpdate->getMiddleName() << endl;
            cout << "Age:\t\t" << toUpdate->getAge() << endl;
            cout << "Gender:\t\t" << toUpdate->getGender() << endl;
            cout << "Contact Number:\t" << toUpdate->getContactNumber() << endl;
            cout << "Address:\t" << toUpdate->getAddress() << endl;
            cout << "New Data: ";
            cout << "Student Number: " << toUpdate->getStudentNumber() << endl;
            cout << "New Family Name: ";
            getline(cin, familyName);
            cout << "New First Name: ";
            getline(cin, firstName);
            cout << "New Middle Name: ";
            getline(cin, middleName);
            cout << "New Age: ";
            cin >> age;
            cin.ignore(1024, '\n');
            cout << "New Gender: ";
            getline(cin, gender);
            cout << "New Contact Number: ";
            getline(cin, contactNumber);
            cout << "New Address: ";
            getline(cin, address);
            forceDeleteData(key, subkey);
            reinsertData(studentNumber, familyName, firstName, age, gender, middleName, contactNumber, address);
            numStudents--;
            return 0;
        }
        int deleteData(string key){
            int occurTimes = occurences(key);
            int subkey=-1;
            if(occurTimes == 0){
                cout << "Key invalid. Node not found." << endl;
                return -1;
            }
            if(occurTimes > 1){
                cout <<"Multiple entries found. Please enter student number: ";
                cin >> subkey;
                cin.ignore(1024,'\n');
            } 
            return deleteData(key, subkey);
;
        }    
        void printMap(void) const{
            for(int i=0; i<tableSize; i++){
                cout << "Bin[" << i << "]: ";
                if(table[i] == nullptr){
                    cout << "Empty" << endl;
                } else{
                    Node *currentNode = table[i];
                    do{
                        cout << currentNode->getFamilyName();
                        if(currentNode->getNext()!=nullptr){
                            cout << " --> ";
                        }
                        currentNode = currentNode->getNext();

                    }while(currentNode!=nullptr);
                    cout << endl;
                }
            }
        }
        void printDataTable(void) const{
            cout << "***PLEASE MAXIMIZE WINDOW***\n" << endl;
            for(int i=0; i<140; i++) cout << "-";
            cout << endl;
            cout << setw(76) << "START OF TABLE" << endl;
            for(int i=0; i<140; i++) cout << "-";
            cout << endl;
            cout << setw(10) << "Data ID|" << setw(20) << "Student Number|" << setw(20) << "Family Name|";
            cout << setw(15) << "First Name|" << setw(15) << "Middle Name|" << setw(10) << "Age|";
            cout << setw(10) << "Gender|" << setw(20) << "Contact Number|" << setw(20) << "Address|" << endl;
            for(int i=0; i<140; i++) cout << "-";
            Node* currentNode;
            for(int i=1; i<=studentNumber; i++){ //Ordering by Student Number is Expensive
                currentNode = find(i);           //May be replaced by linear probing of the table, but will result in unordered data
                if(currentNode == nullptr){
                    continue;
                }
                cout << endl;
                cout << setw(9) << currentNode->getDataID();
                cout << setw(20) << currentNode->getStudentNumber();
                cout << setw(20) << currentNode->getFamilyName();
                cout << setw(15) << currentNode->getFirstName();
                cout << setw(15) << currentNode->getMiddleName();
                cout << setw(10) << currentNode->getAge();
                cout << setw(10) << currentNode->getGender();
                cout << setw(20) << currentNode->getContactNumber();
                cout << setw(20) << currentNode->getAddress();
                currentNode = currentNode->getNext();
                cout << endl;
                for(int i=0; i<140; i++) cout << "-";
            }
            cout << endl;
            cout << setw(76) << "END OF TABLE" << endl;
            for(int i=0; i<140; i++) cout << "-";
        }
};


int main(void){

    HashTable *table = new HashTable();
    bool run=true;
    int option;

    table->insertData("Acha", "Alex", 21, "Male");
    table->insertData("Almario", "Alex", 21, "Male");
    table->insertData("Becite", "Alex", 21, "Male");
    table->insertData("Caballes", "Alex", 21, "Male");
    table->insertData("Caballes", "Alex", 21, "Male");
    table->insertData("Cali", "Alex", 21, "Male");
    table->insertData("Casquejo", "Alex", 21, "Male");
    table->insertData("Chico", "Alex", 21, "Male");
    table->insertData("Codoy", "Alex", 21, "Male");
    table->insertData("Craus", "Alex", 21, "Male");
    table->insertData("De Loyola", "Alex", 21, "Male");
    table->insertData("Delgado", "Alex", 21, "Male");
    table->insertData("Dico", "Alex", 21, "Male");
    table->insertData("Diola", "Alex", 21, "Male");
    table->insertData("Durante", "Alex", 21, "Male");
    table->insertData("Fortaleza", "Alex", 21, "Male");
    table->insertData("Gomez", "Alex", 21, "Male");
    table->insertData("Jabello", "Alex", 21, "Male");
    table->insertData("Jacobe", "Alex", 21, "Male");
    table->insertData("Julve", "Alex", 21, "Male");
    table->insertData("Lauron", "Alex", 21, "Male");
    table->insertData("Lodovice", "Alex", 21, "Male");
    table->insertData("Mata", "Alex", 21, "Male");
    table->insertData("Mondido", "Alex", 21, "Male");
    table->insertData("Torres", "Alex", 21, "Male");

    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    
    table->printTable();
    cout << "Continue...";
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    table->printMap();
    cout << "Continue...";
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    table->printDataTable();
    cout << "Continue...";
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    int age;
    string familyName="", firstName="", middleName="", gender="", contactNumber="", address="";

    cout << "\n\t\tRequirement 8: Hash Table (Chained Link List)" << endl;
    cout <<"Default Table Limit is: " << pow(2,4) << endl;
    do{
        cout << "\nPlease choose one of the following options:" <<endl;
        cout <<     "\t1 -- Add Data" << endl;
        cout <<     "\t2 -- Delete Data" << endl;
        cout <<     "\t3 -- Search Data" << endl;
        cout <<     "\t4 -- Update Data" << endl;
        cout <<     "\t5 -- Print Hash Table" << endl;
        cout <<     "\t6 -- Print Map" << endl;
        cout <<     "\t7 -- Print Data Table" << endl;
        cout <<     "\t0 -- Exit Program" << endl;
        cout << "OPTION SELECTED: ";
        cin >> option;
        cin.ignore(1024, '\n');     //Clears '\n' buffer
         switch (option){
             case 0:
                cout << "EXITING PROGRAM\n" << endl;
                run = false;
                delete table;
                break;
            case 1:
                cout << "\nOPTION SELECTED:\tADD DATA\n" <<endl;
                cout << "Enter Family Name: ";
                getline(cin, familyName);
                while(familyName.length() == 0){
                    cout << "Please enter a valid family name.\n";
                    cout << "Enter Family Name: ";
                    getline(cin, familyName);
                }
                cout << "Enter First Name: ";
                getline(cin, firstName);
                cout << "Enter Middle Name: ";
                getline(cin, middleName);
                cout << "Enter Age: ";
                cin >> age;
                cin.ignore(1024, '\n');
                cout << "Enter Gender: ";
                getline(cin, gender);
                cout << "Enter Contact Number: ";
                getline(cin, contactNumber);
                cout << "Enter Address: ";
                getline(cin, address);
                table->insertData(familyName, firstName, age, gender, middleName, contactNumber, address);
                break;
            case 2:
                cout << "\nOPTION SELECTED:\tDELETE DATA\n" <<endl;
                cout << "Enter Family Name of Data to Delete: ";
                getline(cin, familyName);
                table->deleteData(familyName);
                break;        
            case 3:
                cout << "\nOPTION SELECTED:\tSEARCH DATA\n" << endl;
                cout << "Enter Family Name of Data to Search: ";
                getline(cin, familyName);
                table->search(familyName);
                break;
            case 4:
                cout << "\nOPTION SELECTED:\tUPDATE DATA\n" << endl;
                cout << "Enter Family Name of Data to Update: ";
                getline(cin, familyName);
                table->updateData(familyName);
                break;
            case 5:
                cout << "\nOPTION SELECTED:\tPRINT HASH TABLE\n" << endl;
                table->printTable();
                break;
            case 6:
                cout << "\nOPTION SELECTED:\tPRINT MAP\n" << endl;
                table->printMap();              
                break;
            case 7:
                cout << "\nOPTION SELECTED:\tPRINT DATA TABLE\n" << endl;
                table->printDataTable();              
                break;
            default:
                cout << "\nInvalid option selected. Please select an option from menu given." << endl;
        }

    }while(run);

    return 0;
}