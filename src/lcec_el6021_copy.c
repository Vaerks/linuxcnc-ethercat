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
  
  hal_bit_t *transmit_ack;
  hal_bit_t *transmit_req;
  hal_bit_t *receive_ack;
  hal_bit_t *receive_req;
  hal_bit_t *init_ack;
  hal_bit_t *init_req;
  hal_bit_t *send_continues;
  hal_bit_t *buffer_full;
  hal_bit_t *parity_error;
  hal_bit_t *framing_error;
  hal_bit_t *overrun_error;
  hal_u32_t *data_length;
  hal_u32_t *data0;
  hal_u32_t *data1;
  hal_u32_t *data2;
  hal_u32_t *data3;
  hal_u32_t *data4;
  hal_u32_t *data5;
  hal_u32_t *data6;
  hal_u32_t *data7;
  hal_u32_t *data8;
  hal_u32_t *data9;
  hal_u32_t *data10;
  hal_u32_t *data11;
  hal_u32_t *data12;
  hal_u32_t *data13;
  hal_u32_t *data14;
  hal_u32_t *data15;
  hal_u32_t *data16;
  hal_u32_t *data17;
  hal_u32_t *data18;
  hal_u32_t *data19;
  hal_u32_t *data20;
  hal_u32_t *data21;
 // unsigned char data[];
  unsigned int tx_ack_os;
  unsigned int tx_ack_bp;
  unsigned int tx_req_os;
  unsigned int tx_req_bp;
  unsigned int rx_ack_os;
  unsigned int rx_ack_bp;
  unsigned int rx_req_os;
  unsigned int rx_req_bp;
  unsigned int init_ack_os;
  unsigned int init_ack_bp;
  unsigned int init_req_os;
  unsigned int init_req_bp;
  unsigned int buff_full_os;
  unsigned int buff_full_bp;
  unsigned int parity_err_os;
  unsigned int parity_err_bp;
  unsigned int frame_err_os;
  unsigned int frame_err_bp;
  unsigned int ovrrun_err_os;
  unsigned int ovrrun_err_bp;
  unsigned int send_continues_os;
  unsigned int send_continues_bp;
} lcec_el6021_data_t;


static const lcec_pindesc_t slave_pins[] = {
  { HAL_BIT, HAL_OUT, offsetof(lcec_el6021_data_t ,transmit_ack), "%s.%s.%s.ain-%d-transmit_ack" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el6021_data_t ,transmit_req), "%s.%s.%s.ain-%d-transmit_req" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el6021_data_t ,receive_ack), "%s.%s.%s.ain-%d-receive_ack" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el6021_data_t ,receive_req), "%s.%s.%s.ain-%d-receive_req" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el6021_data_t ,send_continues), "%s.%s.%s.ain-%d-send_continues" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el6021_data_t ,buffer_full), "%s.%s.%s.ain-%d-buffer_full" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el6021_data_t ,parity_error), "%s.%s.%s.ain-%d-parity_error" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el6021_data_t ,overrun_error), "%s.%s.%s.ain-%d-overrun_error" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,data_length), "%s.%s.%s.ain-%d-data_length" },
 // { HAL_U32, HAL_OUT, offsetof(lcec_el6021_data_t ,value), "%s.%s.%s.ain-%d-value" },
  { HAL_TYPE_UNSPECIFIED, HAL_DIR_UNSPECIFIED, -1, NULL }
};


//RX, Inputs
static ec_pdo_entry_info_t lcec_el6021_in[] = {
    {0x6000, 0x01, 1}, // transmit accepted
    {0x6000, 0x02, 1}, // receive request
    {0x6000, 0x03, 1}, // Init accepted
    {0x6000, 0x04, 1}, // buffer full
    {0x6000, 0x05, 1}, // parity error
    {0x6000, 0x06, 1}, // Framing error
    {0x6000, 0x07, 1}, // overrun error
    {0x0000, 0x00, 1}, // Gap
    {0x6000, 0x09, 8}, // data length
    {0x6000, 0x11, 8},  // data0
    {0x6000, 0x12, 8},  // data1
    {0x6000, 0x13, 8},  // data2
    {0x6000, 0x14, 8},  // data3
    {0x6000, 0x15, 8},  // data4
    {0x6000, 0x16, 8},  // data5
    {0x6000, 0x17, 8},  // data6
    {0x6000, 0x18, 8},  // data7
    {0x6000, 0x19, 8},  // data8
    {0x6000, 0x1A, 8},  // data9
    {0x6000, 0x1B, 8}, //  data10
    {0x6000, 0x1C, 8}, //  data11
    {0x6000, 0x1D, 8}, //  data12
    {0x6000, 0x1E, 8}, //  data13
    {0x6000, 0x1F, 8}, //  data14
    {0x6000, 0x20, 8}, //  data15
    {0x6000, 0x21, 8},  //  data16
    {0x6000, 0x22, 8}, //  data17
    {0x6000, 0x23, 8}, //  data18
    {0x6000, 0x24, 8}, //  data19
    {0x6000, 0x25, 8}, //  data20
    {0x6000, 0x26, 8} //  data21  
};

