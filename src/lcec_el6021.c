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
#include "lcec_el6021.h"

typedef struct {
  hal_u32_t *control;
  hal_u32_t *status;
  hal_u32_t *tx_data0;
  hal_u32_t *tx_data1;
  hal_u32_t *tx_data2;
  hal_u32_t *tx_data3;
  hal_u32_t *tx_data4;
  hal_u32_t *tx_data5;
  hal_u32_t *tx_data6;
  hal_u32_t *tx_data7;
  hal_u32_t *tx_data8;
  hal_u32_t *tx_data9;
  hal_u32_t *tx_data10;
  hal_u32_t *tx_data11;
  hal_u32_t *tx_data12;
  hal_u32_t *tx_data13;
  hal_u32_t *tx_data14;
  hal_u32_t *tx_data15;
  hal_u32_t *tx_data16;
  hal_u32_t *tx_data17;
  hal_u32_t *tx_data18;
  hal_u32_t *tx_data19;
  hal_u32_t *tx_data20;
  hal_u32_t *tx_data21;
  hal_u32_t *rx_data0;
  hal_u32_t *rx_data1;
  hal_u32_t *rx_data2;
  hal_u32_t *rx_data3;
  hal_u32_t *rx_data4;
  hal_u32_t *rx_data5;
  hal_u32_t *rx_data6;
  hal_u32_t *rx_data7;
  hal_u32_t *rx_data8;
  hal_u32_t *rx_data9;
  hal_u32_t *rx_data10;
  hal_u32_t *rx_data11;
  hal_u32_t *rx_data12;
  hal_u32_t *rx_data13;
  hal_u32_t *rx_data14;
  hal_u32_t *rx_data15;
  hal_u32_t *rx_data16;
  hal_u32_t *rx_data17;
  hal_u32_t *rx_data18;
  hal_u32_t *rx_data19;
  hal_u32_t *rx_data20;
  hal_u32_t *rx_data21;

 // unsigned char data[];
  unsigned int status_os;
  unsigned int control_os;
  unsigned short int tx_data0_os;
  unsigned short int rx_data0_os;
} lcec_el6021_data_t;


static const lcec_pindesc_t slave_pins[] = {
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,control), "%s.%s.%s.ain-%d-control" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,status), "%s.%s.%s.ain-%d-status" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data0_os), "%s.%s.%s.ain-%d-tx_data0" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data1), "%s.%s.%s.ain-%d-tx_data1" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data2), "%s.%s.%s.ain-%d-tx_data2" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data3), "%s.%s.%s.ain-%d-tx_data3" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data4), "%s.%s.%s.ain-%d-tx_data4" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data5), "%s.%s.%s.ain-%d-tx_data5" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data6), "%s.%s.%s.ain-%d-tx_data6" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data7), "%s.%s.%s.ain-%d-tx_data7" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data8), "%s.%s.%s.ain-%d-tx_data8" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data9), "%s.%s.%s.ain-%d-tx_data9" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data10), "%s.%s.%s.ain-%d-tx_data10" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data11), "%s.%s.%s.ain-%d-tx_data11" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data12), "%s.%s.%s.ain-%d-tx_data12" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data13), "%s.%s.%s.ain-%d-tx_data13" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data14), "%s.%s.%s.ain-%d-tx_data14" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data15), "%s.%s.%s.ain-%d-tx_data15" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data16), "%s.%s.%s.ain-%d-tx_data16" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data17), "%s.%s.%s.ain-%d-tx_data17" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data18), "%s.%s.%s.ain-%d-tx_data18" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data19), "%s.%s.%s.ain-%d-tx_data19" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data20), "%s.%s.%s.ain-%d-tx_data20" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,tx_data21), "%s.%s.%s.ain-%d-tx_data21" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data0_os), "%s.%s.%s.ain-%d-rx_data0" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data1), "%s.%s.%s.ain-%d-rx_data1" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data2), "%s.%s.%s.ain-%d-rx_data2" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data3), "%s.%s.%s.ain-%d-rx_data3" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data4), "%s.%s.%s.ain-%d-rx_data4" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data5), "%s.%s.%s.ain-%d-rx_data5" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data6), "%s.%s.%s.ain-%d-rx_data6" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data7), "%s.%s.%s.ain-%d-rx_data7" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data8), "%s.%s.%s.ain-%d-rx_data8" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data9), "%s.%s.%s.ain-%d-rx_data9" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data10), "%s.%s.%s.ain-%d-rx_data10" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data11), "%s.%s.%s.ain-%d-rx_data11" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data12), "%s.%s.%s.ain-%d-rx_data12" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data13), "%s.%s.%s.ain-%d-rx_data13" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data14), "%s.%s.%s.ain-%d-rx_data14" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data15), "%s.%s.%s.ain-%d-rx_data15" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data16), "%s.%s.%s.ain-%d-rx_data16" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data17), "%s.%s.%s.ain-%d-rx_data17" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data18), "%s.%s.%s.ain-%d-rx_data18" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data19), "%s.%s.%s.ain-%d-rx_data19" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data20), "%s.%s.%s.ain-%d-rx_data20" },
  { HAL_U32, HAL_IN, offsetof(lcec_el6021_data_t ,rx_data21), "%s.%s.%s.ain-%d-rx_data21" },
 // { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,value), "%s.%s.%s.ain-%d-value" },
  { HAL_TYPE_UNSPECIFIED, HAL_DIR_UNSPECIFIED, -1, NULL }
};


