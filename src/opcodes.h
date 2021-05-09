#ifndef quesler_opcodes_h
#define quesler_opcodes_h

#include "common.h"

#define OPCODE_MAX_ARITY 11

typedef enum {
    T_NONE,
    T_ARGS,
    T_DWORD,
    T_WORD,
    T_BYTE,
    T_FLOAT,
    T_REG,
    T_BREG,
    T_DREG,
    T_IMED,
    T_FUNC,
    T_FUNC2,
    T_PUSH,
    T_VASTART,
    T_VAEND,
    T_V2,
    T_V3,
    T_V4,
    T_DC,
    T_STR,
    T_PFLAG,
    T_SWITCH,
    T_SWITCH2B,
    T_DATA,
    T_STRDATA,
    NUM_PARAMETER_KINDS
} ParameterType;

#define VARIABLE_SIZED -1

typedef struct {
    const uint16_t opcode;
    const char *name;
    const ParameterType parameters[OPCODE_MAX_ARITY];
} OpcodeDefinition;

typedef enum {
    OP_NOP = 0x00,
    OP_RET = 0x01,
    OP_SYNC = 0x02,
    OP_EXIT = 0x03,
    OP_THREAD = 0x04,
    OP_VA_START = 0x05,
    OP_VA_END = 0x06,
    OP_VA_CALL = 0x07,
    OP_LET = 0x08,
    OP_LETI = 0x09,
    OP_LETB = 0x0A,
    OP_LETW = 0x0B,
    OP_LETA = 0x0C,
    OP_LETO = 0x0D,
    OP_SET = 0x10,
    OP_CLEAR = 0x11,
    OP_REV = 0x12,
    OP_GSET = 0x13,
    OP_GCLEAR = 0x14,
    OP_GREV = 0x15,
    OP_GLET = 0x16,
    OP_GGET = 0x17,
    OP_ADD = 0x18,
    OP_ADDI = 0x19,
    OP_SUB = 0x1a,
    OP_SUBI = 0x1b,
    OP_MUL = 0x1c,
    OP_MULI = 0x1d,
    OP_DIV = 0x1e,
    OP_DIVI = 0x1f,
    OP_AND = 0x20,
    OP_ANDI = 0x21,
    OP_OR = 0x22,
    OP_ORI = 0x23,
    OP_XOR = 0x24,
    OP_XORI = 0x25,
    OP_MOD = 0x26,
    OP_MODI = 0x27,
    OP_JMP = 0x28,
    OP_CALL = 0x29,
    OP_JMP_ON = 0x2a,
    OP_JMP_OFF = 0x2b,
    OP_JMP_EQ = 0x2c,
    OP_JMPI_EQ = 0x2d,
    OP_JMP_NEQ = 0x2e,
    OP_JMPI_NEQ = 0x2f,
    OP_UJMP_GT = 0x30,
    OP_UJMPI_GT = 0x31,
    OP_JMP_GT = 0x32,
    OP_JMPI_GT = 0x33,
    OP_UJMP_LT = 0x34,
    OP_UJMPI_LT = 0x35,
    OP_JMP_LT = 0x36,
    OP_JMPI_LT = 0x37,
    OP_UJMP_GTE = 0x38,
    OP_UJMPI_GTE = 0x39,
    OP_JMP_GTE = 0x3a,
    OP_JMPI_GTE = 0x3b,
    OP_UJMP_LTE = 0x3c,
    OP_UJMPI_LTE = 0x3d,
    OP_JMP_LTE = 0x3e,
    OP_JMPI_LTE = 0x3f,
    OP_SWITCH_JMP = 0x40,
    OP_SWITCH_CALL = 0x41,
    OP_STACK_PUSH = 0x42,
    OP_STACK_POP = 0x43,
    OP_STACK_PUSHM = 0x44,
    OP_STACK_POPM = 0x45,
    OP_ARG_PUSHR = 0x48,
    OP_ARG_PUSHL = 0x49,
    OP_ARG_PUSHB = 0x4a,
    OP_ARG_PUSHW = 0x4b,
    OP_ARG_PUSHA = 0x4C,
    OP_ARG_PUSHO = 0x4D,
    OP_ARG_PUSHS = 0x4e,
    OP_MESSAGE = 0x50,
    OP_LIST = 0x51,
    OP_FADEIN = 0x52,
    OP_FADEOUT = 0x53,
    OP_SE = 0x54,
    OP_BGM = 0x55,
    OP_ENABLE = 0x58,
    OP_DISABLE = 0x59,
    OP_WINDOW_MSG = 0x5a,
    OP_ADD_MSG = 0x5b,
    OP_MESEND = 0x5c,
    OP_GETTIME = 0x5d,
    OP_WINEND = 0x5e,
    OP_NPC_CRT_V1 = 0x60,
    OP_NPC_CRT_V3 = 0x60,
    OP_NPC_STOP = 0x61,
    OP_NPC_PLAY = 0x62,
    OP_NPC_KILL = 0x63,
    OP_NPC_NONT = 0x64,
    OP_NPC_TALK = 0x65,
    OP_NPC_CRP_V1 = 0x66,
    OP_NPC_CRP_V3 = 0x66,
    OP_CREATE_PIPE = 0x68,
    OP_P_HPSTAT_V1 = 0x69,
    OP_P_HPSTAT_V3 = 0x69,
    OP_P_DEAD_V1 = 0x6a,
    OP_P_DEAD_V3 = 0x6a,
    OP_P_DISABLEWARP = 0x6b,
    OP_P_ENABLEWARP = 0x6c,
    OP_P_MOVE_V1 = 0x6d,
    OP_P_MOVE_V3 = 0x6d,
    OP_P_LOOK = 0x6e,
    OP_P_ACTION_DISABLE = 0x70,
    OP_P_ACTION_ENABLE = 0x71,
    OP_DISABLE_MOVEMENT1 = 0x72,
    OP_ENABLE_MOVEMENT1 = 0x73,
    OP_P_NONCOL = 0x74,
    OP_P_COL = 0x75,
    OP_P_SETPOS = 0x76,
    OP_P_RETURN_GUILD = 0x77,
    OP_P_TALK_GUILD = 0x78,
    OP_NPC_TALK_PL_V1 = 0x79,
    OP_NPC_TALK_PL_V3 = 0x79,
    OP_NPC_TALK_KILL = 0x7a,
    OP_NPC_CRTPK_V1 = 0x7b,
    OP_NPC_CRTPK_V3 = 0x7b,
    OP_NPC_CRPPK_V1 = 0x7c,
    OP_NPC_CRPPK_V3 = 0x7c,
    OP_NPC_CRPTALK_V1 = 0x7d,
    OP_NPC_CRPTALK_V3 = 0x7d,
    OP_P_LOOK_AT = 0x7e,
    OP_NPC_CRP_ID_V1 = 0x7f,
    OP_NPC_CRP_ID_V3 = 0x7f,
    OP_CAM_QUAKE = 0x80,
    OP_CAM_ADJ = 0x81,
    OP_CAM_ZMIN = 0x82,
    OP_CAM_ZMOUT = 0x83,
    OP_CAM_PAN_V1 = 0x84,
    OP_CAM_PAN_V3 = 0x84,
    OP_GAME_LEV_SUPER = 0x85,
    OP_GAME_LEV_RESET = 0x86,
    OP_POS_PIPE_V1 = 0x87,
    OP_POS_PIPE_V3 = 0x87,
    OP_IF_ZONE_CLEAR = 0x88,
    OP_CHK_ENE_NUM = 0x89,
    OP_UNHIDE_OBJ = 0x8a,
    OP_UNHIDE_ENE = 0x8b,
    OP_AT_COORDS_CALL = 0x8c,
    OP_AT_COORDS_TALK = 0x8d,
    OP_COL_NPCIN = 0x8e,
    OP_COL_NPCINR = 0x8f,
    OP_SWITCH_ON = 0x90,
    OP_SWITCH_OFF = 0x91,
    OP_PLAYBGM_EPI = 0x92,
    OP_SET_MAINWARP = 0x93,
    OP_SET_OBJ_PARAM = 0x94,
    OP_SET_FLOOR_HANDLER = 0x95,
    OP_CLR_FLOOR_HANDLER = 0x96,
    OP_COL_PLINAW = 0x97,
    OP_HUD_HIDE = 0x98,
    OP_HUD_SHOW = 0x99,
    OP_CINE_ENABLE = 0x9a,
    OP_CINE_DISABLE = 0x9b,
    OP_SET_QT_FAILURE = 0xa1,
    OP_SET_QT_SUCCESS = 0xa2,
    OP_CLR_QT_FAILURE = 0xa3,
    OP_CLR_QT_SUCCESS = 0xa4,
    OP_SET_QT_CANCEL = 0xa5,
    OP_CLR_QT_CANCEL = 0xa6,
    OP_PL_WALK_V1 = 0xa8,
    OP_PL_WALK_V3 = 0xa8,
    OP_PL_ADD_MESETA = 0xb0,
    OP_THREAD_STG = 0xb1,
    OP_DEL_OBJ_PARAM = 0xb2,
    OP_ITEM_CREATE = 0xb3,
    OP_ITEM_CREATE2 = 0xb4,
    OP_ITEM_DELETE = 0xb5,
    OP_ITEM_DELETE2 = 0xb6,
    OP_ITEM_CHECK = 0xb7,
    OP_SETEVT = 0xb8,
    OP_GET_DIFFLVL = 0xb9,
    OP_SET_QT_EXIT = 0xba,
    OP_CLR_QT_EXIT = 0xbb,
    OP_PARTICLE_V1 = 0xc0,
    OP_PARTICLE_V3 = 0xc0,
    OP_NPC_TEXT = 0xc1,
    OP_NPC_CHKWARP = 0xc2,
    OP_PL_PKOFF = 0xc3,
    OP_MAP_DESIGNATE = 0xc4,
    OP_MASTERKEY_ON = 0xc5,
    OP_MASTERKEY_OFF = 0xc6,
    OP_WINDOW_TIME = 0xc7,
    OP_WINEND_TIME = 0xc8,
    OP_WINSET_TIME = 0xc9,
    OP_GETMTIME = 0xca,
    OP_SET_QUEST_BOARD_HANDLER = 0xcb,
    OP_CLEAR_QUEST_BOARD_HANDLER = 0xcc,
    OP_PARTICLE_ID_V1 = 0xcd,
    OP_PARTICLE_ID_V3 = 0xcd,
    OP_NPC_CRPTALK_ID_V1 = 0xce,
    OP_NPC_CRPTALK_ID_V3 = 0xce,
    OP_NPC_LANG_CLEAN = 0xcf,
    OP_PL_PKON = 0xd0,
    OP_PL_CHK_ITEM2 = 0xd1,
    OP_ENABLE_MAINMENU = 0xd2,
    OP_DISABLE_MAINMENU = 0xd3,
    OP_START_BATTLEBGM = 0xd4,
    OP_END_BATTLEBGM = 0xd5,
    OP_DISP_MSG_QB = 0xd6,
    OP_CLOSE_MSG_QB = 0xd7,
    OP_SET_EVENTFLAG_V1 = 0xd8,
    OP_SET_EVENTFLAG_V3 = 0xd8,
    OP_SYNC_LETI = 0xd9,
    OP_SET_RETURNHUNTER = 0xda,
    OP_SET_RETURNCITY = 0xdb,
    OP_LOAD_PVR = 0xdc,
    OP_LOAD_MIDI = 0xdd,
    OP_UNKNOWNDE = 0xde,
    OP_NPC_PARAM_V1 = 0xdf,
    OP_NPC_PARAM_V3 = 0xdf,
    OP_PAD_DRAGON = 0xe0,
    OP_CLEAR_MAINWARP = 0xe1,
    OP_PCAM_PARAM_V1 = 0xe2,
    OP_PCAM_PARAM_V3 = 0xe2,
    OP_START_SETEVT_V1 = 0xe3,
    OP_START_SETEVT_V3 = 0xe3,
    OP_WARP_ON = 0xe4,
    OP_WARP_OFF = 0xe5,
    OP_GET_SLOTNUMBER = 0xe6,
    OP_GET_SERVERNUMBER = 0xe7,
    OP_SET_EVENTFLAG2 = 0xe8,
    OP_RES = 0xe9,
    OP_UNKNOWNEA = 0xea,
    OP_ENABLE_BGMCTRL = 0xeb,
    OP_SW_SEND = 0xec,
    OP_CREATE_BGMCTRL = 0xed,
    OP_PL_ADD_MESETA2 = 0xee,
    OP_SYNC_LET = 0xef,
    OP_SYNC_REGISTER = 0xef,
    OP_SYNC_REGISTER2 = 0xef,
    OP_SEND_REGWORK = 0xf0,
    OP_LETI_FIXED_CAMERA_V1 = 0xf1,
    OP_LETI_FIXED_CAMERA_V3 = 0xf1,
    OP_DEFAULT_CAMERA_POS1 = 0xf2,
    OP_UNKNOWNF8 = 0xf8,
    EXTENDED_OPCODE_TABLE1 = 0xf8,
    OP_GET_GC_NUMBER = 0xfa,
    OP_UNKNOWNFB = 0xfb,
    OP_SET_CHAT_CALLBACK = 0xf801,
    OP_GET_DIFFLVL2 = 0xf808,
    OP_GET_NUMBER_OF_PLAYER1 = 0xf809,
    OP_GET_COORD_OF_PLAYER = 0xf80a,
    OP_ENABLE_MAP = 0xf80b,
    OP_DISABLE_MAP = 0xf80c,
    OP_MAP_DESIGNATE_EX = 0xf80d,
    OP_UNKNOWNF80E = 0xf80e,
    OP_UNKNOWNF80F = 0xf80f,
    OP_BA_INITIAL_FLOOR = 0xf810,
    OP_SET_BA_RULES = 0xf811,
    OP_UNKNOWNF812 = 0xf812,
    OP_UNKNOWNF813 = 0xf813,
    OP_UNKNOWNF814 = 0xf814,
    OP_UNKNOWNF815 = 0xf815,
    OP_UNKNOWNF816 = 0xf816,
    OP_UNKNOWNF817 = 0xf817,
    OP_UNKNOWNF818 = 0xf818,
    OP_UNKNOWNF819 = 0xf819,
    OP_UNKNOWNF81A = 0xf81a,
    OP_UNKNOWNF81B = 0xf81b,
    OP_BA_DISP_MSG = 0xf81c,
    OP_DEATH_LVL_UP = 0xf81d,
    OP_DEATH_TECH_LVL_UP = 0xf81e,
    OP_CMODE_STAGE = 0xf820,
    OP_UNKNOWNF823 = 0xf823,
    OP_UNKNOWNF824 = 0xf824,
    OP_EXP_MULTIPLICATION = 0xf825,
    OP_EXP_DIVISION = 0xf826,
    OP_GET_USER_IS_DEAD = 0xf827,
    OP_GO_FLOOR = 0xf828,
    OP_UNLOCK_DOOR2 = 0xf82b,
    OP_LOCK_DOOR2 = 0xf82c,
    OP_IF_SWITCH_NOT_PRESSED = 0xf82d,
    OP_IF_SWITCH_PRESSED = 0xf82e,
    OP_UNKNOWNF82F = 0xf82f,
    OP_CONTROL_DRAGON = 0xf830,
    OP_RELEASE_DRAGON = 0xf831,
    OP_SHRINK = 0xf838,
    OP_UNSHRINK = 0xf839,
    OP_DISPLAY_CLOCK2 = 0xf83c,
    OP_UNKNOWNF83D = 0xf83d,
    OP_DELETE_AREA_TITLE = 0xf83e,
    OP_LOAD_NPC_DATA = 0xf840,
    OP_GET_NPC_DATA = 0xf841,
    OP_GIVE_DAMAGE_SCORE = 0xf848,
    OP_TAKE_DAMAGE_SCORE = 0xf849,
    OP_UNK_SCORE_F84A = 0xf84a,
    OP_UNK_SCORE_F84B = 0xf84b,
    OP_KILL_SCORE = 0xf84c,
    OP_DEATH_SCORE = 0xf84d,
    OP_UNK_SCORE_F84E = 0xf84e,
    OP_ENEMY_DEATH_SCORE = 0xf84f,
    OP_MESETA_SCORE = 0xf850,
    OP_UNKNOWNF851 = 0xf851,
    OP_UNKNOWNF852 = 0xf852,
    OP_REVERSE_WARPS = 0xf853,
    OP_UNREVERSE_WARPS = 0xf854,
    OP_SET_ULT_MAP = 0xf855,
    OP_UNSET_ULT_MAP = 0xf856,
    OP_SET_AREA_TITLE = 0xf857,
    OP_UNKNOWNF858 = 0xf858,
    OP_EQUIP_ITEM_V2 = 0xf85a,
    OP_EQUIP_ITEM_V3 = 0xf85a,
    OP_UNEQUIP_ITEM_V2 = 0xf85b,
    OP_UNEQUIP_ITEM_V3 = 0xf85b,
    OP_BA_SHOW_SELF_TRAPS = 0xf858,
    OP_BA_HIDE_SELF_TRAPS = 0xf859,
    OP_UNKNOWNF85E = 0xf85e,
    OP_UNKNOWNF85F = 0xf85f,
    OP_UNKNOWNF860 = 0xf860,
    OP_UNKNOWNF861 = 0xf861,
    OP_CMODE_RANK = 0xf864,
    OP_AWARD_ITEM_NAME = 0xf865,
    OP_AWARD_ITEM_SELECT = 0xf866,
    OP_AWARD_ITEM_GIVE_TO = 0xf867,
    OP_UNKNOWNF868 = 0xf868,
    OP_UNKNOWNF869 = 0xf869,
    OP_ITEM_CREATE_CMODE = 0xf86a,
    OP_UNKNOWNF86B = 0xf86b,
    OP_AWARD_ITEM_OK = 0xf86c,
    OP_UNKNOWNF86D = 0xf86d,
    OP_UNKNOWNF86E = 0xf86e,
    OP_BA_SET_LIVES = 0xf86f,
    OP_BA_SET_TECH_LVL = 0xf870,
    OP_BA_SET_LVL = 0xf871,
    OP_BA_SET_TIME_LIMIT = 0xf872,
    OP_BOSS_IS_DEAD = 0xf873,
    OP_ENABLE_TECHS = 0xf877,
    OP_DISABLE_TECHS = 0xf878,
    OP_GET_GENDER = 0xf879,
    OP_GET_CHARA_CLASS = 0xf87a,
    OP_TAKE_SLOT_MESETA = 0xf87b,
    OP_READ_GUILDCARD_FLAG = 0xf87f,
    OP_UNKNOWNF880 = 0xf880,
    OP_GET_PL_NAME = 0xf881,
    OP_UNKNOWNF883 = 0xf883,
    OP_BA_CLOSE_MSG = 0xf888,
    OP_GET_PLAYER_STATUS = 0xf88a,
    OP_SEND_MAIL = 0xf88b,
    OP_GET_GAME_VERSION = 0xf88c,
    OP_CHL_SET_TIMERECORD = 0xf88d,
    OP_CHL_GET_TIMERECORD = 0xf88e,
    OP_UNKNOWNF88F = 0xf88f,
    OP_UNKNOWNF890 = 0xf890,
    OP_LOAD_ENEMY_DATA = 0xf891,
    OP_GET_PHYSICAL_DATA = 0xf892,
    OP_GET_ATTACK_DATA = 0xf893,
    OP_GET_RESIST_DATA = 0xf894,
    OP_GET_MOVEMENT_DATA = 0xf895,
    OP_SHIFT_LEFT = 0xf898,
    OP_SHIFT_RIGHT = 0xf899,
    OP_GET_RANDOM = 0xf89a,
    OP_RESET_MAP = 0xf89b,
    OP_DISP_CHL_RETRY_MNU = 0xf89c,
    OP_CHL_REVERSER = 0xf89d,
    OP_UNKNOWNF89E = 0xf89e,
    OP_UNKNOWNF89F = 0xf89f,
    OP_UNKNOWNF8A0 = 0xf8a0,
    OP_UNKNOWNF8A1 = 0xf8a1,
    OP_UNKNOWNF8A8 = 0xf8a8,
    OP_UNKNOWNF8A9 = 0xf8a9,
    OP_GET_NUMBER_OF_PLAYER2 = 0xf8ad,
    OP_UNKNOWNF8B8 = 0xf8b8,
    OP_CHL_RECOVERY = 0xf8b9,
    OP_SET_EPISODE = 0xf8bc,
    OP_FILE_DL_REQ = 0xf8c0,
    OP_GET_DL_STATUS = 0xf8c1,
    OP_GBA_UNKNOWN4 = 0xf8c2,
    OP_GET_GBA_STATE = 0xf8c3,
    OP_UNKNOWNF8C4 = 0xf8c4,
    OP_UNKNOWNF8C5 = 0xf8c5,
    OP_QEXIT = 0xf8c6,
    OP_USE_ANIMATION = 0xf8c7,
    OP_STOP_ANIMATION = 0xf8c8,
    OP_RUN_TO_COORD = 0xf8c9,
    OP_SET_SLOT_INVINCIBLE = 0xf8ca,
    OP_UNKNOWNF8CB = 0xf8cb,
    OP_SET_SLOT_POISON = 0xf8cc,
    OP_SET_SLOT_PARALYSE = 0xf8cd,
    OP_SET_SLOT_SHOCK = 0xf8ce,
    OP_SET_SLOT_FREEZE = 0xf8cf,
    OP_SET_SLOT_SLOW = 0xf8d0,
    OP_SET_SLOT_CONFUSE = 0xf8d1,
    OP_SET_SLOT_SHIFTA = 0xf8d2,
    OP_SET_SLOT_DEBAND = 0xf8d3,
    OP_SET_SLOT_JELLEN = 0xf8d4,
    OP_SET_SLOT_ZALURE = 0xf8d5,
    OP_FLETI_FIXED_CAMERA = 0xf8d6,
    OP_FLETI_LOCKED_CAMERA = 0xf8d7,
    OP_DEFAULT_CAMERA_POS2 = 0xf8d8,
    OP_SET_MOTION_BLUR = 0xf8d9,
    OP_SET_SCREEN_BW = 0xf8da,
    OP_UNKNOWNF8DB = 0xf8db,
    OP_NPC_ACTION_STRING = 0xf8dc,
    OP_GET_PAD_COND = 0xf8dd,
    OP_GET_BUTTON_COND = 0xf8de,
    OP_FREEZE_ENEMIES = 0xf8df,
    OP_UNFREEZE_ENEMIES = 0xf8e0,
    OP_FREEZE_EVERYTHING = 0xf8e1,
    OP_UNFREEZE_EVERYTHING = 0xf8e2,
    OP_RESTORE_HP = 0xf8e3,
    OP_RESTORE_TP = 0xf8e4,
    OP_CLOSE_CHAT_BUBBLE = 0xf8e5,
    OP_UNKNOWNF8E6 = 0xf8e6,
    OP_UNKNOWNF8E7 = 0xf8e7,
    OP_UNKNOWNF8E8 = 0xf8e8,
    OP_UNKNOWNF8E9 = 0xf8e9,
    OP_UNKNOWNF8EA = 0xf8ea,
    OP_UNKNOWNF8EB = 0xf8eb,
    OP_UNKNOWNF8EC = 0xf8ec,
    OP_ANIMATION_CHECK = 0xf8ed,
    OP_CALL_IMAGE_DATA = 0xf8ee,
    OP_UNKNOWNF8EF = 0xf8ef,
    OP_TURN_OFF_BGM_P2 = 0xf8f0,
    OP_TURN_ON_BGM_P2 = 0xf8f1,
    OP_LOAD_UNK_DATA = 0xf8f2,
    OP_PARTICLE2 = 0xf8f3,
    EXTENDED_OPCODE_TABLE2 = 0xf9,
    OP_DEC2FLOAT = 0xf901,
    OP_FLOAT2DEC = 0xf902,
    OP_FLET = 0xf903,
    OP_FLETI = 0xf904,
    OP_FADD = 0xf908,
    OP_FADDI = 0xf909,
    OP_FSUB = 0xf90a,
    OP_FSUBI = 0xf90b,
    OP_FMUL = 0xf90c,
    OP_FMULI = 0xf90d,
    OP_FDIV = 0xf90e,
    OP_FDIVI = 0xf90f,
    OP_GET_UNKNOWN_COUNT = 0xf910,
    OP_GET_STACKABLE_ITEM_COUNT = 0xf911,
    OP_FREEZE_AND_HIDE_EQUIP = 0xf912,
    OP_THAW_AND_SHOW_EQUIP = 0xf913,
    OP_SET_PALETTEX_CALLBACK = 0xf914,
    OP_ACTIVATE_PALETTEX = 0xf915,
    OP_ENABLE_PALETTEX = 0xf916,
    OP_RESTORE_PALETTEX = 0xf917,
    OP_DISABLE_PALETTEX = 0xf918,
    OP_GET_PALETTEX_ACTIVATED = 0xf919,
    OP_GET_UNKNOWN_PALETTEX_STATUS = 0xf91a,
    OP_DISABLE_MOVEMENT2 = 0xf91b,
    OP_ENABLE_MOVEMENT2 = 0xf91c,
    OP_GET_TIME_PLAYED = 0xf91d,
    OP_GET_GUILDCARD_TOTAL = 0xf91e,
    OP_GET_SLOT_MESETA = 0xf91f,
    OP_GET_PLAYER_LEVEL = 0xf920,
    OP_GET_SECTION_ID = 0xf921,
    OP_GET_PLAYER_HP = 0xf922,
    OP_GET_FLOOR_NUMBER = 0xf923,
    OP_GET_COORD_PLAYER_DETECT = 0xf924,
    OP_READ_GLOBAL_FLAG = 0xf925,
    OP_WRITE_GLOBAL_FLAG = 0xf926,
    OP_UNKNOWNF927 = 0xf927,
    OP_FLOOR_PLAYER_DETECT = 0xf928,
    OP_READ_DISK_FILE = 0xf929,
    OP_OPEN_PACK_SELECT = 0xf92a,
    OP_ITEM_SELECT = 0xf92b,
    OP_GET_ITEM_ID = 0xf92c,
    OP_COLOR_CHANGE = 0xf92d,
    OP_SEND_STATISTIC = 0xf92e,
    OP_UNKNOWNF92F = 0xf92f,
    OP_CHAT_BOX = 0xf930,
    OP_CHAT_BUBBLE = 0xf931,
    OP_UNKNOWNF933 = 0xf933,
    OP_SCROLL_TEXT = 0xf934,
    OP_GBA_UNKNOWN1 = 0xf935,
    OP_GBA_UNKNOWN2 = 0xf936,
    OP_GBA_UNKNOWN3 = 0xf937,
    OP_ADD_DAMAGE_TO = 0xf938,
    OP_ITEM_DELETE2_V3 = 0xf939,
    OP_GET_ITEM_INFO = 0xf93a,
    OP_ITEM_PACKING1 = 0xf93b,
    OP_ITEM_PACKING2 = 0xf93c,
    OP_GET_LANG_SETTING = 0xf93d,
    OP_PREPARE_STATISTIC = 0xf93e,
    OP_KEYWORD_DETECT = 0xf93f,
    OP_KEYWORD = 0xf940,
    OP_GET_GUILDCARD_NUM = 0xf941,
    OP_GET_WRAP_STATUS = 0xf944,
    OP_INITIAL_FLOOR = 0xf945,
    OP_SIN = 0xf946,
    OP_COS = 0xf947,
    OP_BOSS_IS_DEAD2 = 0xf94a,
    OP_UNKNOWNF94B = 0xf94b,
    OP_UNKNOWNF94C = 0xf94c,
    OP_IS_THERE_CARDBATTLE = 0xf94d,
    OP_BB_P2_MENU = 0xf950,
    OP_BB_MAP_DESIGNATE = 0xf951,
    OP_BB_GET_NUMBER_IN_PACK = 0xf952,
    OP_BB_SWAP_ITEM = 0xf953,
    OP_BB_CHECK_WRAP = 0xf954,
    OP_BB_EXCHANGE_PD_ITEM = 0xf955,
    OP_BB_EXCHANGE_PD_SRANK = 0xf956,
    OP_BB_EXCHANGE_PD_SPECIAL = 0xf957,
    OP_BB_EXCHANGE_PD_PERCENT = 0xf958,
    OP_UNKNOWNF959 = 0xf959,
    OP_BB_EXCHANGE_SLT = 0xf95c,
    OP_BB_EXCHANGE_PC = 0xf95d,
    OP_BB_BOX_CREATE_BP = 0xf95e,
    OP_BB_EXCHAGE_PT = 0xf95f,
    OP_UNKNOWNF960 = 0xf960,
    OP_UNKNOWNF961 = 0xf961
} Opcode;

#define NUM_OPCODES 505

const OpcodeDefinition* get_opcode_definition(Opcode opcode);

const OpcodeDefinition* get_opcode_definition_by_name(char* name);

size_t get_parameter_size(ParameterType type);

bool is_variable_sized(ParameterType type);

size_t get_opcode_arity(const OpcodeDefinition* opcode);

#endif
