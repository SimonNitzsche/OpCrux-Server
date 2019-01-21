#ifndef __ENUMS__EAUTHPACKETS_HPP__
#define __ENUMS__EAUTHPACKETS_HPP__

#include <memory>

enum class ELoginReturnCode : uint8_t {
	GENERAL_FAIL = 0x00,
	SUCCESS = 0x01,
	ACCOUNT_BANNED = 0x02,
	GENERAL_FAIL_03 = 0x03,
	GENERAL_FAIL_04 = 0x04,
	ACCOUNT_FORBIDDEN = 0x05,
	INVALID_LOGIN = 0x06,
	ACCOUNT_LOCKED = 0x07,
	ACCOUNT_NOT_FOUND = 0x08,
	ACCOUNT_ACTIVATION_PENDING = 0x09,
	ACCOUNT_DISABLED = 0x0a,
	GAME_TIME_EXPIRED = 0x0b,
	FREE_TRIAL_ENDED = 0x0c,
	PLAYSHEDULE_FAIL = 0x0d,
	ACCOUNT_NOT_ACTIVATED = 0x0e,
	GENERAL_FAIL_0F = 0x0f,
	GENERAL_FAIL_10 = 0x10,
	GENERAL_FAIL_11 = 0x11,
	GENERAL_FAIL_12 = 0x12,
	GENERAL_FAIL_13 = 0x13,
	GENERAL_FAIL_14 = 0x14,
	GENERAL_FAIL_15 = 0x15,
	GENERAL_FAIL_16 = 0x16,
	GENERAL_FAIL_17 = 0x17,
	GENERAL_FAIL_18 = 0x18,
	GENERAL_FAIL_19 = 0x19,
	GENERAL_FAIL_1A = 0x1a,
	GENERAL_FAIL_1B = 0x1b,
	GENERAL_FAIL_1C = 0x1c,
	GENERAL_FAIL_1D = 0x1d,
	GENERAL_FAIL_1E = 0x1e,
	GENERAL_FAIL_1F = 0x1f,
	GENERAL_FAIL_20 = 0x20,
	GENERAL_FAIL_21 = 0x21,
	GENERAL_FAIL_22 = 0x22,
	GENERAL_FAIL_23 = 0x23,
	GENERAL_FAIL_24 = 0x24,
	GENERAL_FAIL_25 = 0x25,
	GENERAL_FAIL_26 = 0x26,
	GENERAL_FAIL_27 = 0x27,
	GENERAL_FAIL_28 = 0x28,
	GENERAL_FAIL_29 = 0x29,
	GENERAL_FAIL_2A = 0x2a,
	GENERAL_FAIL_2B = 0x2b,
	GENERAL_FAIL_2C = 0x2c,
	GENERAL_FAIL_2D = 0x2d,
	GENERAL_FAIL_2E = 0x2e,
	GENERAL_FAIL_2F = 0x2f,
	GENERAL_FAIL_30 = 0x30,
	GENERAL_FAIL_31 = 0x31,
	GENERAL_FAIL_32 = 0x32,
	GENERAL_FAIL_33 = 0x33,
	GENERAL_FAIL_34 = 0x34,
	GENERAL_FAIL_35 = 0x35,
	GENERAL_FAIL_36 = 0x36,
	GENERAL_FAIL_37 = 0x37,
	GENERAL_FAIL_38 = 0x38,
	GENERAL_FAIL_39 = 0x39,
	GENERAL_FAIL_3A = 0x3a,
	GENERAL_FAIL_3B = 0x3b,
	GENERAL_FAIL_3C = 0x3c,
	GENERAL_FAIL_3D = 0x3d,
	GENERAL_FAIL_3E = 0x3e,
	GENERAL_FAIL_3F = 0x3f,
	GENERAL_FAIL_40 = 0x40,
	GENERAL_FAIL_41 = 0x41,
	GENERAL_FAIL_42 = 0x42,
	GENERAL_FAIL_43 = 0x43,
	GENERAL_FAIL_44 = 0x44,
	GENERAL_FAIL_45 = 0x45,
	GENERAL_FAIL_46 = 0x46,
	GENERAL_FAIL_47 = 0x47,
	GENERAL_FAIL_48 = 0x48,
	GENERAL_FAIL_49 = 0x49,
	GENERAL_FAIL_4A = 0x4a,
	GENERAL_FAIL_4B = 0x4b,
	GENERAL_FAIL_4C = 0x4c,
	GENERAL_FAIL_4D = 0x4d,
	GENERAL_FAIL_4E = 0x4e,
	GENERAL_FAIL_4F = 0x4f,
	GENERAL_FAIL_50 = 0x50,
	GENERAL_FAIL_51 = 0x51,
	GENERAL_FAIL_52 = 0x52,
	GENERAL_FAIL_53 = 0x53,
	GENERAL_FAIL_54 = 0x54,
	GENERAL_FAIL_55 = 0x55,
	GENERAL_FAIL_56 = 0x56,
	GENERAL_FAIL_57 = 0x57,
	GENERAL_FAIL_58 = 0x58,
	GENERAL_FAIL_59 = 0x59,
	GENERAL_FAIL_5A = 0x5a,
	GENERAL_FAIL_5B = 0x5b,
	GENERAL_FAIL_5C = 0x5c,
	GENERAL_FAIL_5D = 0x5d,
	GENERAL_FAIL_5E = 0x5e,
	GENERAL_FAIL_5F = 0x5f,
	GENERAL_FAIL_60 = 0x60,
	GENERAL_FAIL_61 = 0x61,
	GENERAL_FAIL_62 = 0x62,
	GENERAL_FAIL_63 = 0x63,
	GENERAL_FAIL_64 = 0x64,
	GENERAL_FAIL_65 = 0x65,
	GENERAL_FAIL_66 = 0x66,
	GENERAL_FAIL_67 = 0x67,
	GENERAL_FAIL_68 = 0x68,
	GENERAL_FAIL_69 = 0x69,
	GENERAL_FAIL_6A = 0x6a,
	GENERAL_FAIL_6B = 0x6b,
	GENERAL_FAIL_6C = 0x6c,
	GENERAL_FAIL_6D = 0x6d,
	GENERAL_FAIL_6E = 0x6e,
	GENERAL_FAIL_6F = 0x6f,
	GENERAL_FAIL_70 = 0x70,
	GENERAL_FAIL_71 = 0x71,
	GENERAL_FAIL_72 = 0x72,
	GENERAL_FAIL_73 = 0x73,
	GENERAL_FAIL_74 = 0x74,
	GENERAL_FAIL_75 = 0x75,
	GENERAL_FAIL_76 = 0x76,
	GENERAL_FAIL_77 = 0x77,
	GENERAL_FAIL_78 = 0x78,
	GENERAL_FAIL_79 = 0x79,
	GENERAL_FAIL_7A = 0x7a,
	GENERAL_FAIL_7B = 0x7b,
	GENERAL_FAIL_7C = 0x7c,
	GENERAL_FAIL_7D = 0x7d,
	GENERAL_FAIL_7E = 0x7e,
	GENERAL_FAIL_7F = 0x7f,
	GENERAL_FAIL_80 = 0x80,
	GENERAL_FAIL_81 = 0x81,
	GENERAL_FAIL_82 = 0x82,
	GENERAL_FAIL_83 = 0x83,
	GENERAL_FAIL_84 = 0x84,
	GENERAL_FAIL_85 = 0x85,
	GENERAL_FAIL_86 = 0x86,
	GENERAL_FAIL_87 = 0x87,
	GENERAL_FAIL_88 = 0x88,
	GENERAL_FAIL_89 = 0x89,
	GENERAL_FAIL_8A = 0x8a,
	GENERAL_FAIL_8B = 0x8b,
	GENERAL_FAIL_8C = 0x8c,
	GENERAL_FAIL_8D = 0x8d,
	GENERAL_FAIL_8E = 0x8e,
	GENERAL_FAIL_8F = 0x8f,
	GENERAL_FAIL_90 = 0x90,
	GENERAL_FAIL_91 = 0x91,
	GENERAL_FAIL_92 = 0x92,
	GENERAL_FAIL_93 = 0x93,
	GENERAL_FAIL_94 = 0x94,
	GENERAL_FAIL_95 = 0x95,
	GENERAL_FAIL_96 = 0x96,
	GENERAL_FAIL_97 = 0x97,
	GENERAL_FAIL_98 = 0x98,
	GENERAL_FAIL_99 = 0x99,
	GENERAL_FAIL_9A = 0x9a,
	GENERAL_FAIL_9B = 0x9b,
	GENERAL_FAIL_9C = 0x9c,
	GENERAL_FAIL_9D = 0x9d,
	GENERAL_FAIL_9E = 0x9e,
	GENERAL_FAIL_9F = 0x9f,
	GENERAL_FAIL_A0 = 0xa0,
	GENERAL_FAIL_A1 = 0xa1,
	GENERAL_FAIL_A2 = 0xa2,
	GENERAL_FAIL_A3 = 0xa3,
	GENERAL_FAIL_A4 = 0xa4,
	GENERAL_FAIL_A5 = 0xa5,
	GENERAL_FAIL_A6 = 0xa6,
	GENERAL_FAIL_A7 = 0xa7,
	GENERAL_FAIL_A8 = 0xa8,
	GENERAL_FAIL_A9 = 0xa9,
	GENERAL_FAIL_AA = 0xaa,
	GENERAL_FAIL_AB = 0xab,
	GENERAL_FAIL_AC = 0xac,
	GENERAL_FAIL_AD = 0xad,
	GENERAL_FAIL_AE = 0xae,
	GENERAL_FAIL_AF = 0xaf,
	GENERAL_FAIL_B0 = 0xb0,
	GENERAL_FAIL_B1 = 0xb1,
	GENERAL_FAIL_B2 = 0xb2,
	GENERAL_FAIL_B3 = 0xb3,
	GENERAL_FAIL_B4 = 0xb4,
	GENERAL_FAIL_B5 = 0xb5,
	GENERAL_FAIL_B6 = 0xb6,
	GENERAL_FAIL_B7 = 0xb7,
	GENERAL_FAIL_B8 = 0xb8,
	GENERAL_FAIL_B9 = 0xb9,
	GENERAL_FAIL_BA = 0xba,
	GENERAL_FAIL_BB = 0xbb,
	GENERAL_FAIL_BC = 0xbc,
	GENERAL_FAIL_BD = 0xbd,
	GENERAL_FAIL_BE = 0xbe,
	GENERAL_FAIL_BF = 0xbf,
	GENERAL_FAIL_C0 = 0xc0,
	GENERAL_FAIL_C1 = 0xc1,
	GENERAL_FAIL_C2 = 0xc2,
	GENERAL_FAIL_C3 = 0xc3,
	GENERAL_FAIL_C4 = 0xc4,
	GENERAL_FAIL_C5 = 0xc5,
	GENERAL_FAIL_C6 = 0xc6,
	GENERAL_FAIL_C7 = 0xc7,
	GENERAL_FAIL_C8 = 0xc8,
	GENERAL_FAIL_C9 = 0xc9,
	GENERAL_FAIL_CA = 0xca,
	GENERAL_FAIL_CB = 0xcb,
	GENERAL_FAIL_CC = 0xcc,
	GENERAL_FAIL_CD = 0xcd,
	GENERAL_FAIL_CE = 0xce,
	GENERAL_FAIL_CF = 0xcf,
	GENERAL_FAIL_D0 = 0xd0,
	GENERAL_FAIL_D1 = 0xd1,
	GENERAL_FAIL_D2 = 0xd2,
	GENERAL_FAIL_D3 = 0xd3,
	GENERAL_FAIL_D4 = 0xd4,
	GENERAL_FAIL_D5 = 0xd5,
	GENERAL_FAIL_D6 = 0xd6,
	GENERAL_FAIL_D7 = 0xd7,
	GENERAL_FAIL_D8 = 0xd8,
	GENERAL_FAIL_D9 = 0xd9,
	GENERAL_FAIL_DA = 0xda,
	GENERAL_FAIL_DB = 0xdb,
	GENERAL_FAIL_DC = 0xdc,
	GENERAL_FAIL_DD = 0xdd,
	GENERAL_FAIL_DE = 0xde,
	GENERAL_FAIL_DF = 0xdf,
	GENERAL_FAIL_E0 = 0xe0,
	GENERAL_FAIL_E1 = 0xe1,
	GENERAL_FAIL_E2 = 0xe2,
	GENERAL_FAIL_E3 = 0xe3,
	GENERAL_FAIL_E4 = 0xe4,
	GENERAL_FAIL_E5 = 0xe5,
	GENERAL_FAIL_E6 = 0xe6,
	GENERAL_FAIL_E7 = 0xe7,
	GENERAL_FAIL_E8 = 0xe8,
	GENERAL_FAIL_E9 = 0xe9,
	GENERAL_FAIL_EA = 0xea,
	GENERAL_FAIL_EB = 0xeb,
	GENERAL_FAIL_EC = 0xec,
	GENERAL_FAIL_ED = 0xed,
	GENERAL_FAIL_EE = 0xee,
	GENERAL_FAIL_EF = 0xef,
	GENERAL_FAIL_F0 = 0xf0,
	GENERAL_FAIL_F1 = 0xf1,
	GENERAL_FAIL_F2 = 0xf2,
	GENERAL_FAIL_F3 = 0xf3,
	GENERAL_FAIL_F4 = 0xf4,
	GENERAL_FAIL_F5 = 0xf5,
	GENERAL_FAIL_F6 = 0xf6,
	GENERAL_FAIL_F7 = 0xf7,
	GENERAL_FAIL_F8 = 0xf8,
	GENERAL_FAIL_F9 = 0xf9,
	GENERAL_FAIL_FA = 0xfa,
	GENERAL_FAIL_FB = 0xfb,
	GENERAL_FAIL_FC = 0xfc,
	GENERAL_FAIL_FD = 0xfd,
	GENERAL_FAIL_FE = 0xfe,
	GENERAL_FAIL_FF = 0xff
};

#endif // !__ENUMS__EAUTHPACKETS_HPP__
