#include "check_internet_connected/check_internet_connected.hpp"
#include "disk_partitioning/disk_partitioning.hpp"
#include "create_script/create_script.hpp"
#include "base_package-instal/base_package-instal.hpp"
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>

int main(){
    std::cout << "========HELLO========\n" <<
                 "=====ARCHINSTALL=====\n" <<
                 "=====by rei-root=====\n" <<
                 "---------------------\n" << std::endl;
    if(!internet_check()){
        std::cout << "No internet connection! Please connect to the internet!\n" << std::endl;
        std::cout << "To connect to the Internet, you can use the prompts:\n" <<
                     "1. iwctl\n"
                     "2. station wlan0(replace your network interface) scan\n"
                     "3. station wlan0 get-networks\n"
                     "4. station wlan0 connect SSID\n"
                     "wlan0 replace your network interface\n" << std::endl;
        return 1;
    }

    std::string ml_choise;

    while (true) {
        std::cout << "Update mirror list for better speed and stability? (y/n): ";
        std::getline(std::cin, ml_choise);

        if(ml_choise == "y" || ml_choise == "Y" || ml_choise == "yes"){
            std::cout << "Mirrors are being updated...\n";
            system("reflector --latest 10 --protocol https --sort rate --save /etc/pacman.d/mirrorlist");
            break;
        } else if(ml_choise == "n" || ml_choise == "N" || ml_choise == "no"){
            std::cout << "Skip mirrors update\n";
            break;
        } else {
            std::cout << "Invalid input. Write y/n, Y/N, yes/no\n" << std::endl;
        }
    }


    std::cout << "=====Disk partitioning=====\n" << std::endl;
    disk_partitioning();

    std::cout << "========Install base package========" << std::endl;
    install_package();

    std::cout << "=======Create install.sh=======";
    create_script();

}
