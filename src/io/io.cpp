#include <io/io.h>

namespace IO {
    int readFromFile(std::vector<uint8_t>& data,const std::string& filename)
    {
        FILE * pFile;
        pFile = fopen(filename.c_str(),"r");
        if(!pFile) return -1;

        fseek(pFile, 0, SEEK_END);
        int size = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);

        data.resize(size + 1);
        int read = fread(data.data(),size,1,pFile);
        data[size] = '\0';
        return read >= 0 ? size : -1;
    }
}