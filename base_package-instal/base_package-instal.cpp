#include "base_package-instal.hpp"
#include <iostream>
#include <ostream>

void install_package(){
    std::cout << "Instaling base package..." << std::endl;
    system("pacstrap /mnt base base-devel linux linux-firmware nano sudo dhcpcd networkmanager");

    std::cout << "Genarate fstab..." << std::endl;
    system("genfstab -U /mnt >> /mnt/etc/fstab");
}
