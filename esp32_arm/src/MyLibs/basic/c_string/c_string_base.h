#pragma once

// #define  REPRAP_GCODE_MAX_SIZE  38

#define C_STRING_OK  1
#define C_STRING_ERR_OVER_SIZE -2
#define C_STRING_ERR_ -1

class  C_String_Base{
    public:
        int CopyFrom(const char* bytes);
        int CopyFrom(const char* bytes, int length);
        void PrintFlat(const char* title);
        void CopyTo(char* destination);
        bool IsEqualTo(const char* chars);
        bool IsPrefix(const char* chars);
        char* GetChars(){return __chars;};
        C_String_Base(){};
        void Init(int buffer_size, char* buffer);
        int Find_MismachLocation(const void* b, int length);
        

        // const char &GetText = *__chars;  // For Arduino::chars[n]
        // const char* GetChars = __chars;   // For std::cxx11::string
    protected:
        // C_String_Base(int buffer_size);
        void _CopyTo(char* destination, int remove_prefix_bytes_length);
        C_String_Base(int buffer_size, char* buffer);

    private:
        char* __chars;
        int __buffer_size;

};


