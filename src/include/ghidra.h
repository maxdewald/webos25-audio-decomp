/*
 * Support header for C lifted mechanically from Ghidra pseudocode (tools/lift.py).
 * Defines Ghidra's helper operators (CONCATxy, SUBxy, CARRYx, ...) for ILP32 ARM.
 */
#ifndef GHIDRA_H
#define GHIDRA_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

typedef void code;

/* "x._A_B_": B bytes of x starting at byte A (little-endian), as an lvalue. */
#define PIECE(x, a, b) (*(PIECE_T##b *)((char *)&(x) + (a)))
typedef uint8_t PIECE_T1;
typedef uint16_t PIECE_T2;
typedef uint32_t PIECE_T3;
typedef uint32_t PIECE_T4;
typedef uint64_t PIECE_T8;

#define CONCAT11(h, l) ((uint16_t)(((uint16_t)(uint8_t)(h) << 8) | (uint8_t)(l)))
#define CONCAT12(h, l) ((uint32_t)(((uint32_t)(uint8_t)(h) << 16) | (uint16_t)(l)))
#define CONCAT13(h, l) ((uint32_t)(((uint32_t)(uint8_t)(h) << 24) | ((uint32_t)(l) & 0xffffff)))
#define CONCAT14(h, l) ((uint64_t)(((uint64_t)(uint8_t)(h) << 32) | (uint32_t)(l)))
#define CONCAT21(h, l) ((uint32_t)(((uint32_t)(uint16_t)(h) << 8) | (uint8_t)(l)))
#define CONCAT22(h, l) ((uint32_t)(((uint32_t)(uint16_t)(h) << 16) | (uint16_t)(l)))
#define CONCAT31(h, l) ((uint32_t)(((uint32_t)(h) << 8) | (uint8_t)(l)))
#define CONCAT44(h, l) ((uint64_t)(((uint64_t)(uint32_t)(h) << 32) | (uint32_t)(l)))
#define CONCAT71(h, l) ((uint64_t)(((uint64_t)(h) << 8) | (uint8_t)(l)))

#define SUB21(x, b) ((uint8_t)((uint16_t)(x) >> ((b) * 8)))
#define SUB41(x, b) ((uint8_t)((uint32_t)(x) >> ((b) * 8)))
#define SUB42(x, b) ((uint16_t)((uint32_t)(x) >> ((b) * 8)))
#define SUB43(x, b) ((uint32_t)(((uint32_t)(x) >> ((b) * 8)) & 0xffffff))
#define SUB81(x, b) ((uint8_t)((uint64_t)(x) >> ((b) * 8)))
#define SUB84(x, b) ((uint32_t)((uint64_t)(x) >> ((b) * 8)))
#define ZEXT48(x) ((uint64_t)(uint32_t)(x))
#define ZEXT14(x) ((uint32_t)(uint8_t)(x))
#define ZEXT24(x) ((uint32_t)(uint16_t)(x))
#define SEXT48(x) ((int64_t)(int32_t)(x))

#define CARRY4(a, b) ((uint32_t)(a) + (uint32_t)(b) < (uint32_t)(a))
#define SCARRY4(a, b) __builtin_add_overflow_p((int32_t)(a), (int32_t)(b), (int32_t)0)
#define SBORROW4(a, b) __builtin_sub_overflow_p((int32_t)(a), (int32_t)(b), (int32_t)0)
#define LZCOUNT(x) ((x) ? __builtin_clz((uint32_t)(x)) : 32)
#define POPCOUNT(x) __builtin_popcount((uint32_t)(x))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define NAN(x) ((x) != (x))
#define SQRT(x) __builtin_sqrt(x)

/* VectorSignedToFloat(int, rounding mode): plain int -> float conversion */
#define VectorSignedToFloat(x, rm) ((float)(int32_t)(x))
#define VectorUnsignedToFloat(x, rm) ((float)(uint32_t)(x))

#define builtin_strncpy(d, s, n) memcpy((d), (s), (n))
#define halt_baddata() __builtin_trap()
#define halt_unimplemented() __builtin_trap()

#endif
