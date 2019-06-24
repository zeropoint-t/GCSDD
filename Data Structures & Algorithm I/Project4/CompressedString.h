#ifndef CompressedString_h
#define CompressedString_h

    #include <vector>
    #include <string>

    using namespace std;

    class CompressedString{

        friend ostream& operator<<(ostream &outStream, const CompressedString &comp);

        public:
            CompressedString(const string str);
            void DisplayCompressed();
           
        private:
            vector<char> compressed;
            void compress(const string str);
            string decompress() const;
            //converts a vector of numeric chars to an actual integer
            int getNum(const vector<char>& nums) const;
            //helper method to push char c into compressed vector cnt times
            void pushToCompressed(const int cnt, const char c);
    };

#endif