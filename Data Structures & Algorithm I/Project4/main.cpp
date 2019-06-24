
//
//  Compressed_String.cpp
//  HW4
//
//  UW BOTHELL
//  FALL 2018
//  CSS501, HANSEL ONG INSTRUCTOR
//  Monday / Wednesday, 6-8 PM, UW2 304
//

#include <iostream>
#include <vector>
#include <string>

#include "CompressedString.h"
using namespace std;

// void compressString(string input){

//     vector<char> compressedString;

//     compressedString.clear(); //get rid of any values currently in the compressed string vector, otherwise values will get added to the end of whatever currently exists. Unnecessary for the scope of this assignment
    
//     vector<char> data(input.begin(), input.end());
//     vector<char>::iterator it;
//     int mult;
    
//     for (it = data.begin(); it != data.end(); it++){//loop through the string
//         //handle multiple characters
//         mult = 1;
//         while(*it == *(it+1)){
//             mult++;
//             it++;
//         }

//         if (mult != 1){
//             compressedString.push_back((char)(mult + '0')); //only accessed to push a multiplier value to the compressedString vector
//         }
        
//         compressedString.push_back(*it); //pushes every unique group of characters in the string (even single groups)
//     }

//     for(auto it = compressedString.begin(); it != compressedString.end(); it++){
//         cout << *it;
//     }
//     cout << endl;

//     for (it = compressedString.begin(); it != compressedString.end(); ++it){
//         int temp;
//         if(isdigit(*it)){ //handles duplicate character blocks
//             temp = (int)*it-'0';
//             for(int i = 0; i < temp; i++){
//                 cout << *(it+1);
//             }
//             ++it;
//         }
//         else cout << *it;
//     }

//     cout << endl;
// }

int reverse(int x) {
    // string sign = x > 0 ? "" : "-";
    // string str_num = to_string(abs(x));
    // int size = str_num.size();
    // string reverse_str = "";
    // for(int i = 1; i <= size; i++){
    //     reverse_str.push_back(str_num[size-i]);
    // }
    // return atoi((sign + reverse_str).c_str());

    int rev = 0;
    while (x != 0) {
        int pop = x % 10;
        x /= 10;
        if (rev > INT_MAX/10 || (rev == INT_MAX / 10 && pop > 7)) return 0;
        if (rev < INT_MIN/10 || (rev == INT_MIN / 10 && pop < -8)) return 0;
        rev = rev * 10 + pop;
    }
    return rev;
}

int main(int argc, const char* argv[]){

    int a = reverse(321);

    // compressString("aaabbbbc");
    cout << endl;

    //---------------------------------------
    //string to compress 1
    cout << "---Demonstration of short string---" << endl;
    string strToCompress1 = "abc";
    cout << "String to compress: \t" << strToCompress1 << endl;

    CompressedString cs1(strToCompress1);
    cs1.DisplayCompressed();
    cout << cs1 << endl << endl;
    //---------------------------------------


    //---------------------------------------
    //string to compress 2
    cout << "---Demonstration of long string---" << endl;
    string strToCompress2 = "abbbbccccccddeeefgg";
    cout << "String to compress: \t" << strToCompress2 << endl;

    CompressedString cs2(strToCompress2);
    cs2.DisplayCompressed();
    cout << cs2 << endl << endl;
    //---------------------------------------


    //---------------------------------------
    //string to compress 3
    cout << "---Demonstration of loooooooooong string---" << endl;
    string strToCompress3 = "abbbbccccccddeeefffffffffffffffffffffghhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh";//"abcdefghijklmn";
    cout << "String to compress: \t" << strToCompress3 << endl;

    CompressedString cs3(strToCompress3);
    cs3.DisplayCompressed();
    cout << cs3 << endl << endl;
    //---------------------------------------


    //---------------------------------------
    cout << "***Big-O Analysis***" << endl;
    cout << "Compress method: It loops through each character in a str parameter n times." << endl;
    cout << "Vector.push_back has amotized time complexity of O(1)." << endl;
    cout << "Therefore, compress function operates at O(n) time complexity" << endl << endl;

    cout << "Decompress method: It loops through each character in the \"compressed\" vector once." << endl;
    cout << "Since the original string contains \"n\" characters and the characters are recreated n times." << endl;
    cout << "Therefore, decompress function operates at O(n) time complexity" << endl;
    //----------------------------------------
    return 0;
}