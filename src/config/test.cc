#include "readConfig.hpp"

int main(int argc, char **argv)
{

    string ip = "ip";
    string value;
    readConfigFile("./test.conf", ip, value);
    cout << value << endl;
}
