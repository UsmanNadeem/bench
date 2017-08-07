#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <malloc.h>
#include <iostream>

using namespace std;

void* destination = NULL;
char* attackString = "sh";


class ParentOne
{
    public:
        void* buffer[3];
        virtual void doSomething1(char *string){
            cout << "called ParentOne::F1" << endl;

        }
        virtual void doSomething2(char *string){
            cout << "called ParentOne::F2" << endl;
        }
};

class ParentTwo
{
    public:
        
        virtual void foo1(char *string){
            cout << "called ParentTwo::F1" << endl;
        }
        virtual void foo2(char *string)
        {
            cout << "called ParentTwo::F2" << endl;
        }
};

class ParentThree
{
    public:
        
        virtual void foo1(char *string){
            cout << "called ParentThree::F1" << endl;
        }
        virtual void foo2(char *string)
        {
            cout << "called ParentThree::F2" << endl;
        }
};


class SingleInherit1:public ParentOne
{
    public:
        virtual void doSomething1(char *string)
        {
            cout << "called SingleInherit1::F1" << endl;
        }
        virtual void doSomething2(char *string){
            cout << "called SingleInherit1::F2" << endl;
        }

};
class SingleInherit2:public ParentOne
{
    public:
        virtual void doSomething1(char *string)
        {
            cout << "called SingleInherit2::F1" << endl;
        }
        virtual void doSomething2(char *string){
            cout << "called SingleInherit2::F2" << endl;
        }

};

class DoubleInheritParent1:public ParentOne, public ParentTwo
{
    public:
        // virtual void doSomething1(char *string)
        // {
        //     cout << "called DoubleInheritParent1::F1" << endl;
        // }
        // virtual void doSomething2(char *string){
        //     cout << "called DoubleInheritParent1::F2" << endl;
        // }

        // virtual void foo1(char *string){
        //     cout << "called DoubleInheritParent1::F3" << endl;
        // }
        // virtual void foo2(char *string)
        // {
        //     cout << "called DoubleInheritParent1::F4" << endl;
        // }
        void overflow() {

            void** vPTRlocation = (void**)this;
            vPTRlocation += 4;// sizeof(void*)*4;  // 3 for array one for initial vptr

            *(vPTRlocation) = (void*)destination;
        }


};
class DoubleInheritParent2:public ParentOne, public ParentTwo
{
    public:
        virtual void doSomething1(char *string)
        {
            cout << "called DoubleInheritParent2::F1" << endl;
        }
        virtual void doSomething2(char *string){
            cout << "called DoubleInheritParent2::F2" << endl;
        }

        virtual void foo1(char *string){
            cout << "called DoubleInheritParent2::F3" << endl;
        }
        virtual void foo2(char *string)
        {
            cout << "called DoubleInheritParent2::F4" << endl;
        }


};
class DoubleInheritChild1:public DoubleInheritParent1
{
    public:
        virtual void doSomething1(char *string)
        {
            cout << "called DoubleInheritChild1::F1" << endl;
        }
        virtual void doSomething2(char *string){
            cout << "called DoubleInheritChild1::F2" << endl;
        }

        virtual void foo1(char *string){
            cout << "called DoubleInheritChild1::F3" << endl;
        }
        virtual void foo2(char *string)
        {
            cout << "called DoubleInheritChild1::F4" << endl;
        }
        void overflow() {

            void** vPTRlocation = *(void***)this;
            vPTRlocation += 4;// sizeof(void*)*4;  // 3 for array one for initial vptr

            *(vPTRlocation) = (void*)destination;
        }
};

class DoubleInheritChild2:public DoubleInheritParent1
{
    public:
        virtual void doSomething1(char *string)
        {
            cout << "called DoubleInheritChild2::F1" << endl;
        }
        virtual void doSomething2(char *string){
            cout << "called DoubleInheritChild2::F2" << endl;
        }

        virtual void foo1(char *string){
            cout << "called DoubleInheritChild2::F3" << endl;
        }
        virtual void foo2(char *string)
        {
            cout << "called DoubleInheritChild2::F4" << endl;
        }

};



void doSyscall(int x, char* string) {
    cout << "calling system with args: " << string << endl;
    system(string);
}