// TX, Outputs
static ec_pdo_entry_info_t lcec_el6021_out[] = {
    {0x7000, 0x01, 1}, // transmit request
    {0x7000, 0x02, 1}, // receive accepted
    {0x7000, 0x03, 1}, // Init requested
    {0x7000, 0x04, 1}, // Send continues
    {0x0000, 0x00, 1}, // Gap
    {0x7000, 0x09, 8}, // data length
    {0x7000, 0x11, 8}, // data0
    {0x7000, 0x12, 8}, // data1
    {0x7000, 0x13, 8}, // data2
    {0x7000, 0x14, 8}, // data3
    {0x7000, 0x15, 8}, // data4
    {0x7000, 0x16, 8}, // data5
    {0x7000, 0x17, 8}, // data6
    {0x7000, 0x18, 8}, // data7
    {0x7000, 0x19, 8}, // data8
    {0x7000, 0x1A, 8}, // data9
    {0x7000, 0x1B, 8}, //  data10
    {0x7000, 0x1C, 8}, //  data11
    {0x7000, 0x1D, 8}, //  data12
    {0x7000, 0x1E, 8}, //  data13
    {0x7000, 0x1F, 8}, //  data14
    {0x7000, 0x20, 8}, //  data15
    {0x7000, 0x21, 8},  //  data16
    {0x7000, 0x22, 8}, //  data17
    {0x7000, 0x23, 8}, //  data18
    {0x7000, 0x24, 8}, //  data19
    {0x7000, 0x25, 8}, //  data20
    {0x7000, 0x26, 8} //  data21  
};

static ec_pdo_info_t lcec_el6021_pdos_in[] = {
    {0x1A04, 28, lcec_el6021_in}
};

static ec_pdo_info_t lcec_el6021_pdos_out[] = {
    {0x1604, 28, lcec_el6021_out}
};

//Syncmanager info 
static ec_sync_info_t lcec_el6021_syncs[] = {
    {0, EC_DIR_OUTPUT, 0, NULL},
    {1, EC_DIR_INPUT,  0, NULL},
    {2, EC_DIR_OUTPUT, 1, lcec_el6021_pdos_out},
    {3, EC_DIR_INPUT,  1, lcec_el6021_pdos_in},
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
  //RX
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000, 0x01, &hal_data->tx_ack_os, &hal_data->tx_ack_bp);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000, 0x02, &hal_data->rx_req_os, &hal_data->rx_req_bp);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000, 0x03, &hal_data->init_ack_os, &hal_data->init_ack_bp);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000, 0x04, &hal_data->buff_full_os, &hal_data->buff_full_bp);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000, 0x05, &hal_data->parity_err_os, &hal_data->parity_err_bp);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000, 0x06, &hal_data->frame_err_os, &hal_data->frame_err_bp);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000, 0x07, &hal_data->ovrrun_err_os, &hal_data->ovrrun_err_bp);
  //TX
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000, 0x01, &hal_data->tx_req_os, &hal_data->tx_req_bp);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000, 0x02, &hal_data->rx_ack_os, &hal_data->rx_ack_bp);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000, 0x03, &hal_data->init_req_os, &hal_data->init_req_bp);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6000, 0x04, &hal_data->send_continues_os, &hal_data->send_continues_bp);

    // export pins
  if ((err = lcec_pin_newf_list(hal_data, slave_pins, LCEC_MODULE_NAME, master->name, slave->name)) != 0) {
    return err;
  }

    // initialize pins
//    *(chan->transmit_ack) = 0;
//    *(chan->transmit_req) = 0;
//    *(chan->recieve_ack) = 0;   
//    *(chan->recieve_req) = 0;
//    *(chan->init_ack) = 0;
//    *(chan->init_req) = 0;
//    *(chan->buffer_full) = 0;
//    *(chan->parity_error) = 0;
//    *(chan->framing_error) = 0;
//    *(chan->overrun_error) = 0;
//    *(chan->send_continues) = 0;
//    *(chan->data_length) = 0;
//    *(chan->send_continues) = 0;

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
  *(hal_data->transmit_ack) = EC_READ_BIT(&pd[hal_data->tx_ack_os], hal_data->tx_ack_bp);
  *(hal_data->receive_req) =  EC_READ_BIT(&pd[hal_data->rx_req_os], hal_data->rx_req_bp);
  *(hal_data->init_ack) =  EC_READ_BIT(&pd[hal_data->init_ack_os], hal_data->init_ack_bp);
  *(hal_data->buffer_full) =  EC_READ_BIT(&pd[hal_data->buff_full_os], hal_data->buff_full_bp);
  *(hal_data->parity_error) =  EC_READ_BIT(&pd[hal_data->parity_err_os], hal_data->parity_err_bp);
  *(hal_data->framing_error) =  EC_READ_BIT(&pd[hal_data->frame_err_os], hal_data->frame_err_bp);
  *(hal_data->overrun_error) =  EC_READ_BIT(&pd[hal_data->ovrrun_err_os], hal_data->ovrrun_err_bp);

    // update value

}  

void lcec_el6021_write(struct lcec_slave *slave, long period) {
  lcec_master_t *master = slave->master;
  lcec_el6021_data_t *hal_data = (lcec_el6021_data_t *) slave->hal_data;
  uint8_t *pd = master->process_data;


  EC_WRITE_BIT(&pd[hal_data->tx_req_os], hal_data->tx_req_bp, *(hal_data->transmit_req));
  EC_WRITE_BIT(&pd[hal_data->rx_ack_os], hal_data->rx_ack_bp, *(hal_data->receive_ack));
  EC_WRITE_BIT(&pd[hal_data->init_req_os], hal_data->init_req_bp, *(hal_data->init_req));
  EC_WRITE_BIT(&pd[hal_data->send_continues_os], hal_data->send_continues_bp, *(hal_data->send_continues));
 

}


