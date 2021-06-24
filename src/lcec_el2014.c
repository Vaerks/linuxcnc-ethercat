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
#include "lcec_el2014.h"

typedef struct {

  hal_bit_t *overtemp;
  hal_bit_t *overcurrent;
  hal_bit_t *openload;
  hal_bit_t *shortcircuit;
  hal_bit_t *output;


  unsigned int ovt_pdo_os;
  unsigned int ovt_pdo_bp;
  unsigned int ovc_pdo_os;
  unsigned int ovc_pdo_bp;
  unsigned int opl_pdo_os;
  unsigned int opl_pdo_bp;  
  unsigned int shc_pdo_os;
  unsigned int shc_pdo_bp;
  unsigned int out_pdo_os;
  unsigned int out_pdo_bp;
} lcec_el2014_chan_t;

typedef struct{
  hal_bit_t *commonfault;
  hal_bit_t *overtempdev;
  hal_bit_t *undervoltage;
  hal_bit_t *missingvoltage;

  unsigned int cf_pdo_os;
  unsigned int cf_pdo_bp;
  unsigned int ovtd_pdo_os;
  unsigned int ovtd_pdo_bp;
  unsigned int udv_pdo_os;
  unsigned int udv_pdo_bp;
  unsigned int misv_pdo_os;
  unsigned int misv_pdo_bp;
} lcec_el2014_diag_t;

static const lcec_pindesc_t slave_pins[] = {
   
  { HAL_BIT, HAL_OUT, offsetof(lcec_el2014_chan_t ,overtemp), "%s.%s.%s.in.%d.overtemp" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el2014_chan_t ,overcurrent), "%s.%s.%s.in.%d.overcurrent" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el2014_chan_t ,openload), "%s.%s.%s.in.%d.openload" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el2014_chan_t ,shortcircuit), "%s.%s.%s.in.%d.shortcircuit" },
  { HAL_BIT, HAL_IN, offsetof(lcec_el2014_chan_t ,output), "%s.%s.%s.out.%d.output" },
  { HAL_TYPE_UNSPECIFIED, HAL_DIR_UNSPECIFIED, -1, NULL }
};

static const lcec_pindesc_t diag_pins[] = {
  { HAL_BIT, HAL_OUT, offsetof(lcec_el2014_diag_t ,commonfault), "%s.%s.%s.diag.%d.commonfault" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el2014_diag_t ,overtempdev), "%s.%s.%s.diag.%d.overtempdev" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el2014_diag_t ,undervoltage), "%s.%s.%s.diag.%d.undervoltage" },
  { HAL_BIT, HAL_OUT, offsetof(lcec_el2014_diag_t ,missingvoltage), "%s.%s.%s.diag.%d.missingvoltage" },
  { HAL_TYPE_UNSPECIFIED, HAL_DIR_UNSPECIFIED, -1, NULL }
};

typedef struct {
  lcec_el2014_chan_t chans[LCEC_EL2014_CHANS];
  lcec_el2014_diag_t diags[LCEC_EL2014_DIAGS];
} lcec_el2014_data_t;

