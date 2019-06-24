#include <iostream>
#include <stdio.h>
#include <string.h>

#include "file.h"

using namespace std;

int main(){

    try {
        /*
            "r" demo
            A file contains A through Z
            Steps:
            1. Read first 5 chars
            2. Check if it is eof
            3. Read next char
            4. write 5 chars
            5. Check error flag
        */
        cout << "----- r demo -----" << endl;
        File f1("message1.txt","r");
        //step 1
        char s[5];
        size_t bytes_read = f1.fread(s, 1, 5);
        if(bytes_read == 5){
            cout << "Correct: " << bytes_read << " bytes read, chars: ";
            for(size_t i = 0; i < 5; i++){
                cout << s[i];
            }
            cout << endl;
        }else{
            cout << "Wrong: wrong number of bytes were read" << endl;
        }

        //step 2
        if(f1.feof()) 
            cout << "Wrong: it should not be eof" << endl;
        else
            cout << "Correct: it is not eof yet" << endl;

        //step 3
        char c = f1.fgetc();
        if(c == 'F')
            cout << "Correct: the next char is " << c << endl;
        else
            cout << "Wrong: the next char shouldn't be " << c << endl;

        //step 4
        size_t bytes_written = f1.fwrite("12345",1,5);
        if(bytes_written == 0)
            cout << "Correct: bytes written: " << bytes_written << endl;
        else
            cout << "Wrong: bytes written: " << bytes_written << endl;

        //step 5
        if(f1.ferror() == 1)
            cout << "Correct: error code is " << f1.ferror() << " after trying to write in read only mode" << endl;
        else
            cout << "Wrong: error code is " << f1.ferror() << " after trying to write in read only mode" << endl;


cout << endl;

        /*
            "r+" demo
            A file contains A through Z
            Steps:
            1. Read the entire file
            2. Read first 3 chars
            3. Check if it is eof
            4. Read first 3 chars
            5. Read the entire file
            6. Reset to original chars(ABCDEFGHI)
        */
        cout << "----- r+ demo -----" << endl;
        File f2("message2.txt","r+");
        //step 1
        char d[9];
        bytes_read = f2.fread(d, 1, 9);
        if(bytes_read == 9){
            cout << "Correct: " << bytes_read << " bytes read, chars: ";
            for(size_t i = 0; i < 9; i++){
                cout << d[i];
            }
            cout << endl;
        }else{
            cout << "Wrong: " << bytes_read << " bytes read" << endl;
        }

        //move back to the beggining of the file
        f2.fseek(0, File::SEEK_SET_);

        //step 2
        char t[3];
        bytes_read = f2.fread(t, 1, 3);
        if(bytes_read == 3){
            cout << "Correct: " << bytes_read << " bytes read, chars: ";
            for(size_t i = 0; i < 3; i++){
                cout << t[i];
            }
            cout << endl;
        }else{
            cout << "Wrong: wrong number of bytes were read" << endl;
        }

        //step 3
        bytes_written = f2.fwrite("XYZ",1,3);
        if(bytes_written == 3)
            cout << "Correct: Writing XYZ, bytes written is " << bytes_written << endl;
        else
            cout << "Wrong:  Writing XYZ, bytes read is " << bytes_written << endl;

        //step 4
        bytes_read = f2.fread(t, 1, 3);
        if(bytes_read == 3){
            cout << "Correct: " << bytes_read << " bytes read, chars: ";
            for(size_t i = 0; i < 3; i++){
                cout << t[i];
            }
            cout << endl;
        }else{
            cout << "Wrong: wrong number of bytes were read" << endl;
        }

        //move back to the beggining of the file
        f2.fseek(0, File::SEEK_SET_);

        //step 5
        bytes_read = f2.fread(d, 1, 9);
        if(bytes_read == 9){
            cout << "Correct: " << bytes_read << " bytes read, chars: ";
            for(size_t i = 0; i < 9; i++){
                cout << d[i];
            }
            cout << endl;
        }else{
            cout << "Wrong: " << bytes_read << " bytes read";
        }

        //move back to the beggining of the file
        f2.fseek(0, File::SEEK_SET_);

        //step 6
        bytes_written = f2.fwrite("ABCDEFGHI",1,9);
        if(bytes_written == 9)
            cout << "Correct: Writing ABCDEFGHI, bytes written is " << bytes_written << endl;
        else
            cout << "Wrong:  Writing ABCDEFGHI, bytes read is " << bytes_written << endl;

cout << endl;


        /*
            "w" demo
            A file contains A through Z
            Steps:
            1. Write ABC
            2. Read ABC
            3. Check Error Flag
            3. Write DEF at the end
        */
        cout << "----- w demo -----" << endl;
        File f3("message3.txt","w");
        //step 1
        bytes_written = f3.fwrite("ABC",1,3);
        if(bytes_written == 3)
            cout << "Correct: Writing ABC, bytes written is " << bytes_written << endl;
        else
            cout << "Wrong:  Writing ABC, bytes read is " << bytes_written << endl;

        //move back to the begining of the file
        f3.fseek(0, File::SEEK_SET_);

        //step 2
        char z[3];
        bytes_read = f3.fread(z, 1, 3);
        if(bytes_read == 0){
            cout << "Correct: " << bytes_read << " bytes read"
             << endl;
        }else{
            cout << "Wrong: " << bytes_read << " bytes read, chars: ";
            for(size_t i = 0; i < 3; i++){
                cout << z[i];
            }
            cout << endl;
        }

        //step 3
        if(f3.ferror() == 1)
            cout << "Correct: error code is " << f3.ferror() << " after trying to read in write only mode" << endl;
        else
            cout << "Wrong: error code is " << f3.ferror() << " after trying to read in write only mode" << endl;

        //move to the end of the file
        f3.fseek(0, File::SEEK_END_);

        //step 4
        bytes_written = f3.fwrite("DEF",1,3);
        if(bytes_written == 3)
            cout << "Correct: Writing DEF, bytes written is " << bytes_written << endl;
        else
            cout << "Wrong:  Writing DEF, bytes read is " << bytes_written << endl;


cout << endl;

        /*
            "w+" demo
            A file contains A through Z
            Steps:
            1. Write ABC
            2. Read first 3 chars
            3. Check error flag
        */
        cout << "----- w+ demo -----" << endl;
        File f4("message4.txt","w+");

        //step 1
        bytes_written = f4.fwrite("ABC",1,3);
        if(bytes_written == 3)
            cout << "Correct: Writing ABC, bytes written is " << bytes_written << endl;
        else
            cout << "Wrong:  Writing ABC, bytes read is " << bytes_written << endl;

        //move back to the beggining of the file
        f4.fseek(0, File::SEEK_SET_);

        //step 2
        char w[3];
        bytes_read = f4.fread(w, 1, 3);
        if(bytes_read == 3){
            cout << "Correct: " << bytes_read << " bytes read, chars: ";
            for(size_t i = 0; i < 3; i++){
                cout << w[i];
            }
            cout << endl;
        }else{
            cout << "Wrong: wrong number of bytes were read" << endl;
        }

        //step 3
        if(f4.ferror() == 0)
            cout << "Correct: error code is " << f4.ferror() << endl;
        else
            cout << "Wrong: error code is " << f4.ferror() << endl;


    } catch (int param) 
    {
        
    }
}