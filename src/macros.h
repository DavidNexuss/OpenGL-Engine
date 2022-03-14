#pragma once
#define ENUM_OPERATORS(T) \
inline T operator~ (T a) { return (T)~(int)a; } \
inline T operator| (T a, T b) { return (T)((int)a | (int)b); } \
inline T operator& (T a, T b) { return (T)((int)a & (int)b); } \
inline T operator^ (T a, T b) { return (T)((int)a ^ (int)b); } \
inline T& operator|= (T& a, T b) { return (T&)((int&)a |= (int)b); } \
inline T& operator&= (T& a, T b) { return (T&)((int&)a &= (int)b); } \
inline T& operator^= (T& a, T b) { return (T&)((int&)a ^= (int)b); }
