
// arch-bootstrap - Arch Linux installer
// Copyright (C) 2026 rei-root
// This program is free software under the GNU GPL v3 license.
// See LICENSE file for details.

#include "check_internet_connected.hpp"
#include <cstdlib>

bool internet_check(){
    int result = std::system("ping -c 1 -W 2 8.8.8.8 > /dev/null 2>&1");
        return (result == 0);
}