//RX, Inputs
static ec_pdo_entry_info_t lcec_el6021_in[] = {
    {0x3103, 0x01, 16}, // status
    {0x3103, 0x02, 8},  // data0
    {0x3103, 0x03, 8},  // data1
    {0x3103, 0x04, 8},  // data2
    {0x3103, 0x05, 8},  // data3
    {0x3103, 0x06, 8},  // data4
    {0x3103, 0x07, 8},  // data5
    {0x3103, 0x08, 8},  // data6
    {0x3103, 0x09, 8},  // data7
    {0x3103, 0x0A, 8},  // data8
    {0x3103, 0x0B, 8},  // data9
    {0x3103, 0x0C, 8}, //  data10
    {0x3103, 0x0D, 8}, //  data11
    {0x3103, 0x0E, 8}, //  data12
    {0x3103, 0x0F, 8}, //  data13
    {0x3103, 0x10, 8}, //  data14
    {0x3103, 0x11, 8}, //  data15
    {0x3103, 0x12, 8},  //  data16
    {0x3103, 0x13, 8}, //  data17
    {0x3103, 0x14, 8}, //  data18
    {0x3103, 0x15, 8}, //  data19
    {0x3103, 0x16, 8}, //  data20
    {0x3103, 0x17, 8} //  data21  
};

// TX, Outputs
static ec_pdo_entry_info_t lcec_el6021_out[] = {
    {0x3003, 0x01, 16}, // control
    {0x3003, 0x02, 8},  // data0
    {0x3003, 0x03, 8},  // data1
    {0x3003, 0x04, 8},  // data2
    {0x3003, 0x05, 8},  // data3
    {0x3003, 0x06, 8},  // data4
    {0x3003, 0x07, 8},  // data5
    {0x3003, 0x08, 8},  // data6
    {0x3003, 0x09, 8},  // data7
    {0x3003, 0x0A, 8},  // data8
    {0x3003, 0x0B, 8},  // data9
    {0x3003, 0x0C, 8}, //  data10
    {0x3003, 0x0D, 8}, //  data11
    {0x3003, 0x0E, 8}, //  data12
    {0x3003, 0x0F, 8}, //  data13
    {0x3003, 0x10, 8}, //  data14
    {0x3003, 0x11, 8}, //  data15
    {0x3003, 0x12, 8},  //  data16
    {0x3003, 0x13, 8}, //  data17
    {0x3003, 0x14, 8}, //  data18
    {0x3003, 0x15, 8}, //  data19
    {0x3003, 0x16, 8}, //  data20
    {0x3003, 0x17, 8} //  data21
};