static ec_pdo_entry_info_t lcec_el2014_pdo_entries[] = {
    {0x7000, 0x01, 1}, /* Output */
    {0x7010, 0x01, 1}, /* Output */
    {0x7020, 0x01, 1}, /* Output */
    {0x7030, 0x01, 1}, /* Output */
    {0x0000, 0x00, 4}, /* Gap */
    {0x6001, 0x01, 1}, /* Overtemperature */
    {0x6001, 0x02, 1}, /* Open Load */
    {0x6001, 0x03, 1}, /* Overcurrent */
    {0x6001, 0x04, 1}, /* Short Circuit to 24V */
    {0x6011, 0x01, 1}, /* Overtemperature */
    {0x6011, 0x02, 1}, /* Open Load */
    {0x6011, 0x03, 1}, /* Overcurrent */
    {0x6011, 0x04, 1}, /* Short Circuit to 24V */
    {0x6021, 0x01, 1}, /* Overtemperature */
    {0x6021, 0x02, 1}, /* Open Load */
    {0x6021, 0x03, 1}, /* Overcurrent */
    {0x6021, 0x04, 1}, /* Short Circuit to 24V */
    {0x6031, 0x01, 1}, /* Overtemperature */
    {0x6031, 0x02, 1}, /* Open Load */
    {0x6031, 0x03, 1}, /* Overcurrent */
    {0x6031, 0x04, 1}, /* Short Circuit to 24V */
    {0xf600, 0x11, 1}, /* Common Fault */
    {0xf600, 0x12, 1}, /* Overtemperature Device */
    {0xf600, 0x13, 1}, /* Undervoltage */
    {0xf600, 0x14, 1}, /* Missing Voltage */
    {0x0000, 0x00, 4}, /* Gap */
};


static ec_pdo_info_t lcec_el2014_pdos[] = {
    {0x1600, 5, lcec_el2014_pdo_entries + 0}, /* DIG RxPDO-Map Outputs */
    {0x1a00, 16, lcec_el2014_pdo_entries + 5}, /* DIG TxPDO-Map Diag Inputs */
    {0x1a02, 5, lcec_el2014_pdo_entries + 21}, /* DIG TxPDO-Map Inputs Device */
};

static ec_sync_info_t lcec_el2014_syncs[] = {
    {0, EC_DIR_OUTPUT, 0, NULL, EC_WD_DISABLE},
    {1, EC_DIR_INPUT, 0, NULL, EC_WD_DISABLE},
    {2, EC_DIR_OUTPUT, 1, lcec_el2014_pdos + 0, EC_WD_DISABLE},
    {3, EC_DIR_INPUT, 2, lcec_el2014_pdos + 1, EC_WD_DISABLE},
    {0xff}
};

void lcec_el2014_read(struct lcec_slave *slave, long period);
void lcec_el2014_write(struct lcec_slave *slave, long period);

int lcec_el2014_init(int comp_id, struct lcec_slave *slave, ec_pdo_entry_reg_t *pdo_entry_regs) {
  lcec_master_t *master = slave->master;
  lcec_el2014_data_t *hal_data;
  lcec_el2014_chan_t *chan;
  lcec_el2014_diag_t *diag;
  int i;
  int err;

  // initialize callbacks
  slave->proc_read = lcec_el2014_read;
  slave->proc_write = lcec_el2014_write;
  // alloc hal memory
  if ((hal_data = hal_malloc(sizeof(lcec_el2014_data_t))) == NULL) {
    rtapi_print_msg(RTAPI_MSG_ERR, LCEC_MSG_PFX "hal_malloc() for slave %s.%s failed\n", master->name, slave->name);
    return -EIO;
  }
  memset(hal_data, 0, sizeof(lcec_el2014_data_t));
  slave->hal_data = hal_data;

  // initializer sync info
  slave->sync_info = lcec_el2014_syncs;

  for (i=0; i<LCEC_EL2014_DIAGS; i++) {
    diag = &hal_data->diags[i];
    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0xf600, 0x11, &diag->cf_pdo_os, &diag->cf_pdo_bp);
    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0xf600, 0x12, &diag->ovtd_pdo_os, &diag->ovtd_pdo_bp);
    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0xf600, 0x13, &diag->udv_pdo_os, &diag->udv_pdo_bp);
    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0xf600, 0x14, &diag->misv_pdo_os, &diag->misv_pdo_bp);
        // export pins
    if ((err = lcec_pin_newf_list(diag, diag_pins, LCEC_MODULE_NAME, master->name, slave->name, i)) != 0) {
    return err;
    }

    *(diag->commonfault) = 0;
    *(diag->overtempdev) = 0;
    *(diag->undervoltage) = 0;
    *(diag->missingvoltage) = 0;
  }
  // initialize pins
  for (i=0; i<LCEC_EL2014_CHANS; i++) {
    chan = &hal_data->chans[i];
		
    // initialize POD entries
    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6001 + (i << 4), 0x01, &chan->ovt_pdo_os, &chan->ovt_pdo_bp);
    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6001 + (i << 4), 0x02, &chan->opl_pdo_os, &chan->opl_pdo_bp);
    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6001 + (i << 4), 0x03, &chan->ovc_pdo_os, &chan->ovc_pdo_bp);
    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x6001 + (i << 4), 0x04, &chan->shc_pdo_os, &chan->shc_pdo_bp);

    LCEC_PDO_INIT(pdo_entry_regs, slave->index, slave->vid, slave->pid, 0x7000 + (i << 4), 0x01, &chan->out_pdo_os, &chan->out_pdo_bp);

    // export pins
    if ((err = lcec_pin_newf_list(chan, slave_pins, LCEC_MODULE_NAME, master->name, slave->name, i)) != 0) {
      return err;
    }

    // initialize pins
    *(chan->overtemp) = 0;
    *(chan->overcurrent) = 0;
    *(chan->openload) = 0;
    *(chan->shortcircuit) = 0;
    *(chan->output) = 0;

  }

  return 0;
}

