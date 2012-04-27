#ifndef UTIL_H_
#define UTIL_H_

#include "lpc_types.h"
#include "lpc17xx_uart.h"

/* Limits defs */
#ifndef ULONG_MAX
#	define	ULONG_MAX	((unsigned long)(~0L))
#endif

#ifndef LONG_MAX
#	define	LONG_MAX	((long)(ULONG_MAX >> 1))
#endif

#ifndef LONG_MIN
#	define	LONG_MIN	((long)(~LONG_MAX))
#endif

/* CTYPE defs */
#define ISLOWER(c)  	(c >=  'a' && c <= 'z')
#define ISUPPER(c)  	(c >=  'A' && c <= 'Z')
#define TOUPPER(c)  	(ISLOWER(c) ? (c - 'a' + 'A') : (c))
#define TOLOWER(c)  	(ISUPPER(c) ? (c - 'A' + 'a') : (c))
#define ISDIGIT(x) 		((x) >= '0' && (x) <= '9')
#define ISALPHA(c)  	(ISUPPER(c) || ISLOWER(c))
#define ISSPACE(c)  	(c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')


/* Double defs */
#define DBL_MIN_EXP     (-1021)
#define DBL_MAX_EXP     (1024)
#define DBL_MAX        	(1.7976931348623157e+308)
#define DBL_MANT_DIG 	(53)
#define HUGE_VAL		(__huge_val())

/* Math defs */
#define POLYNOM1(x, a)  ((a)[1]*(x)+(a)[0])
#define POLYNOM2(x, a)  (POLYNOM1((x),(a)+1)*(x)+(a)[0])
#define POLYNOM3(x, a)  (POLYNOM2((x),(a)+1)*(x)+(a)[0])
#define POLYNOM4(x, a)  (POLYNOM3((x),(a)+1)*(x)+(a)[0])
#define POLYNOM5(x, a)  (POLYNOM4((x),(a)+1)*(x)+(a)[0])
#define POLYNOM6(x, a)  (POLYNOM5((x),(a)+1)*(x)+(a)[0])
#define POLYNOM7(x, a)  (POLYNOM6((x),(a)+1)*(x)+(a)[0])
#define POLYNOM8(x, a)  (POLYNOM7((x),(a)+1)*(x)+(a)[0])
#define POLYNOM9(x, a)  (POLYNOM8((x),(a)+1)*(x)+(a)[0])
#define POLYNOM10(x, a) (POLYNOM9((x),(a)+1)*(x)+(a)[0])
#define POLYNOM11(x, a) (POLYNOM10((x),(a)+1)*(x)+(a)[0])
#define POLYNOM12(x, a) (POLYNOM11((x),(a)+1)*(x)+(a)[0])
#define POLYNOM13(x, a) (POLYNOM12((x),(a)+1)*(x)+(a)[0])
#define M_PI            3.14159265358979323846264338327950288
#define M_2PI           6.28318530717958647692528676655900576
#define M_3PI_4         2.35619449019234492884698253745962716
#define M_PI_2          1.57079632679489661923132169163975144
#define M_3PI_8         1.17809724509617246442349126872981358
#define M_PI_4          0.78539816339744830961566084581987572
#define M_PI_8          0.39269908169872415480783042290993786
#define M_1_PI          0.31830988618379067153776752674502872
#define M_2_PI          0.63661977236758134307553505349005744
#define M_4_PI          1.27323954473516268615107010698011488
#define M_E             2.71828182845904523536028747135266250
#define M_LOG2E         1.44269504088896340735992468100189213
#define M_LOG10E        0.43429448190325182765112891891660508
#define M_LN2           0.69314718055994530941723212145817657
#define M_LN10          2.30258509299404568401799145468436421
#define M_SQRT2         1.41421356237309504880168872420969808
#define M_1_SQRT2       0.70710678118654752440084436210484904
#define M_EULER         0.57721566490153286060651209008240243
#define M_LN_MAX_D      (M_LN2 * DBL_MAX_EXP)
#define M_LN_MIN_D      (M_LN2 * (DBL_MIN_EXP - 1))

/* ieee_float defs */
#define F64_EXP_SHIFT               20
#define F64_EXP_MASK                0x7ff
#define F64_EXP_BIAS                1023
#define F64_MANT_SHIFT              0
#define F64_MANT_MASK               0xfffff
#define F64_GET_MANT_LOW(fp)        ((fp)->low_word)
#define F64_SET_MANT_LOW(fp, val)   ((fp)->low_word= (val))
#define F64_GET_EXP(fp)             (((fp)->high_word >> F64_EXP_SHIFT) & F64_EXP_MASK)
#define F64_GET_MANT_HIGH(fp)       (((fp)->high_word >> F64_MANT_SHIFT) & F64_MANT_MASK)
#define F64_SET_MANT_HIGH(fp, val)  ((fp)->high_word= ((fp)->high_word & ~(F64_MANT_MASK << F64_MANT_SHIFT)) | (((val) & F64_MANT_MASK) << F64_MANT_SHIFT))
#define F64_EXP_MAX     			2047
#define F64_SET_EXP(fp, val)    	((fp)->high_word= ((fp)->high_word & ~(F64_EXP_MASK << F64_EXP_SHIFT)) | (((val) & F64_EXP_MASK) << F64_EXP_SHIFT))

/* ieee_float */
 struct f64
 {
         uint32_t low_word;
         uint32_t high_word;
 };

 /* Memory related functions */
extern void		Bzero(void *pvBlock, unsigned int uiLen);
extern void* 	Memset(void *pvDst, int iValue, unsigned int uiLen);
extern int 		Memcmp(const void* c_pvBlock1, const void *c_pvBlock2, unsigned int uiLen);
extern void* 	Memcpy(void *pvDst, const void* c_pvSrc, unsigned int uiLen);
extern void* 	Memchr(const void *src_void, int c, unsigned int length);
extern int 		Strcmp(const char* c_pcStr1, const char* c_pcStr2);
extern char* 	Strcpy(char *pcDst, const char *c_pcSrc);
extern int 		Strlen(const char* c_pcString);
extern int 		Strncmp(const char *c_pcStr1, const char *c_pcStr2, unsigned int uiLen);

/* Conversion related functions */
extern long int	Strtol(const char *c_pcStr, char **ppcEnd, int iBase);
extern double 	Strtod(const char *str, char **endptr);
extern int 		Atoi(char *ap);
extern double	Atof(const char *str);
extern char* 	Itoa(int value, char* result, int base);
extern void 	Ftoa(float x, char *str, char prec, char format);

/* Math related functions */
extern int 		__IsNan(double d);
extern double	__huge_val(void);
extern double 	Modf(double value, double *iptr);
extern double 	Sin(double x);
extern double 	Cos(double x);
extern double 	Tan(double x);
extern double 	Sinh(double x);
extern double 	Cosh(double x);
extern double 	Tanh(double x);
extern double 	Atan(double x);
extern double 	Atan2(double y, double x);
extern double 	Asin(double x);
extern double 	Acos(double x);
extern double 	Sqrt(double x);
extern double 	Floor(double x);
extern double 	Pow(double x, double y);
extern double 	Exp(double x);
extern double	Log(double x);
extern double 	Log10(double x);
extern double 	Ceil(double x);
extern double 	Fabs(double x);
extern double 	Ldexp(double fl, int exp);
extern double	Frexp(double value, int *eptr);

/* Other functions */
extern uint8_t	explode(char *pcInput, char *pacExploded[], char cExplodeCh);


#endif
