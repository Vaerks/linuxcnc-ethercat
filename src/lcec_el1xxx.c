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

#include "lcec.h"
#include "lcec_el1xxx.h"

typedef struct {
  hal_bit_t *in;
  hal_bit_t *in_not;
  unsigned int pdo_os;
  unsigned int pdo_bp;
} lcec_el1xxx_pin_t;

static const lcec_pindesc_t slave_pins[] = {
  { HAL_BIT, HAL_OUT, offsetof(lcec_el1xxx_pin_t, in), "%s.%s.%s.din-%d" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el1xxx_pin_t, in_not), "%s.%s.%s.din-%d-not" },
  { HAL_TYPE_UNSPECIFIED, HAL_DIR_UNSPECIFIED, -1, NULL }
};

void lcec_el1xxx_read(struct lcec_slave *slave, long period);

int lcec_el1xxx_init(int comp_id, struct lcec_slave *slave, ec_pdo_entry_reg_t *pdo_entry_regs) {
  rtapi_print_msg(RTAPI_MSG_ERR,"EL1xxx INIT begun... \n");
  lcec_master_t *master = slave->master;
  lcec_el1xxx_pin_t *hal_data;
  lcec_el1xxx_pin_t *pin;
  int i;
  int err;

  // initialize callbacks
  slave->proc_read = lcec_el1xxx_read;

  // alloc hal memory
  if ((hal_data = hal_malloc(sizeof(lcec_el1xxx_pin_t) * slave->pdo_entry_count)) == NULL) {
    rtapi_print_msg(RTAPI_MSG_ERR, LCEC_MSG_PFX "hal_malloc() for slave %s.%s failed\n", master->name, slave->name);
    return -EIO;
  }
  memset(hal_data, 0, sizeof(lcec_el1xxx_pin_t) * slave->pdo_entry_count);
  slave->hal_data = hal_data;

  // initialize pins
  for (i=0, pin=hal_data; i<slave->pdo_entry_count; i++, pin++) {
    // initialize POD entry
    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000 + (i << 4), 0x01, &pin->pdo_os, &pin->pdo_bp);

    // export pins
    if ((err = lcec_pin_newf_list(pin, slave_pins, LCEC_MODULE_NAME, master->name, slave->name, i)) != 0) {
      return err;
    }
  }
  rtapi_print_msg(RTAPI_MSG_ERR,"EL1xxx INIT finished... \n");
  return 0;
}

void lcec_el1xxx_read(struct lcec_slave *slave, long period) {
  lcec_master_t *master = slave->master;
  lcec_el1xxx_pin_t *hal_data = (lcec_el1xxx_pin_t *) slave->hal_data;
  uint8_t *pd = master->process_data;
  lcec_el1xxx_pin_t *pin;
  int i, s;

  // wait for slave to be operational
  if (!slave->state.operational) {
    return;
  }

  // check inputs
  for (i=0, pin=hal_data; i<slave->pdo_entry_count; i++, pin++) {
    s = EC_READ_BIT(&pd[pin->pdo_os], pin->pdo_bp);
    *(pin->in) = s;
    *(pin->in_not) = !s;
  }
}