static ec_pdo_info_t lcec_el6021_pdos_in[] = {
    {0x1A02, 18, lcec_el6021_in}
};

static ec_pdo_info_t lcec_el6021_pdos_out[] = {
    {0x1602, 18, lcec_el6021_out}
};

//Syncmanager info 
static ec_sync_info_t lcec_el6021_syncs[] = {
    {0, EC_DIR_OUTPUT, 0, NULL},
    {1, EC_DIR_INPUT,  0, NULL},
    {2, EC_DIR_OUTPUT, 1, lcec_el6021_pdos_out, EC_WD_DISABLE},
    {3, EC_DIR_INPUT,  1, lcec_el6021_pdos_in, EC_WD_DISABLE},
    {0xff}
};

void lcec_el6021_read(struct lcec_slave *slave, long period);
void lcec_el6021_write(struct lcec_slave *slave, long period);

int lcec_el6021_init(int comp_id, struct lcec_slave *slave, ec_pdo_entry_reg_t *pdo_entry_regs) {
  lcec_master_t *master = slave->master;
  lcec_el6021_data_t *hal_data;
  int err;

  // initialize callbacks
  slave->proc_read = lcec_el6021_read;
  slave->proc_write = lcec_el6021_write;

  // alloc hal memory
  if ((hal_data = hal_malloc(sizeof(lcec_el6021_data_t))) == NULL) {
    rtapi_print_msg(RTAPI_MSG_ERR, LCEC_MSG_PFX "hal_malloc() for slave %s.%s failed\n", master->name, slave->name);
    return -EIO;
  }
  memset(hal_data, 0, sizeof(lcec_el6021_data_t));
  slave->hal_data = hal_data;

  // initializer sync info
  slave->sync_info = lcec_el6021_syncs;

  // initialize PDO entries

  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x3003, 0x01, &hal_data->control_os, NULL);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x3103, 0x01, &hal_data->status_os, NULL);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x3003, 0x02, &hal_data->tx_data0_os, NULL);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x3103, 0x02, &hal_data->rx_data0_os, NULL);
//  for(i=0;i<22;i++){
//    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x3103, (0x02 +1) , NULL, NULL)
//    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x3003, (0x02 +1) , NULL, NULL)
//  }

    // export pins
  if ((err = lcec_pin_newf_list(hal_data, slave_pins, LCEC_MODULE_NAME, master->name, slave->name)) != 0) {
    return err;
  }

  return 0;
}

void lcec_el6021_read(struct lcec_slave *slave, long period) {
  lcec_master_t *master = slave->master;
  lcec_el6021_data_t *hal_data = (lcec_el6021_data_t *) slave->hal_data;
  uint8_t *pd = master->process_data;
  uint8_t state;
  int16_t value;

  // wait for slave to be operational
  if (!slave->state.operational) {
    return;
  }

    // update state
  *(hal_data->status)=EC_READ_U16(&pd[hal_data->status_os]);
    // update values
  *(hal_data->rx_data0)=EC_READ_U8(&pd[hal_data->rx_data0_os]);
//  *(hal_data->rx_data1)=EC_READ_U8(&pd[hal_data->rx_data1]);
//  *(hal_data->rx_data2)=EC_READ_U8(&pd[hal_data->rx_data2]);
//  *(hal_data->rx_data3)=EC_READ_U8(&pd[hal_data->rx_data3]);
//  *(hal_data->rx_data4)=EC_READ_U8(&pd[hal_data->rx_data4]);
//  *(hal_data->rx_data5)=EC_READ_U8(&pd[hal_data->rx_data5]);
//  *(hal_data->rx_data6)=EC_READ_U8(&pd[hal_data->rx_data6]);
//  *(hal_data->rx_data7)=EC_READ_U8(&pd[hal_data->rx_data7]);
//  *(hal_data->rx_data8)=EC_READ_U8(&pd[hal_data->rx_data8]);
//  *(hal_data->rx_data9)=EC_READ_U8(&pd[hal_data->rx_data9]);
//  *(hal_data->rx_data10)=EC_READ_U8(&pd[hal_data->rx_data10]);
//  *(hal_data->rx_data11)=EC_READ_U8(&pd[hal_data->rx_data11]);
//  *(hal_data->rx_data12)=EC_READ_U8(&pd[hal_data->rx_data12]);
//  *(hal_data->rx_data13)=EC_READ_U8(&pd[hal_data->rx_data13]);
//  *(hal_data->rx_data14)=EC_READ_U8(&pd[hal_data->rx_data14]);
//  *(hal_data->rx_data15)=EC_READ_U8(&pd[hal_data->rx_data15]);
//  *(hal_data->rx_data16)=EC_READ_U8(&pd[hal_data->rx_data16]);
//  *(hal_data->rx_data17)=EC_READ_U8(&pd[hal_data->rx_data17]);
//  *(hal_data->rx_data18)=EC_READ_U8(&pd[hal_data->rx_data19]);
//  *(hal_data->rx_data19)=EC_READ_U8(&pd[hal_data->rx_data19]);
//  *(hal_data->rx_data20)=EC_READ_U8(&pd[hal_data->rx_data20]);
//  *(hal_data->rx_data21)=EC_READ_U8(&pd[hal_data->rx_data21]);

}

