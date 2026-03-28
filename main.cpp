#include "check_internet_connected/check_internet_connected.hpp"
#include "disk_partitioning/disk_partitioning.hpp"
#include "create_script/create_script.hpp"
#include "base_package-instal/base_package-instal.hpp"
#include <iostream>
#include <ostream>

int main(){
    std::cout << "========HELLO========\n" <<
                 "=====ARCHINSTALL=====\n" <<
                 "=====by rei-root=====\n" <<
                 "---------------------\n" << std::endl;
    if(!internet_check()){
        std::cout << "No internet connection! Please connect to the internet!\n" << std::endl;
        std::cout << "To connect to the Internet, you can use the prompts:\n" <<
                     "1. iwctl\n"
                     "2. station wlan0 scan\n"
                     "3. station wlan0 get-networks\n"
                     "4. station wlan0 connect SSID\n" << std::endl;
        return 1;
    }
    std::cout << "========Install base package========" << std::endl;
    install_package();

    std::cout << "=====Disk partitioning=====\n" << std::endl;
    disk_partitioning();

    std::cout << "=======Create install.sh=======";
    create_script();

}
