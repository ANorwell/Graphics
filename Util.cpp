#include "Util.h"

Log::LogLevel Log::pLevel = Log::DEBUG;
std::fstream* Log::pFile = NULL;

char* Util::slurp(const char* fn, const char* mode) {
    
    FILE *fp;
    fp = fopen(fn,mode);
    
    if (fp == NULL)
        throw NoFileException();
    
    fseek(fp, 0, SEEK_END);
    int count = ftell(fp);
    rewind(fp);

    //for text files, add a null delimiter
    size_t size = count;
    if (mode[0] == 'r')
        size++;
    
    char* content = new char[size];
    count = fread(content,sizeof(char),count,fp);

    if (mode[0] == 'r')
        content[count] = '\0';
    
    std::cout << "Read a file of size " << count << std::endl;
    
    fclose(fp);
    return content;
}