void lcec_el2014_read(struct lcec_slave *slave, long period) {
  lcec_master_t *master = slave->master;
  lcec_el2014_data_t *hal_data = (lcec_el2014_data_t *) slave->hal_data;
  uint8_t *pd = master->process_data;
  int i;
  lcec_el2014_chan_t *chan;
  lcec_el2014_diag_t *diag;
  uint8_t state;
  int16_t value;

  // wait for slave to be operational
  if (!slave->state.operational) {
    return;
  }
  for (i=0; i<LCEC_EL2014_DIAGS; i++) {
    diag = &hal_data->diags[i];
    *(diag->commonfault) = EC_READ_BIT(&pd[diag->cf_pdo_os], diag->cf_pdo_bp);
    *(diag->overtempdev) = EC_READ_BIT(&pd[diag->ovtd_pdo_os], diag->ovtd_pdo_bp);
    *(diag->undervoltage) =  EC_READ_BIT(&pd[diag->udv_pdo_os], diag->udv_pdo_bp);
    *(diag->missingvoltage) =  EC_READ_BIT(&pd[diag->misv_pdo_os], diag->misv_pdo_bp);
  }
  // check inputs
  for (i=0; i<LCEC_EL2014_CHANS; i++) {
    chan = &hal_data->chans[i];

    *(chan->overtemp) =  EC_READ_BIT(&pd[chan->ovt_pdo_os], chan->ovt_pdo_bp);
    *(chan->overcurrent) =  EC_READ_BIT(&pd[chan->ovc_pdo_os], chan->ovc_pdo_bp);
    *(chan->openload) =  EC_READ_BIT(&pd[chan->opl_pdo_os], chan->opl_pdo_bp);
    *(chan->shortcircuit) =  EC_READ_BIT(&pd[chan->shc_pdo_os], chan->shc_pdo_bp);

  }
}
void lcec_el2014_write(struct lcec_slave *slave, long period) {
  lcec_master_t *master = slave->master;
  lcec_el2014_data_t *hal_data = (lcec_el2014_data_t *) slave->hal_data;
  uint8_t *pd = master->process_data;
  int i;
  lcec_el2014_chan_t *chan;
  uint8_t state;
  int16_t value;

  // wait for slave to be operational
  if (!slave->state.operational) {
    return;
  }

  // check inputs
  for (i=0; i<LCEC_EL2014_CHANS; i++) {
    chan = &hal_data->chans[i];

    EC_WRITE_BIT(&pd[chan->out_pdo_os], chan->out_pdo_bp, *(chan->output));

  }




}