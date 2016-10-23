#if !defined(__USBREMOTE_H__)
#define __USBREMOTE_H__

#include <usbhid.h>

struct GamePadEventData {
        uint8_t X, Y, Z1, Z2, Rz;
};

class JoystickEvents {
public:
        virtual void OnGamePadChanged(const GamePadEventData *evt, bool debug_flag, int *x_, int *y_, int *z1_, int *z2_, int *rz_);
        virtual void OnHatSwitch(uint8_t hat, bool debug_flag, int *up_, int *down_, int *left_, int *right_);
        virtual void OnButtonUp(uint8_t but_id, bool debug_flag, int *one_, int *two_, int *three_, int *four_, int *l1_, int *l2_, int *l3_, int *r1_, int *r2_, int *r3_, int *start_, int *sel_);
        virtual void OnButtonDn(uint8_t but_id, bool debug_flag, int *one_, int *two_, int *three_, int *four_, int *l1_, int *l2_, int *l3_, int *r1_, int *r2_, int *r3_, int *start_, int *sel_);
};

#define RPT_GEMEPAD_LEN		5

class JoystickReportParser : public HIDReportParser {
        JoystickEvents *joyEvents;

        uint8_t oldPad[RPT_GEMEPAD_LEN];
        uint8_t oldHat;
        uint16_t oldButtons;
        int *addr_one, *addr_two, *addr_three, *addr_four, *addr_up, *addr_down, *addr_left, *addr_right, *addr_l1, *addr_l2, *addr_l3, *addr_r1, *addr_r2, *addr_r3, *addr_x, *addr_y, *addr_z1, *addr_z2, *addr_rz, *addr_start, *addr_sel;
        bool debug_flag;
public:
        JoystickReportParser(JoystickEvents *evt, bool flag, int *one_, int *two_, int *three_, int *four_, int *up_, int *down_, int *left_, int *right_, int *l1_, int *l2_, int *l3_, int *r1_, int *r2_, int *r3_, int *x_, int *y_, int *z1_, int *z2_, int *rz_, int *start_, int *sel_);

        virtual void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};

#endif // __USBREMOTE_H__