int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        cout << "not enough params\n";
        exit(0);
    }

    cout << argv[0] <<" "<<argv[1] <<" "<<argv[2] << endl;    

    switch (atoi(argv[1])) {  //todo fix

        case 0:  // start of good vable point to good inheritance func: 
        {
            if (atoi(argv[2]) > 3)  // double inherit
            {
                cout << "not possible for now\n";
                exit(0);
                // DoubleInheritParent1* object2 = new DoubleInheritChild2();
                // void* vtableO2 = (void*)*((void**)object2 +4);  // 1 ptr + 3 for buffer
                // destination = vtableO2;
                // delete object2;
                // break;
            }

            // P1 obj of SI1 points to SI2
            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;
            destination = vtableO2;
            delete object2;
            break;
        }


        case 1:  // middle of good vable point to good inheritance func: 
        {
            if (atoi(argv[2]) > 3)  // double inherit
            {
                cout << "not possible for now\n";
                exit(0);
                // DoubleInheritParent1* object2 = new DoubleInheritChild2();
                // void* vtableO2 = (void*)*((void**)object2 +4);  // 1 ptr + 3 for buffer
                // destination = (void*)((uint64_t)vtableO2 + sizeof(void*));  // second ptr
                // delete object2;
                // break;
            }

            // P1 obj of SI1 points to SI2 func 2
            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((uint64_t)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }

        case 2:  // start of good vable point to bad inheritance func:
        {
            if (atoi(argv[2]) > 3)  // double inherit
            {
                ParentThree* object2 = new ParentThree();
                void* vtableO2 = (void*)*((void**)object2);  // 1 ptr + 3 for buffer
                destination = (void*)vtableO2;  // first ptr
                delete object2;
                break;
            }

            // P1 obj of SI1 points to P2
            ParentTwo* object2 = new ParentTwo();
            void* vtableO2 = (void*)*(void**)object2;
            destination = vtableO2;
            
            delete object2;
            break;
        }

        case 3:  // middle of good vable point to bad inheritance func:
        {
            if (atoi(argv[2]) > 3)  // double inherit
            {
                ParentThree* object2 = new ParentThree();
                void* vtableO2 = (void*)*((void**)object2);  // 1 ptr + 3 for buffer
                destination = (void*)((uint64_t)vtableO2 + sizeof(void*));  // second ptr
                delete object2;
                break;
            }
            // P1 obj of SI1 points to P2 func 2
            ParentTwo* object2 = new ParentTwo();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((uint64_t)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }

        case 4:  // crafted vable point to good inheritance func:
        {
            if (atoi(argv[2]) > 3)  // double inherit
            {
                cout << "should call the same method\n";
                void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary
                DoubleInheritParent1* object2 = new DoubleInheritParent1();
                void* vtableO2 = (void*)*((void**)object2 +4);  // 1 ptr + 3 for buffer

                for (int i = 0; i < 8; ++i)
                {
                    *(buffer+i) = *(((void**)vtableO2));
                }

                destination = buffer;
                delete object2;
                break;
            }

            // make a buffer and use it as a vtable point to SI2
            void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary
            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;

            for (int i = 0; i < 8; ++i)
            {
                *(buffer+i) = *(((void**)vtableO2));
            }

            destination = buffer;
            
            delete object2;
            break;
        }

        case 5:  // crafted vable point to bad inheritance func:
        {

            // dont need this just point to P3 object 
            if (atoi(argv[2]) > 3)  // double inherit
            {
                // void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary
                // DoubleInheritParent2* object2 = new DoubleInheritParent2();
                // void* vtableO2 = (void*)*((void**)object2 +4);  // 1 ptr + 3 for buffer
                void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary
                ParentThree* object2 = new ParentThree();
                void* vtableO2 = (void*)*(void**)object2;

                for (int i = 0; i < 8; ++i)
                {
                    *(buffer+i) = *(((void**)vtableO2)+1);  // second function
                }

                destination = buffer;
                delete object2;
                break;
            }

            // make a buffer and use it as a vtable point to P2
            void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary
            ParentTwo* object2 = new ParentTwo();
            void* vtableO2 = (void*)*(void**)object2;

            for (int i = 0; i < 8; ++i)
            {
                *(buffer+i) = *(((void**)vtableO2)+1);  // second function
            }

            destination = buffer;
            
            delete object2;
            break;
        }

        case 6:  // crafted vable nonClassFunc:
        {
            // make a buffer and use it as a vtable point to doSyscall
            void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary

            for (int i = 0; i < 8; ++i)
            {
               *(buffer+i) = (void*)&doSyscall;  // second function
            }

            destination = buffer;
            break;
        }


        case 7:  // crafted vable middle of nonClassFunc:
        {
            // make a buffer and use it as a vtable point to syslbl
            void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary

            for (int i = 0; i < 8; ++i)
            {
                *(buffer+i) = &&syslbl;  // second function
            }

            destination = buffer;
            break;
        }
        // todo shellcode
        // todo existing code double reference

        default: {
            cout << "bad first option\n";
            exit(0);
            break;
        }
    }


    switch (atoi(argv[2])) {  //todo fix
        case 0:  //stack+SingleInherit+sequentialOverFlow:
        {
            SingleInherit1 singleObj;
            void* buffer[3];

            ParentOne* object = &singleObj;

            if (((void*)object < (void*)buffer))
            {
                cout << "*****cant overflow...object before buffer\n\n" << endl;
                printf("object = %p\tbuffer = %p\n", object, buffer);
                exit(0);
            }

            printf("old vpointer = %p\n", (void*)*(void**)object);
            // printf("address of buffer = %p \naddress of object = %p\n\n", &buffer[0], &singleObj);
            for (int i = 0; i<((uint64_t)object - (uint64_t)buffer); ++i)  // overflow
            {
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            printf("new vpointer = %p\n", (void*)*(void**)object);
            printf("vpointer to copy = %p\n", destination);

            cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 1:  // heap+SingleInherit+sequentialOverFlow:
        {
            // void** buffer = (void **)malloc(sizeof(void*)*3);
            void** buffer = new void*[3];
            ParentOne* object = new SingleInherit1();

            if (((void*)object < (void*)buffer))
            {
                cout << "*****cant overflow...object before buffer\n\n" << endl;
                printf("object = %p\tbuffer = %p\n", object, buffer);
                exit(0);
            }

            printf("old vpointer = %p\n", (void*)*(void**)object);

            for (int i = 0;i<((uint64_t)object - (uint64_t)buffer) ; ++i)  // overflow
            {
                // cout << ((uint64_t)object - (uint64_t)buffer)<< "   "<<i << endl;
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }
            printf("new vpointer = %p\n", (void*)*(void**)object);
            printf("vpointer to copy = %p\n", destination);


            cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);

            break;
        }

        case 2:  // stack+SingleInherit+indirectOverwrite:
        {
            SingleInherit1 singleObj;

            ParentOne* object = &singleObj;

            printf("old vpointer = %p\n", (void*)*(void**)object);

            *(void**)object = destination;

            printf("new vpointer = %p\n", (void*)*(void**)object);
            printf("vpointer to copy = %p\n", destination);
            cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 3:  // heap+SingleInherit+indirectOverwrite:
        {
            ParentOne* object = new SingleInherit1();

            printf("old vpointer = %p\n", (void*)*(void**)object);

            *(void**)object = destination;
            
            printf("new vpointer = %p\n", (void*)*(void**)object);
            printf("vpointer to copy = %p\n", destination);
            
            cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

//////////////////////////double
        case 4:  // stack+DoubleInherit+withinObjectOverFlow:
        {
            DoubleInheritParent1 doubleObj;

            DoubleInheritParent1* object = &doubleObj;

            for (int i = 0; i < 4; ++i)  // overflow assume that vptr is directly after
            {
                *(object->buffer + i) = (void*)destination;
            }

            cout << "calling DoubleInheritParent1::F3 ParentTwo::F1" << endl;
            object->foo1(attackString);
            break;
        }

        case 5:  // heap+DoubleInherit+withinObjectOverFlow:
        {
            DoubleInheritParent1* object = new DoubleInheritParent1();

            for (int i = 0; i < 4; ++i)  // overflow assume that vptr is directly after
            {
                *(object->buffer + i) = (void*)destination;
            }

            cout << "calling DoubleInheritParent1::F3 ParentTwo::F1" << endl;
            object->foo1(attackString);
            break;
        }

        case 6:  // stack+DoubleInherit+INdirectOverwrite:
        {
            DoubleInheritParent1 doubleObj;

            DoubleInheritParent1* object = &doubleObj;

            object->overflow();

            cout << "calling DoubleInheritParent1::F3 ParentTwo::F1" << endl;
            object->foo1(attackString);
            break;
        }

        case 7:  // heap+DoubleInherit+INdirectOverwrite:
        {

            DoubleInheritParent1* object = new DoubleInheritParent1();

            object->overflow();

            cout << "calling DoubleInheritParent1::F3 ParentTwo::F1" << endl;
            object->foo1(attackString);
            break;
        }

        default: {
            cout << "bad second option\n";
            exit(0);
            break;
        }
    }



    return 0;

syslbl:
    system(attackString);



 //    ******************uaf
	// Child1* c1 = new Child1();

	// c1 -> doSomething("sh");

	// free(c1);

	// new Child2();

	// c1 -> doSomething("sh");
}


















