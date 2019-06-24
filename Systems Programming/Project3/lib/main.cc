#include "../lib/file.h"

#include <iostream>

using namespace std;

int main(){

    try {
        File f("/Users/tetsuya/Documents/Projects/master/CSS503A/HW3/lib/message.txt","r+");

size_t size = 0;
// size_t total_bytes_read = 0;
// int chunk_size = 1;
// char *buf = new char[chunk_size];

//       while((size = f.fread(buf, 1, chunk_size)) > 0){
//         total_bytes_read += size;

//         if(total_bytes_read % 1000 == 0)
//           cout << total_bytes_read << endl;

//         if(f.feof())
//           break;

//         if(f.ferror()){
//           printf("Error while reading using stdio reader");
//           exit(EXIT_FAILURE);
//         }
//       }

        char c;
        size_t total_bytes_read = 0;
        while(!f.feof()){
          char c = f.fgetc();
          cout << c << endl;
          total_bytes_read += 1;

        if(total_bytes_read % 1000 == 0)
          cout << total_bytes_read << endl;

          if(f.ferror()){
            printf("Error while reading using stdio reader");
            exit(EXIT_FAILURE);
          }
        } 


      f.fputc('V');
        // //step 1
        // size_t bytes_written = f3.fwrite("ABC",1,3);
        // if(bytes_written == 3)
        //     cout << "Correct: Writing ABC, bytes written is " << bytes_written << endl;
        // else
        //     cout << "Wrong:  Writing ABC, bytes read is " << bytes_written << endl;

        // //move back to the begining of the file
        // f3.fseek(0, File::SEEK_SET_);

        // //step 2
        // char z[3];
        // size_t bytes_read = f3.fread(z, 1, 3);
        // if(bytes_read == 3){
        //     cout << "Correct: " << bytes_read << " bytes read, chars: ";
        //     for(size_t i = 0; i < 3; i++){
        //         cout << z[i];
        //     }
        //     cout << endl;
        // }else{
        //     cout << "Wrong: " << bytes_read << " bytes read, chars: ";
        //     for(size_t i = 0; i < 3; i++){
        //         cout << z[i];
        //     }
        //     cout << endl;
        // }

        // //step 3
        // if(f3.ferror() == 1)
        //     cout << "Correct: error code is " << f3.ferror() << " after trying to write in read only mode" << endl;
        // else
        //     cout << "Wrong: error code is " << f3.ferror() << " after trying to write in read only mode" << endl;

        // //move to the end of the file
        // f3.fseek(0, File::SEEK_END_);

        // //step 4
        // bytes_written = f3.fwrite("DEF",1,3);
        // if(bytes_written == 3)
        //     cout << "Correct: Writing DEF, bytes written is " << bytes_written << endl;
        // else
        //     cout << "Wrong:  Writing DEF, bytes read is " << bytes_written << endl;


        // size_t rd = 0;
        // const char str[] = "12345678912345";
        // rd = f.fwrite(str,1,14);
        // rd = rd;

        // char c = 'a';
        // for(int i = 0; i < 26; i++){
        //     f.fputc(c++);
        // }
        // f.fflush();
        // File::Whence whence = File::SEEK_SET_;
        // f.fseek(0, whence);
      //   int total_bytes_read = 0;
      //   int size = 0;
      //   int chunk_size = 4096;
      //   char buf[4096];
      // while((size = f.fread(buf, 1, chunk_size)) > 0){
      //   total_bytes_read += size;
      //   cout << total_bytes_read << " was read " << endl;
      //   if(f.feof())
      //     break;

      //   if(f.ferror()){
      //       int a = 0;
      //       a = a;
      //   //   printf("Error while reading using stdio reader");
      //   //   exit(EXIT_FAILURE);
      //   }
      // }

        // char s[8]; 
        // size_t rd = 0;
        // while(!f.feof()){
        //     rd = f.fread(s, 1, 8);
        //     rd = rd;
        //     rd = rd;
        //     rd = rd;
        //     rd = rd;
        // }
      // size_t rd = 0;
      // char s[12];
      //   if(!f.feof())
      //   rd = f.fread(s, 1, 12);

      //   char t[] = "Hello";
      //   f.fputs(t);

        // if(!f.feof())
        // rd = f.fread(s, 1, 4);

      // char s[5];
      // f.fgets(s, 5);

      // char t[] = "Hello";
      // f.fputs(t);

      // cout << f.ferror() << endl;

      // size_t rd = 0;

      // const char str[] = "987123456";
      // rd = f.fwrite(str,1,9);

      // f.fseek(5,File::SEEK_SET_);
      // //   char t[3]; 
      // if(!f.feof())
      //   rd = f.fread(t, 1, 5);

      // char o[20];
      // f.fread(o,1,20);

      // f.fputs(str);

      // rd = rd;
      //   const char str[] = "987123456";
      //   rd = f.fwrite(str,1,9);

      //   if(!f.feof())
      //   rd = f.fread(t, 1, 3);
        
      //   f.fseek(5,File::SEEK_SET_);
      //   if(!f.feof())
      //   rd = f.fread(t, 1, 3);

      //   rd = rd;

        // f.fseek(15,File::SEEK_CUR_);
        // f.fwrite(str,1,2);

        // f.fgetc();

        // f.fputc('X');
        // f.fputc('Y');
        // f.fputc('Z');

        // f.fputc('-');
        // f.fputc('-');
        // f.fputc('-');

    } catch (int param) 
    {
        
    }
}