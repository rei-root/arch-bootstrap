
// arch-bootstrap - Arch Linux installer
// Copyright (C) 2026 rei-root
// This program is free software under the GNU GPL v3 license.
// See LICENSE file for details.

#include "base_package-instal.hpp"
#include <iostream>
#include <ostream>

void install_package(){
    std::cout << "Instaling base package..." << std::endl;
    system("pacstrap /mnt base base-devel linux linux-firmware nano sudo dhcpcd networkmanager");

    std::cout << "Genarate fstab..." << std::endl;
    system("genfstab -U /mnt > /mnt/etc/fstab");
}
