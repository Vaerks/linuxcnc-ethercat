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

//Here is a detailed commented driver,
// The device is a Beckhoff EL5001 SSI encoder terminal
// Generally the device can be cofigure en two ways, therefore many things is commented to make the driver easily changeable

#include "lcec.h"//These must be included 
#include "lcec_el5001.h" 

typedef struct {            // Here a struct with the data variables 
  // hal_bit_t *data_error;  //The hal variables are the ones where the data is finally cast and will be sent to hal.
  // hal_bit_t *frame_error; // The datatypes are limited to linuxcncs datatypes
  // hal_bit_t *power_failure;
  // hal_bit_t *sync_error;
  // hal_u32_t *cval;
  hal_u32_t *status;
 // hal_u32_t *raw_val;
  hal_u32_t *val;
  // unsigned int derror_pdo_os; // These are variables need to be created for each of the data that you wihs to show in hal
  // unsigned int derror_pdo_bp; // The _os stands for offset and the _bp stands for bit position
  // unsigned int ferror_pdo_os; // bitpostition are only needed for bit datatypes
  // unsigned int ferror_pdo_bp;
  // unsigned int pfail_pdo_os;
  // unsigned int pfail_pdo_bp;
  // unsigned int serror_pdo_os;
  // unsigned int serror_pdo_bp;
  unsigned int status_pdo_os;
  //unsigned int status_pdo_bp;
  // unsigned int cval_pdo_os;
  unsigned int val_pdo_os;
 // unsigned int val_pdo_bp;
} lcec_el5001_data_t; 

static const lcec_pindesc_t slave_pins[] = { //The name of hal pins and mapping of variables to hal is done
//{HAL_DATATYPE, DIRECTION, offsetof(lcec_elxxxx_data_t, VARIABLE), name of hal pin}
  // { HAL_BIT, HAL_OUT, offsetof(lcec_el5001_data_t ,data_error), "%s.%s.%s.ain-%d-data-error" },
  // { HAL_BIT, HAL_OUT, offsetof(lcec_el5001_data_t ,frame_error), "%s.%s.%s.ain-%d-frame-error" },
  // { HAL_BIT, HAL_OUT, offsetof(lcec_el5001_data_t ,power_failure), "%s.%s.%s.ain-%d-power-failure" },
  // { HAL_BIT, HAL_OUT, offsetof(lcec_el5001_data_t ,sync_error), "%s.%s.%s.ain-%d-sync_error" },
  // { HAL_U32, HAL_OUT, offsetof(lcec_el5001_data_t ,cval), "%s.%s.%s.ain-%d-counterval" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el5001_data_t ,status), "%s.%s.%s.ain-%d-status" },
  { HAL_U32, HAL_OUT, offsetof(lcec_el5001_data_t ,val), "%s.%s.%s.ain-%d-val" },
  { HAL_TYPE_UNSPECIFIED, HAL_DIR_UNSPECIFIED, -1, NULL } // Always this at the end.
};

//-----------------PDO ENTRIES-------------------------
//These are defined in the ec_pdo_entry_info_t struct
// Can befound in twincat or with ethercat cstruct or device description file

// static ec_pdo_entry_info_t lcec_el5001_ssi_inputs[] = {
//     {0x6010, 0x01, 1}, // Data error
//     {0x6010, 0x02, 1},  // Frame error
//     {0x6010, 0x03, 1}, // Power failure
//     {0x0000, 0x00, 10},  // GAP
//     {0x6010, 0x0e, 1}, // Sync error
//    // {0x1c32, 0x20 , 1},
//     // {0x1801, 0x07, 1},  // status state
//     // {0x1801, 0x09, 1}, // status toggle
  
//     {0x6010, 0x0F, 1},  // status state
//     {0x6010, 0x10, 1}, // status toggle
//     {0x6010, 0x11, 32},  // value  
// };

static ec_pdo_entry_info_t lcec_el5001_channel1[] = {
  //{ENTRY, SUBINDEX, AMOUNT OF BITS}
    {0x3101, 0x01, 8}, // status  
    {0x3101, 0x02, 32},  // data
    // {0x6000, 0x01, 8}, // status
    // {0x6000, 0x02, 32},  // data
};


static ec_pdo_info_t lcec_el5001_pdos_in[] = {
  //{PDO,AMOUNT OF ENTRIES, STRUCT}
  {0x1A00, 2, lcec_el5001_channel1},
  // {0x1A01, 8, lcec_el5001_ssi_inputs},
};

