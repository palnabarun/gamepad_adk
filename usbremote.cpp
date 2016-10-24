#include "USBremote.h"

JoystickReportParser::JoystickReportParser(JoystickEvents *evt, bool flag, int *one_, int *two_, int *three_, int *four_, int *up_, int *down_, int *left_, int *right_, int *l1_, int *l2_, int *l3_, int *r1_, int *r2_, int *r3_, int *x_, int *y_, int *z1_, int *z2_, int *rz_, int *start_, int *sel_) :
joyEvents(evt),
addr_one(one_),
addr_two(two_),
addr_three(three_),
addr_four(four_),
addr_up(up_),
addr_down(down_),
addr_left(left_),
addr_right(right_),
addr_r1(r1_),
addr_r2(r2_),
addr_r3(r3_),
addr_l1(l1_),
addr_l2(l2_),
addr_l3(l3_),
addr_x(x_),
addr_y(y_),
addr_z1(z1_),
addr_z2(z2_),
addr_rz(rz_),
addr_start(start_),
addr_sel(sel_),
debug_flag(flag),
oldHat(0xDE),
oldButtons(0) {
        for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++)
                oldPad[i] = 0xD;
}

void JoystickReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
        bool match = true;

        // Checking if there are changes in report since the method was last called
        for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++)
                if (buf[i] != oldPad[i]) {
                        match = false;
                        break;
                }

        // Calling Game Pad event handler
        if (!match && joyEvents) {
                joyEvents->OnGamePadChanged((const GamePadEventData*)buf, debug_flag, addr_x, addr_y, addr_z1, addr_z2, addr_rz);

                for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++) oldPad[i] = buf[i];
        }

        uint8_t hat = (buf[5] & 0xF);

        // Calling Hat Switch event handler
        if (hat != oldHat && joyEvents) {
                joyEvents->OnHatSwitch(hat, debug_flag, addr_up, addr_down, addr_left, addr_right);
                oldHat = hat;
        }

        uint16_t buttons = (0x0000 | buf[6]);
        buttons <<= 4;
        buttons |= (buf[5] >> 4);
        uint16_t changes = (buttons ^ oldButtons);

        // Calling Button Event Handler for every button changed
        if (changes) {
                for (uint8_t i = 0; i < 0x0C; i++) {
                        uint16_t mask = (0x0001 << i);

                        if (((mask & changes) > 0) && joyEvents) {
                                if ((buttons & mask) > 0)
                                        joyEvents->OnButtonDn(i, debug_flag, addr_one, addr_two, addr_three, addr_four, addr_l1, addr_l2, addr_l3, addr_r1, addr_r2, addr_r3, addr_start, addr_sel);
                                else
                                        joyEvents->OnButtonUp(i, debug_flag, addr_one, addr_two, addr_three, addr_four, addr_l1, addr_l2, addr_l3, addr_r1, addr_r2, addr_r3, addr_start, addr_sel);
                        }
                }
                oldButtons = buttons;
        }
}

void JoystickEvents::SetAllZero(int *up_, int *down_, int *left_, int *right_) {
  *up_=0;
  *down_=0;
  *left=0;
  *right_=0;
}

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt, bool debug_flag, int *x_, int *y_, int *z1_, int *z2_, int *rz_) {
        if(debug_flag) {
          Serial.print("X1: ");
          Serial.print(evt->X);
          Serial.print("\tY1: ");
          Serial.print(evt->Y);
          Serial.print("\tX2: ");
          Serial.print(evt->Z1);
          Serial.print("\tY2: ");
          Serial.print(evt->Z2);
          Serial.print("\tRz: ");
          Serial.print(evt->Rz);
          Serial.println("");
        } else {
          Serial.println("DEBUG IS OFF.");
        }
          *x_ = (evt->X);
          *y_ = (evt->Y);
          *z1_ = (evt->Z1);
          *z2_ = (evt->Z2);
          *rz_ = (evt->Rz);
}

void JoystickEvents::OnHatSwitch(uint8_t hat, bool debug_flag, int *up_, int *down_, int *left_, int *right_) {
        if(debug_flag) {
          Serial.print("Hat Switch: ");
          Serial.print(hat);
          Serial.println("");
        } else {
          Serial.println("DEBUG IS OFF.");          
        }
        switch(hat) {
          case 0:
            SetAllZero();
            *up_ = 1;
            break;
        case 1:
            SetAllZero();
            *up_ = 1;
            *right_ = 1;
            break;
        case 2:
            SetAllZero();
            *right_ = 1;
            break;
        case 3:
            SetAllZero();
            *right_ = 1;
            *down_ = 1;
            break;
        case 4:
            SetAllZero();
            *down_ = 1;
            break;
        case 5:
            SetAllZero();
            *down_ = 1;
            *left_ = 1;
            break;
        case 6:
            SetAllZero();
            *left_ = 1;
            break;
        case 7:
            SetAllZero();
            *up_ = 1;
            *left_ = 1;
            break;
        case 15:
            SetAllZero();
            break;
        default:
            SetAllZero();
            break;
        }
}

void JoystickEvents::OnButtonUp(uint8_t but_id, bool debug_flag, int *one_, int *two_, int *three_, int *four_, int *l1_, int *l2_, int *l3_, int *r1_, int *r2_, int *r3_, int *start_, int *sel_) {
        if(debug_flag) {
          Serial.print("Up: ");
          Serial.println(but_id, DEC);
        } else {
          Serial.println("DEBUG IS OFF.");          
        }
        switch(but_id) {
          case 0:
            *one_ = 0;
            break;
          case 1:
            *two_ = 0;
            break;
          case 2:
            *three_ = 0;
            break;
          case 3:
            *four_ = 0;
            break;
          case 4:
            *l1_ = 0;
            break;
          case 5:
            *r1_ = 0;
            break;
          case 6:
            *l2_ = 0;
            break;
          case 7:
            *r2_ = 0;
            break;
          case 8:
            *sel_ = 0;
            break;
          case 9:
            *start_ = 0;
            break;
          case 10:
            *l3_ = 0;
            break;
          case 11:
            *r3_ = 0;
            break;
        }
}

void JoystickEvents::OnButtonDn(uint8_t but_id, bool debug_flag, int *one_, int *two_, int *three_, int *four_, int *l1_, int *l2_, int *l3_, int *r1_, int *r2_, int *r3_, int *start_, int *sel_) {
        if(debug_flag) {
          Serial.print("Dn: ");
          Serial.println(but_id, DEC);
        } else {
          Serial.println("DEBUG IS OFF.");          
        }
         switch(but_id) {
            case 0:
              *one_ = 1;
              break;
            case 1:
              *two_ = 1;
              break;
            case 2:
              *three_ = 1;
              break;
            case 3:
              *four_ = 1;
              break;
            case 4:
              *l1_ = 1;
              break;
            case 5:
              *r1_ = 1;
              break;
            case 6:
              *l2_ = 1;
              break;
            case 7:
              *r2_ = 1;
              break;
            case 8:
              *sel_ = 1;
              break;
            case 9:
              *start_ = 1;
              break;
            case 10:
              *l3_ = 1;
              break;
            case 11:
              *r3_ = 1;
              break;
          }
}
