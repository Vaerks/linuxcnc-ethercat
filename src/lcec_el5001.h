//
//    Copyright (C) 2011 Sascha Ittner <sascha.ittner@modusoft.de>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
//
#ifndef _LCEC_EL5001_H_
#define _LCEC_EL5001_H_

#include "lcec.h" //MUST BE INCLUDED

#define LCEC_EL5001_VID LCEC_BECKHOFF_VID //defined elsewhere

#define LCEC_EL5001_PID 0x13893052 //get from cstruct, twincat or device description file

#define LCEC_EL5001_PDOS 2 // Depends on amount of times LCEC_INIT is called

int lcec_el5001_init(int comp_id, struct lcec_slave *slave, ec_pdo_entry_reg_t *pdo_entry_regs); //Funcion declaration

#endif