// Sync managers 
static ec_sync_info_t lcec_el5001_syncs[] = {
    {0, EC_DIR_OUTPUT, 0, NULL}, // Mailbox output
    {1, EC_DIR_INPUT,  0, NULL}, // Mailbox input
    {2, EC_DIR_OUTPUT, 0, NULL}, // PDO outputs
    {3, EC_DIR_INPUT,  1, lcec_el5001_pdos_in}, //PDO inputs
    //{INDEX, DIRECTION, AMOUNT OF PDOS, PDOS}
    {0xff} // HAS to be terminated
};

void lcec_el5001_read(struct lcec_slave *slave, long period); // Function declaration
// if a write method exists  it must also be declared here, i hold the same arguments

int lcec_el5001_init(int comp_id, struct lcec_slave *slave, ec_pdo_entry_reg_t *pdo_entry_regs) { //Obligatory method and arguments
  lcec_master_t *master = slave->master;
  lcec_el5001_data_t *hal_data;
  int i;
  int err;

  // initialize callbacks
  slave->proc_read = lcec_el5001_read;

  // alloc hal memory
  if ((hal_data = hal_malloc(sizeof(lcec_el5001_data_t))) == NULL) {
    rtapi_print_msg(RTAPI_MSG_ERR, LCEC_MSG_PFX "hal_malloc() for slave %s.%s failed\n", master->name, slave->name);
    return -EIO;
  }
  memset(hal_data, 0, sizeof(lcec_el5001_data_t));
  slave->hal_data = hal_data;

  // initializer sync info
  slave->sync_info = lcec_el5001_syncs;

  // initialize PDO entries
  // LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6010, 0x01, &hal_data->derror_pdo_os, &hal_data->derror_pdo_bp);
  // LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6010, 0x02, &hal_data->ferror_pdo_os, &hal_data->ferror_pdo_bp);
  // LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6010, 0x03, &hal_data->pfail_pdo_os, &hal_data->pfail_pdo_bp);
  // LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6010, 0x0e, &hal_data->serror_pdo_os, &hal_data->serror_pdo_bp);
  // //LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x1C32, 0x20, &hal_data->serror_pdo_os, &hal_data->serror_pdo_bp); 
  // LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6010, 0x11, &hal_data->cval_pdo_os, NULL);
  // This function must be called for every pdo entry you wish to access
  // -------------------------------obligatory---------------------,PDOENTRY,SUBINDEX,OFFSET var, Bitpos var);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x3101, 0x01, &hal_data->status_pdo_os, NULL);
  LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x3101, 0x02, &hal_data->val_pdo_os, NULL); 

  // export pins
  if ((err = lcec_pin_newf_list(hal_data, slave_pins, LCEC_MODULE_NAME, master->name, slave->name, i)) != 0) {
      return err;
  }
  // initialize pins
   //Here writable pins can be initialized

  return 0;
}

void lcec_el5001_read(struct lcec_slave *slave, long period) {
  lcec_master_t *master = slave->master;
  lcec_el5001_data_t *hal_data = (lcec_el5001_data_t *) slave->hal_data;
  uint8_t *pd = master->process_data;
  int i;
  uint8_t state;
  int32_t value;
  int32_t cvalue;
  uint8_t status;
  // wait for slave to be operational
  if (!slave->state.operational) {
    return;
  }

  // check inputs

    // update state
  // *(hal_data->data_error) = EC_READ_BIT(&pd[hal_data->derror_pdo_os], hal_data->derror_pdo_bp);
  // *(hal_data->frame_error) = EC_READ_BIT(&pd[hal_data->ferror_pdo_os], hal_data->ferror_pdo_bp);
  // *(hal_data->power_failure) =  EC_READ_BIT(&pd[hal_data->pfail_pdo_os], hal_data->pfail_pdo_bp);
  // *(hal_data->sync_error) =  EC_READ_BIT(&pd[hal_data->serror_pdo_os], hal_data->serror_pdo_bp);
  //  cvalue = EC_READ_U32(&pd[hal_data->cval_pdo_os]);
  // *(hal_data->cval) = cvalue;
  // Find the correct read or write 
  status = EC_READ_U32(&pd[hal_data->status_pdo_os]);
  *(hal_data->status) = status;  
  // update value
  value = EC_READ_U32(&pd[hal_data->val_pdo_os]);
  *(hal_data->val) = value;
  //*(hal_data->val) = value / 10000;

}


