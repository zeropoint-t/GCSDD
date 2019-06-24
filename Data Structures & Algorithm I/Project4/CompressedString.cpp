#include <iostream>
#include <vector>
#include <cmath>
#include "CompressedString.h"

using namespace std;

ostream& operator<<(ostream &outStream, const CompressedString &comp)
{
    string decompressed = comp.decompress();
    outStream << "Decompressed string: \t" << decompressed << endl;

	outStream << "String went from " << decompressed.length() << " to " 
    << comp.compressed.size() << endl;

    //calculate reduction rate
    int diff = decompressed.length() - (double)comp.compressed.size();
    double reductionRatio = (double)diff / decompressed.length();
    int reductionPct = (int)(reductionRatio * 100);

    outStream << "The string was compressed by " << reductionPct << "%";
	return outStream;
}

CompressedString::CompressedString(const string str){
    compress(str);
}

/*
    Compresses str parameter in a number of repeated character plus the repeated character format(ex {'3','a'}) and stores the values into a vector "compressed"
    
    ***Big-O Analysis***
    It loops through each character in a str parameter n times.
    Vector.push_back has amotized time complexity of O(1)
    Therefore, this function operates at O(n) time complexity
*/
void CompressedString::compress(const string str){
    //clear private vectors
    compressed.clear();

    //return if empty string
    if(str.length() == 0){
        cout << "Empty String!" << endl;
        return;
    }

    //insert as is if string length is 2 or less since there won't be any compression
    if(str.length() <= 2){
        for(auto c = str.begin(); c != str.end(); c++){
            compressed.push_back(*c);
        }
        return;
    }

    int cnt = 0;
    char cur_char;
    for(auto it = str.begin(); it != str.end(); it++)
    {
        if(cur_char == '\0'){
            cur_char = *it;
            cnt++;
            continue;
        }

        if(cur_char == *it){
            cnt++;
        }

        if(cur_char != *it){
            pushToCompressed(cnt, cur_char);
            //reset for next
            cur_char = *it;
            cnt = 1;
        }
    }

    //take care of the last pattern
    pushToCompressed(cnt, cur_char);
}
/*
    Decompresses characters stored in vector "compressed" and stores it in a local string and returns it
    
    ***Big-O Analysis***
    It loops through each character in the "compressed" vector once.
    Since the original string contains "n" characters and original chars are recreated n times, this function operates at O(n) time complexity
*/
string CompressedString::decompress() const{
    string origString;
    vector<char> nums;
    char c;
    for(auto it = compressed.begin(); it != compressed.end(); it++){
        c = *it;
        if(isdigit(c)){
            nums.push_back(c);
        }else{
            int cnt = getNum(nums);
            for(int i = 0; i < cnt; i++){
                origString.push_back(c);
            }
            nums.clear();
        }
    }
    // string s(decompressed.begin(), decompressed.end());
    return origString;
}

int CompressedString::getNum(const vector<char>& nums) const{
    if(nums.size() == 0)
        return 1;
    else if(nums.size() == 1){
        return ((int)nums[0] - 48);
    }

    int num = 0;
    int p = nums.size()-1;
    for(int i = 0; i < nums.size(); i++){
        num += pow(10,p--) * ((int)nums[i] - 48);
    }
    return num;
}

void CompressedString::DisplayCompressed(){
    string s(compressed.begin(), compressed.end());
    cout << "Compressed string: \t" << s << endl;
}

void CompressedString::pushToCompressed(const int cnt, const char c){
    //next char is different
    if(cnt <= 2){
        compressed.insert(compressed.end(), cnt, c);
    }else{
        //insert char count
        string s = to_string(cnt);
        for(auto c = s.begin(); c != s.end(); c++){
            compressed.push_back(*c);
        }
        //inser char
        compressed.push_back(c);
    }
}