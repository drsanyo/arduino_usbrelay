const byte DATA_PROTOCOL_LENGHT = 2;

const byte PIN_REL_1 = 2;
const byte PIN_REL_2 = 3;
const byte PIN_REL_3 = 4;
const byte PIN_REL_4 = 5;
const byte PIN_REL_5 = 6;
const byte PIN_REL_6 = 7;
const byte PIN_REL_7 = 8;
const byte PIN_REL_8 = 9;

const long TIMER_HALF_SEC_TIME = 500000;
const long TIMER_ONE_SEC_TIME = TIMER_HALF_SEC_TIME * 2;
const int WATCH_DOG_TIME = 10;

const int LF = 10;

const byte ACTION_CODE_INDEX = 0;
const byte ACTION_ARGS_INDEX = 1;

const byte ACTION_SET_DEVICE_ON = '1';
const byte ACTION_SET_DEVICE_OFF = '2';
const byte ACTION_SET_WATCHDOG_STATE = '3';
const byte ACTION_GET_SYSTEM_STATE = '4';

const byte DEVICE_1 = 0;
const byte DEVICE_2 = 1;
const byte DEVICE_3 = 2;
const byte DEVICE_4 = 3;

const byte MAX_SUPPORTED_DEVICES = 4;
const byte PINS_TO_DEVICE = 2;
const byte DEVICES_LST[MAX_SUPPORTED_DEVICES][PINS_TO_DEVICE] = {
 {PIN_REL_1, PIN_REL_2},
 {PIN_REL_3, PIN_REL_4},
 {PIN_REL_5, PIN_REL_6},
 {PIN_REL_7, PIN_REL_8}
};


const char SET_WATCHDOG_ENABLE = '1'; // all diferent value mean DISABLE
