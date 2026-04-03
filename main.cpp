
/*
 * Arch-Bootstrap
 * Copyright (C) 2026 rei-root (https://github.com/rei-root)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