void lcec_el6021_write(struct lcec_slave *slave, long period) {
  lcec_master_t *master = slave->master;
  lcec_el6021_data_t *hal_data = (lcec_el6021_data_t *) slave->hal_data;
  uint8_t *pd = master->process_data;
    // update state
  EC_WRITE_U16(&pd[hal_data->control_os],*( hal_data->control));
    // update values
  EC_WRITE_U8(&pd[hal_data->tx_data0_os],*( hal_data->tx_data0));
//  *(hal_data->tx_data1)=EC_WRITE_U8(&pd[hal_data->tx_data1]);
//  *(hal_data->tx_data2)=EC_WRITE_U8(&pd[hal_data->tx_data2]);
//  *(hal_data->tx_data3)=EC_WRITE_U8(&pd[hal_data->tx_data3]);
//  *(hal_data->tx_data4)=EC_WRITE_U8(&pd[hal_data->tx_data4]);
//  *(hal_data->tx_data5)=EC_WRITE_U8(&pd[hal_data->tx_data5]);
//  *(hal_data->tx_data6)=EC_WRITE_U8(&pd[hal_data->tx_data6]);
//  *(hal_data->tx_data7)=EC_WRITE_U8(&pd[hal_data->tx_data7]);
//  *(hal_data->tx_data8)=EC_WRITE_U8(&pd[hal_data->tx_data8]);
//  *(hal_data->tx_data9)=EC_WRITE_U8(&pd[hal_data->tx_data9]);
//  *(hal_data->tx_data10)=EC_WRITE_U8(&pd[hal_data->tx_data10]);
//  *(hal_data->tx_data11)=EC_WRITE_U8(&pd[hal_data->tx_data11]);
//  *(hal_data->tx_data12)=EC_WRITE_U8(&pd[hal_data->tx_data12]);
//  *(hal_data->tx_data13)=EC_WRITE_U8(&pd[hal_data->tx_data13]);
//  *(hal_data->tx_data14)=EC_WRITE_U8(&pd[hal_data->tx_data14]);
//  *(hal_data->tx_data15)=EC_WRITE_U8(&pd[hal_data->tx_data15]);
//  *(hal_data->tx_data16)=EC_WRITE_U8(&pd[hal_data->tx_data16]);
//  *(hal_data->tx_data17)=EC_WRITE_U8(&pd[hal_data->tx_data17]);
//  *(hal_data->tx_data18)=EC_WRITE_U8(&pd[hal_data->tx_data19]);
//  *(hal_data->tx_data19)=EC_WRITE_U8(&pd[hal_data->tx_data19]);
//  *(hal_data->tx_data20)=EC_WRITE_U8(&pd[hal_data->tx_data20]);
//  *(hal_data->tx_data21)=EC_WRITE_U8(&pd[hal_data->tx_data21]);

}


