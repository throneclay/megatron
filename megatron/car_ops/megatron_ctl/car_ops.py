# -*- coding: UTF-8 -*-

import smbus

class MegatronCfg(object):
    def __init__(self):
        self.address = 0x08
        self.op_list = dict()
        self.m_cfg_list = dict()
        self.init_op_list()
        self.init_m_cfg_list()

    def init_op_list(self):
        self.op_list['reset'] = 0x00
        self.op_list['prepare_flush_data'] = 0x01
        self.op_list['update_flush_data'] = 0x02
        self.op_list['update_m_cfg'] = 0x03
        self.op_list['move_forward'] = 0x04
        self.op_list['move_backward'] = 0x05
        self.op_list['move_left'] = 0x06
        self.op_list['move_right'] = 0x07
        self.op_list['servo_up'] = 0x08
        self.op_list['servo_down'] = 0x09
        self.op_list['servo_left'] = 0x0a
        self.op_list['servo_right'] = 0x0b

    def init_m_cfg_list(self):
        self.m_cfg_list['restore'] = 0x00
        self.m_cfg_list['left_forward_speed'] = 0x01
        self.m_cfg_list['left_backward_speed'] = 0x02
        self.m_cfg_list['right_forward_speed'] = 0x03
        self.m_cfg_list['right_backward_speed'] = 0x04
        self.m_cfg_list['horizontal_min'] = 0x05
        self.m_cfg_list['horizontal_max'] = 0x06
        self.m_cfg_list['vertical_min'] = 0x07
        self.m_cfg_list['vertical_max'] = 0x08
        self.m_cfg_list['current_horizontal'] = 0x09
        self.m_cfg_list['current_vertical'] = 0x0a
        self.m_cfg_list['v_step'] = 0x0b
        self.m_cfg_list['h_step'] = 0x0c
        self.m_cfg_list['factor'] = 0x0d

class CarHandle(object):
    def __init__(self):
        self.cfg = MegatronCfg()
        self.bus = smbus.SMBus(1)
        self.last_cmd = ''
	self.cnt = 0x08

    def mega_write_cmd(self, cmd, data):
        cmd_list = self.cfg.op_list
        self.bus.write_byte_data(self.cfg.address, cmd_list[cmd], data)

    def mega_read(self, cfg_code):
        cfg_list = self.cfg.m_cfg_list
        self.mega_write_cmd('prepare_flush_data', cfg_list[cfg_code])
        data = self.bus.read_byte(self.cfg.address)
        return data

    def motor_move(self, cmd):
        cmd_list = self.cfg.op_list
        self.last_cmd = cmd
        #self.mega_write_cmd(cmd, 0x00)
        self.mega_write_cmd(cmd, self.cnt)

    def servo_move(self, cmd):
        self.mega_write_cmd(cmd, 0x00)
        if (cmd == 'servo_up' or cmd == 'servo_down'):
            return self.read_v_servo()
        elif (cmd == 'servo_left' or cmd == 'servo_right'):
            return self.read_h_servo()
        else:
            return [self.read_h_servo(), self.read_v_servo()]

    def read_h_servo(self):
        return self.mega_read('current_horizontal')

    def read_v_servo(self):
        return self.mega_read('current_vertical')


