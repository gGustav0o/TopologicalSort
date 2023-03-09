//------------------------INCLUDE------------------------//
#include <iostream>
//-----------------------NAMESPACE-----------------------//
using namespace std;
//------------------------DEFINE------------------------//
#define MEMORY_ERROR 2
#define FILE_ERROR 3
#define path R"(C:\Users\Kurai\Desktop\0\Proga\Lists3\relations.txt)"
//------------------------STRUCT------------------------//
struct bPointer;
struct object{
    char* name;
    bPointer* beforeList;
    int count;
    object* next;
};
struct bPointer{
    object* before;
    bPointer* next;
};
//-------------------ERROR--FUNCTIONS-------------------//
void memoryError(){
    cerr<<"Memory error\n";
    exit(MEMORY_ERROR);
}
void fileError(){
    std::cerr<<"File error\n";
    exit(FILE_ERROR);
}
//------------------------------------------------------//
void printList(object *root){
    while (root){
        printf("%s\n" ,root->name);
        root = root->next;
    }
}
bool compare(const char* a, const char* b){
    int i = 0;
    while (a[i] != '\0' & b[i] != '\0'){
        if (a[i] != b[i]) return false;
        i++;
    }
    if (a[i] == '\0' & b[i] != '\0' || a[i] != '\0' & b[i] == '\0') return
                false;
    return true;
}
char* get_string(int* len, FILE* thread) {
    *len = 0;
    int capacity = 1;
    char* s = (char*)malloc(sizeof(char));
    if (!s) {
        memoryError();
    }
    char c = (char)fgetc(thread);
    while ((c != '\n') & !feof(thread) & (c != '<')) {
        s[(*len)++] = c;
        if (*len >= capacity) {
            capacity *= 2;
            char* temp = (char*)realloc(s, capacity * sizeof(char));
            if (temp) {
                s = temp;
            }
            else {
                memoryError();
            }
        }
        c = (char)fgetc(thread);
    }
    s[*len] = '\0';
    return s;
}
bPointer* createBPointer(){
    bPointer* element = (bPointer*)malloc(sizeof(bPointer));
    if (!element) memoryError();
    element->next = nullptr;
    return element;
}
object* create(char* name){
    object* element = (object*)malloc(sizeof(object));
    if (!element) memoryError();
    element->name = name;
    element->count = 0;
    element->beforeList = nullptr;
    element->next = nullptr;
    return element;
}
object* sort(object* root){
    if (root == nullptr) return nullptr;
    object* ptr, *tmp, *prev = nullptr;
    bool flag;
    do{
        flag = false;
        ptr = root;
        while(ptr->next){
            if(ptr->count > ptr->next->count){
                if(ptr == root){
                    tmp = ptr;
                    ptr = tmp->next;
                    tmp->next = ptr->next;
                    ptr->next = tmp;
                    root = ptr;
                    flag = true;
                }
                else{
                    tmp = ptr;
                    ptr = tmp->next;
                    tmp->next = ptr->next;
                    ptr->next = tmp;
                    prev->next = ptr;
                    flag = true;
                }
            }
            prev = ptr;
            ptr = ptr->next;
        }
    } while(flag);
    return root;
}
int main() {
    FILE *file;
    if( (file = fopen( path, "r" )) == nullptr ) fileError();
    int length;
    char* firstName = get_string(&length, file);
    char* secondName = get_string(&length, file);
    object* root = create(firstName);
    object* element = create(secondName);
    element->count++;
    root->next = element;
    root->beforeList = createBPointer();
    root->beforeList->before = element;
    while (!feof(file)){
        firstName = get_string(&length, file);
        secondName = get_string(&length, file);
        object* zeroA = create(nullptr);
        object* zeroB = create(nullptr);
        object* temp = root;
        bool flagA = false;
        bool flagB = false;
        //--check-if-these-elements-are-in-the-list--//
        while (temp) {
            if (compare(temp->name, firstName)) {
                flagA = true;
                free(zeroA->name);
                free(zeroA);
                zeroA = temp;
            }
            if (compare(temp->name, secondName)) {
                free(zeroB->name);
                free(zeroB);
                flagB = true;
                temp->count++;
                zeroB = temp;
            }
            if (flagB & flagA) break;
            temp = temp->next;
        }
        //--------------------add--------------------//
        if (!flagA){
            zeroA->name = firstName;
            zeroA->next = root->next;
            root->next = zeroA;
        }
        if (!flagB){
            zeroB->name = secondName;
            zeroB->count++;
            zeroB->next = root->next;
            root->next = zeroB;
        }
        //-------------set--dependencies-------------//
        if (!zeroA->beforeList) {
            zeroA->beforeList = createBPointer();
            zeroA->beforeList->before = zeroB;
        } else{
            bPointer* zeroABeforeList = zeroA->beforeList;
            while (zeroABeforeList->next){
                zeroABeforeList = zeroABeforeList->next;
            }
            zeroABeforeList->next = createBPointer();
            zeroABeforeList->next->before = zeroB;
        }//-------------------------------------------//
    }
    sort(root);
    object* temp = root;
    object* r1 = root;
    while (r1) {
        object* last;
        while ((temp != nullptr) && (temp->count == 0)) {
            r1 = temp->next;
            bPointer* bPI = temp->beforeList;
            while (bPI) {
                bPI->before->count--;
                bPI = bPI->next;
            }
            last = temp;
            temp = r1;
        }
        r1 = sort(r1);
        temp = r1;
        last->next = r1;
    }
    printList(root);
    return 0;
}