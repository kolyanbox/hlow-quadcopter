#include "UTIL.h"

uint8_t explode(char *pcInput, char *pacExploded[], char cExplodeCh)
{
    uint8_t uiCount = 0;

    pacExploded[uiCount++] = pcInput;

    do
    {
        if(*pcInput == cExplodeCh)
        {
            pacExploded[uiCount++] = pcInput + 1;
            *pcInput = 0;
        }

        pcInput++;
    }
    while(*pcInput);

    return uiCount;
}

void Bzero (void *pvBlock, unsigned int uiLen)
{
	Memset(pvBlock, 0, uiLen);
}

void* Memset(void *pvDst, int iValue, unsigned int uiLen)
{
    uint8_t *puiDst = (uint8_t *) pvDst;

    while(uiLen--)
        *puiDst++ = iValue;

    return pvDst;
}

int Memcmp (const void* c_pvBlock1, const void *c_pvBlock2, unsigned int uiLen)
{
    const uint8_t* c_puiBlock1 = (const uint8_t *) c_pvBlock1;
    const uint8_t* c_puiBlock2 = (const uint8_t *) c_pvBlock2;

    while(uiLen--)
        if(*c_puiBlock1++ != *c_puiBlock2++)
            return *(c_puiBlock1 - 1) < *(c_puiBlock2 - 1) ? -1 : 1;

    return 0;
}

void* Memcpy(void *pvDst, const void* c_pvSrc, unsigned int uiLen)
{
	uint8_t *puiDst = (uint8_t *) pvDst;
    const uint8_t *c_puiSrc = (const uint8_t *) c_pvSrc;

    while (uiLen--)
        *puiDst++ = *c_puiSrc++;

    return pvDst;
}

void* Memchr (const void *src_void, int c, unsigned int length)
{
	const uint8_t *src = (const uint8_t *) src_void;

	while (length-- > 0)
	{
		if (*src == c)
			return (void *)src;

		src++;
	}

	return NULL;
}

int Strncmp(const char *c_pcStr1, const char *c_pcStr2, unsigned int uiLen)
{
    uint8_t uiByte1;
    uint8_t uiByte2;

    while(uiLen--)
    {
        uiByte1 = (uint8_t) *c_pcStr1++;
        uiByte2 = (uint8_t) *c_pcStr2++;

        if(uiByte1 != uiByte2)
            return uiByte1 - uiByte2;

        if(uiByte1 == '\0')
            return 0;
    }

    return 0;
}

int Strcmp(const char* c_pcStr1, const char* c_pcStr2)
{
    while(*c_pcStr1 == *c_pcStr2++)
    {
        if(*c_pcStr1++ == '\0')
            return 0;
    }

    return (*c_pcStr1 - *--c_pcStr2);
}

char* Strcpy(char *pcDst, const char *c_pcSrc)
{
    char *pcDstPtr = pcDst;

    while(*c_pcSrc)
        *pcDst++ = *c_pcSrc++;

    *pcDst = '\0';

    return pcDstPtr;
}

int Strlen(const char* c_pcString)
{
    int iLength = 0;

    while(*c_pcString++)
        iLength++;

    return iLength;
}

char * Strcat ( char * dest, const char * src )
{
	char *ret = dest;
	while (*dest)
		dest++;
	while (*dest++ = *src++)
		;
	return ret;
}

int str_is_digit(char* c) {
	int i=0;
	while (c[i] != '\0')
	{
		if( c[i] < '0' || c[i] > '9' )
			return 0;
		i++;
	}
	return 1;
}


int Atoi(char *ap)
{
	register int n = 0;
	register char *p = ap;
	int f = 0;

loop:
	while(*p == ' ' || *p == '	')
        p++;

	if(*p == '-')
    {
		f++;
		p++;
		goto loop;
	}

	while(*p >= '0' && *p <= '9')
		n = n*10 + *p++ - '0';

	if(f)
		n = -n;

	return(n);
}


char* Itoa( int value, char* result, int base )
 {
	// check that the base if valid
	if (base < 2 || base > 16)
	{
		*result = 0;
		return result;
	}

	char* out = result;
	int quotient = value;

	int absQModB;

	do
	{
		// KevinJ - get rid of this dependency
		//*out = "0123456789abcdef"[ std::abs( quotient % base ) ];
		absQModB=quotient % base;

		if (absQModB < 0)
			absQModB = -absQModB;

		*out = "0123456789abcdef"[absQModB];
		++out;

		quotient /= base;
	}
	while(quotient);

	// Only apply negative sign for base 10
	if (value < 0 && base == 10)
		*out++ = '-';

	// KevinJ - get rid of this dependency
	// std::reverse( result, out );
	*out = 0;

	// KevinJ - My own reverse code
	char *start = result;
	char temp;
	out--;

	while (start < out)
	{
		temp =* start;

		*start = *out;
		*out = temp;
		start++;
		out--;
	}

	return result;
}


void Ftoa (float x, char *str, char prec, char format)
{
    int ie, i, k, ndig, fstyle;
    double y;

    /*if (nargs() != 7)
        IEHzap("ftoa  "); */

    ndig = (prec <= 0) ? 7 : (prec > 22 ? 23 : prec + 1);

    if  (format == 'f' || format == 'F')
        fstyle = 1;
    else
        fstyle = 0;

    /* print in e format unless last arg is 'f' */
    ie = 0;
    /* if x negative, write minus and reverse */

    if (x < 0)
    {
        *str++ = '-';
        x = -x;
    }

    /* put x in range 1 <= x < 10 */
    if (x > 0.0) while (x < 1.0)
    {
        x *= 10.0;
        ie--;
    }

    while (x >= 10.0)
    {
        x = x / 10.0;
        ie++;
    }

    /* in f format, number of digits is related to size */
    if (fstyle)
        ndig += ie;

    /* round. x is between 1 and 10 and ndig will be printed to right of decimal point so rounding is ... */
    for (y = i = 1; i < ndig; i++)
        y = y/10.;

    x += y / 2.;

    if (x >= 10.0) {x = 1.0; ie++;} /* repair rounding disasters */
                                    /* now loop.  put out a digit (obtain by multiplying by
                                       10, truncating, subtracting) until enough digits out */

    /* if fstyle, and leading zeros, they go out special */
    if (fstyle && ie < 0)
    {
        *str++ = '0'; *str++ = '.';
      if (ndig < 0) ie = ie-ndig; /* limit zeros if underflow */

      for (i = -1; i > ie; i--)
          *str++ = '0';
    }

    for (i=0; i < ndig; i++)
    {
        k = x;
        *str++ = k + '0';

        if (i == (fstyle ? ie : 0)) /* where is decimal point */
            *str++ = '.';

        x -= (y=k);
        x *= 10.0;
  }

    /* now, in estyle,  put out exponent if not zero */
    if (!fstyle && ie != 0)
    {
        *str++ = 'E';

        if (ie < 0)
        {
            ie = -ie;
            *str++ = '-';
        }

        for (k=100; k > ie; k /=10);

        for (; k > 0; k /=10)
        {
            *str++ = ie/k + '0';
           ie = ie%k;
        }
    }

    *str = '\0';
}

double Atof(const char *str)
{
    return Strtod(str, 0);
}

double Strtod(const char *str, char **endptr)
{
    char *p = (char *) str;
    double number;
    double p10;
    int32_t exponent;
    int32_t negative;
    int32_t n;
    int32_t num_digits;
    int32_t num_decimals;

    /* Skip leading whitespace */
    while (ISSPACE(*p))
        p++;

    /* Handle optional sign */
    negative = 0;

    switch (*p)
    {
        case '-': negative = 1; /* Fall through to increment position */
        case '+': p++;
    }

    number = 0.;
    exponent = 0;
    num_digits = 0;
    num_decimals = 0;

    /* Process string of digits */
    while (ISDIGIT(*p))
    {
        number = number * 10. + (*p - '0');
        p++;
        num_digits++;
    }

    /* Process decimal part */
    if (*p == '.')
    {
        p++;

        while (ISDIGIT(*p))
        {
            number = number * 10. + (*p - '0');
            p++;
            num_digits++;
            num_decimals++;
        }

        exponent -= num_decimals;
    }

    if (num_digits == 0)
        return 0.0;

    /* Correct for sign */
    if (negative)
        number = -number;

    /* Process an exponent string */
    if (*p == 'e' || *p == 'E')
    {
        /* Handle optional sign */
        negative = 0;

        switch(*++p)
        {
            case '-': negative = 1;   /* Fall through to increment pos */
            case '+': p++;
        }

        /* Process string of digits */
        n = 0;

        while (ISDIGIT(*p))
        {
            n = n * 10 + (*p - '0');
            p++;
        }

        if (negative)
            exponent -= n;
        else
            exponent += n;
    }

    if (exponent < DBL_MIN_EXP  || exponent > DBL_MAX_EXP)
        return 0.0;

    // Scale the result
    p10 = 10.;
    n = exponent;

    if (n < 0)
        n = -n;

    while (n)
    {
        if (n & 1)
        {
            if (exponent < 0)
            number /= p10;
            else
            number *= p10;
        }

        n >>= 1;
        p10 *= p10;
    }

    if (endptr)
        *endptr = p;

    return number;
}

long int Strtol(const char *c_pcStr, char **ppcEnd, int iBase)
{
	const char *pcStr = c_pcStr;
    char cCurrent;
	unsigned long ulAcc;
    unsigned long ulCutoff;
	int iNeg = 0, iAny, iCutlim;

	do
		cCurrent = *pcStr++;
	while (ISSPACE(cCurrent));

	if (cCurrent == '-')
	{
		iNeg = 1;
		cCurrent = *pcStr++;
	}
	else if (cCurrent == '+')
		cCurrent = *pcStr++;


	if ((iBase == 0 || iBase == 16) && cCurrent == '0' && (*pcStr == 'x' || *pcStr == 'X'))
	{
		cCurrent = pcStr[1];
		pcStr += 2;
		iBase = 16;
	}
	if (iBase == 0)
		iBase = cCurrent == '0' ? 8 : 10;

	ulCutoff = iNeg ? -(unsigned long) LONG_MIN : LONG_MAX;
	iCutlim = ulCutoff % (unsigned long) iBase;
	ulCutoff /= (unsigned long) iBase;

	for (ulAcc = 0, iAny = 0;; cCurrent = *pcStr++)
	{
		if (ISDIGIT(cCurrent))
			cCurrent -= '0';
		else if (ISALPHA(cCurrent))
			cCurrent -= ISUPPER(cCurrent) ? 'A' - 10 : 'a' - 10;
		else
			break;

		if (cCurrent >= iBase)
			break;

		if (iAny < 0 || ulAcc > ulCutoff || (ulAcc == ulCutoff && cCurrent > iCutlim))
			iAny = -1;
		else
		{
			iAny = 1;
			ulAcc *= iBase;
			ulAcc += cCurrent;
		}
	}

	if (iAny < 0)
		ulAcc = iNeg ? LONG_MIN : LONG_MAX;
	else if (iNeg)
		ulAcc = -ulAcc;

	if (ppcEnd != NULL)
		*ppcEnd = (char *) (iAny ? pcStr - 1 : c_pcStr);

	return (ulAcc);
}

int __IsNan(double d)
{
	float f = d;

	if ((*((long *) &f) & 0x7f800000) == 0x7f800000 && (*((long *) &f) & 0x007fffff) != 0)
		return 1;

	return 0;
}

double Modf(double value, double *iptr)
{
    struct f64 *f64p;
    double tmp;
    int exp;
    int mask_bits;
    uint32_t mant;

    f64p = (struct f64 *)&value;

    exp= F64_GET_EXP(f64p);
    exp -= F64_EXP_BIAS;

    if (exp < 0)
    {
        *iptr= 0;
        return value;
    }
    mask_bits= 52-exp;

    if (mask_bits <= 0)
    {
        *iptr= value;
        return 0;
    }

    tmp= value;

    if (mask_bits >= 32)
    {
        F64_SET_MANT_LOW(f64p, 0);
        mask_bits -= 32;
        mant= F64_GET_MANT_HIGH(f64p);
        mant &= ~((1 << mask_bits)-1);
        F64_SET_MANT_HIGH(f64p, mant);
    }
    else
    {
        mant= F64_GET_MANT_LOW(f64p);
        mant &= ~((1 << mask_bits)-1);
        F64_SET_MANT_LOW(f64p, mant);
    }

    *iptr= value;

    return tmp-value;
}


static double Sinus(double x, int cos_flag)
{
    static double r[] =
    {
        -0.16666666666666665052e+0,
        0.83333333333331650314e-2,
        -0.19841269841201840457e-3,
        0.27557319210152756119e-5,
        -0.25052106798274584544e-7,
        0.16058936490371589114e-9,
        -0.76429178068910467734e-12,
        0.27204790957888846175e-14
    };

    double  y;
    int     neg = 1;

    if (__IsNan(x))
    {
        return x;
    }

    if (x < 0)
    {
        x = -x;
        neg = -1;
    }
    if (cos_flag)
    {
        neg = 1;
        y = M_PI_2 + x;
    }
    else
        y = x;

    /* ??? avoid loss of significance, if y is too large, error ??? */

    y = y * M_1_PI + 0.5;

    if (y >= DBL_MAX / M_PI)
        return 0.0;

    /*      Use extended precision to calculate reduced argument.
    Here we used 12 bits of the mantissa for a1.
    Also split x in integer part x1 and fraction part x2.
    */
#define A1 3.1416015625
#define A2 -8.908910206761537356617e-6
    {
        double x1, x2;

        Modf(y, &y);
        if (Modf(0.5*y, &x1))
            neg = -neg;

        if (cos_flag)
            y -= 0.5;

        x2 = Modf(x, &x1);
        x = x1 - y * A1;
        x += x2;
        x -= y * A2;

#undef A1
#undef A2
    }

    if (x < 0)
    {
        neg = -neg;
        x = -x;
    }

    /* ??? avoid underflow ??? */

    y = x * x;
    x += x * y * POLYNOM7(y, r);

    return neg==-1 ? -x : x;
}

double Sin(double x)
{
    return Sinus(x, 0);
}

double Cos(double x)
{
    if (x < 0)
        x = -x;

    return Sinus(x, 1);
}

double Atan(double x)
{
	static double p[] =
	{
		 -0.13688768894191926929e+2,
		 -0.20505855195861651981e+2,
		 -0.84946240351320683534e+1,
		 -0.83758299368150059274e+0
	};
	static double q[] =
	{
		  0.41066306682575781263e+2,
		  0.86157349597130242515e+2,
		  0.59578436142597344465e+2,
		  0.15024001160028576121e+2,
		  1.0
	};
	static double a[] =
	{
		 0.0,
		 0.52359877559829887307710723554658381,  /* pi/6 */
		 M_PI_2,
		 1.04719755119659774615421446109316763   /* pi/3 */
	};

	int     neg = x < 0;
	int     n;
	double  g;

	if (__IsNan(x))
	{
		return x;
	}

	if (neg)
	{
		x = -x;
	}

	if (x > 1.0)
	{
		x = 1.0/x;
		n = 2;
	}
	else
		n = 0;

	if (x > 0.26794919243112270647)
	{
		n = n + 1;
		x = (((0.73205080756887729353*x-0.5)-0.5)+x) / (1.73205080756887729353+x);
	}

	g = x * x;

	x += x * g * POLYNOM3(g, p) / POLYNOM4(g, q);

	if (n > 1)
		x = -x;

	x += a[n];

	return neg ? -x : x;
}

double Atan2(double y, double x)
{
	double absx, absy, val;

	if (x == 0 && y == 0)
	{
		return 0;
	}

	absy = y < 0 ? -y : y;
	absx = x < 0 ? -x : x;

	if (absy - absx == absy)
	{
		return y < 0 ? -M_PI_2 : M_PI_2;
	}

	if (absx - absy == absx)
	{
		val = 0.0;
	}
	else
		val = Atan(y/x);

	if (x > 0)
	{
		return val;
	}

	if (y < 0)
	{
		return val - M_PI;
	}

	return val + M_PI;
}

double Sqrt(double x)
{
	int exponent;
	double val;

	if (__IsNan(x))
	{
		return x;
	}

	if (x <= 0)
	{
		return 0;
	}

	if (x > DBL_MAX)
		return x;

	val = Frexp(x, &exponent);

	if (exponent & 1)
	{
		exponent--;
		val *= 2;
	}

	val = Ldexp(val + 1.0, exponent/2 - 1);

#define NITER   5

	for (exponent = NITER - 1; exponent >= 0; exponent--)
	{
		val = (val + x / val) / 2.0;
	}

#undef NITER

	return val;
}


double Frexp(double value, int *eptr)
{
	struct f64 *f64p;
	int exp, exp_bias;
	double factor;

	f64p= (struct f64 *)&value;
	exp_bias= 0;

	exp= F64_GET_EXP(f64p);

	if (exp == F64_EXP_MAX)
	{
		 *eptr= 0;
		 return value;
	}
	if (exp == 0)
	{
		if (F64_GET_MANT_LOW(f64p) == 0 && F64_GET_MANT_HIGH(f64p) == 0)
		{
			*eptr= 0;
			return value;
		}

		/* Multiply by 2^64 */
		factor= 65536.0;        /* 2^16 */
		factor *= factor;       /* 2^32 */
		factor *= factor;       /* 2^64 */
		value *= factor;
		exp_bias= 64;
		exp= F64_GET_EXP(f64p);
	}

	exp= exp - F64_EXP_BIAS - exp_bias + 1;
	*eptr= exp;
	F64_SET_EXP(f64p, F64_EXP_BIAS-1);

	return value;
}

double __huge_val(void)
{
	return DBL_MAX;
}

double Ldexp(double fl, int exp)
{
	int sign = 1;
	int currexp;

	if (__IsNan(fl))
	{
		return fl;
	}

	if (fl == 0.0)
		return 0.0;

	if (fl<0)
	{
		fl = -fl;
		sign = -1;
	}

	if (fl > DBL_MAX)
	{
		return sign * fl;
	}

	fl = Frexp(fl,&currexp);

	exp += currexp;

	if (exp > 0)
	{
		if (exp > DBL_MAX_EXP)
		{
			return sign * HUGE_VAL;
		}

		while (exp>30)
		{
			fl *= (double) (1L << 30);
			exp -= 30;
		}

		fl *= (double) (1L << exp);
	}
	else
	{
		if (exp < DBL_MIN_EXP - DBL_MANT_DIG)
		{
			return 0.0;
		}

		while (exp<-30)
		{
			fl /= (double) (1L << 30);
			exp += 30;
		}

		fl /= (double) (1L << -exp);
	}

	return sign * fl;
}

static double Asin_acos(double x, int cosfl)
{
	int negative = x < 0;
	int     i;
	double  g;

	static double p[] =
	{
		-0.27368494524164255994e+2,
		0.57208227877891731407e+2,
		-0.39688862997540877339e+2,
		0.10152522233806463645e+2,
		-0.69674573447350646411e+0
	};

	static double q[] =
	{
		-0.16421096714498560795e+3,
		0.41714430248260412556e+3,
		-0.38186303361750149284e+3,
		0.15095270841030604719e+3,
		-0.23823859153670238830e+2,
		1.0
	};

	if (__IsNan(x))
	{
		return x;
	}


	if (negative)
	{
		x = -x;
	}

	if (x > 0.5)
	{
		i = 1;

		if (x > 1)
		{
			return 0;
		}
		g = 0.5 - 0.5 * x;
		x = - Sqrt(g);

		x += x;
	}
	else
	{
		i = 0;
		g = x * x;
	}

	x += x * g * POLYNOM4(g, p) / POLYNOM5(g, q);

	if (cosfl)
	{
		if (! negative) x = -x;
	}

	if ((cosfl == 0) == (i == 1))
	{
		x = (x + M_PI_4) + M_PI_4;
	}
	else if (cosfl && negative && i == 1)
	{
		x = (x + M_PI_2) + M_PI_2;
	}

	if (! cosfl && negative)
		x = -x;

	return x;
}

double Asin(double x)
{
	return Asin_acos(x, 0);
}

double Acos(double x)
{
	return Asin_acos(x, 1);
}

double Tan(double x)
{
	int negative = x < 0;
	int invert = 0;
	double  y;

	static double p[] =
	{
		1.0,
		-0.13338350006421960681e+0,
		0.34248878235890589960e-2,
		-0.17861707342254426711e-4
	};

	static double q[] =
	{
		1.0,
		-0.46671683339755294240e+0,
		0.25663832289440112864e-1,
		-0.31181531907010027307e-3,
		0.49819433993786512270e-6
	};

	if (__IsNan(x))
	{
		return x;
	}

	if (negative)
		x = -x;

	y = x * M_2_PI + 0.5;

	if (y >= DBL_MAX / M_PI_2)
		return 0.0;

#define A1 1.57080078125
#define A2 -4.454455103380768678308e-6
	{
		double x1, x2;

		Modf(y, &y);

		if (Modf(0.5*y, &x1))
			invert = 1;

		x2 = Modf(x, &x1);

		x = x1 - y * A1;
		x += x2;
		x -= y * A2;
#undef A1
#undef A2
	}

	y = x * x;

	x += x * y * POLYNOM2(y, p+1);

	y = POLYNOM4(y, q);

	if (negative)
		x = -x;

	return invert ? -y/x : x/y;
}

double Tanh(double x)
{
	static double p[] =
	{
		-0.16134119023996228053e+4,
		-0.99225929672236083313e+2,
		-0.96437492777225469787e+0
	};

	static double q[] =
	{
		0.48402357071988688686e+4,
		0.22337720718962312926e+4,
		0.11274474380534949335e+3,
		1.0
	};

	int negative = x < 0;

	if (__IsNan(x))
	{
		return x;
	}

	if (negative)
		x = -x;

	if (x >= 0.5*M_LN_MAX_D)
	{
		x = 1.0;
	}

#define LN3D2   0.54930614433405484570e+0       /* ln(3)/2 */

	else if (x > LN3D2)
	{
		x = 0.5 - 1.0/(Exp(x+x)+1.0);
		x += x;
	}
	else
	{
		double g = x*x;
		x += x * g * POLYNOM2(g, p)/POLYNOM3(g, q);
	}
#undef LN3D2

	return negative ? -x : x;
}


static double Sinh_cosh(double x, int cosh_flag)
{
	static double p[] = {
		-0.35181283430177117881e+6,
		-0.11563521196851768270e+5,
		-0.16375798202630751372e+3,
		-0.78966127417357099479e+0
	};

	static double q[] = {
		-0.21108770058106271242e+7,
		0.36162723109421836460e+5,
		-0.27773523119650701167e+3,
		1.0
	};

	int     negative = x < 0;
	double  y = negative ? -x : x;

	if (__IsNan(x))
	{
		return x;
	}

	if (! cosh_flag && y <= 1.0)
	{
		y = y * y;
		return x + x * y * POLYNOM3(y, p)/POLYNOM3(y,q);
	}

	if (y >= M_LN_MAX_D)
	{
#define LNV     0.69316101074218750000e+0
#define VD2M1   0.52820835025874852469e-4

		double  w = y - LNV;

		if (w < M_LN_MAX_D+M_LN2-LNV)
		{
			x = Exp(w);
			x += VD2M1 * x;
		}
		else
		{
			x = HUGE_VAL;
		}

#undef LNV
#undef VD2M1
	}
	else
	{
		double  z = Exp(y);

		x = 0.5 * (z + (cosh_flag ? 1.0 : -1.0)/z);
	}

	return negative ? -x : x;
}

double Sinh(double x)
{
	return Sinh_cosh(x, 0);
}

double Cosh(double x)
{
	if (x < 0) x = -x;
		return Sinh_cosh(x, 1);
}


double Floor(double x)
{
	double val;
	return Modf(x, &val) < 0 ? val - 1.0 : val ;
}

double Pow(double x, double y)
{
	double dummy;
	int     result_neg = 0;

	if ((x == 0 && y == 0) || (x < 0 && Modf(y, &dummy) != 0))
	{
		return 0;
	}

	if (x == 0)
		return x;

	if (x < 0)
	{
		if (Modf(y/2.0, &dummy) != 0)
		{
			result_neg = 1;
		}

		x = -x;
	}

	x = Log(x);

	if (x < 0)
	{
		x = -x;
		y = -y;
	}

	if (x > 1.0 && y > DBL_MAX/x)
	{
		return result_neg ? -HUGE_VAL : HUGE_VAL;
	}

	x = Exp(x * y);

	return result_neg ? -x : x;
}

double Log(double x)
{
	static double a[] =
	{
		-0.64124943423745581147e2,
		0.16383943563021534222e2,
		-0.78956112887491257267e0
	};

	static double b[] =
	{
		-0.76949932108494879777e3,
		0.31203222091924532844e3,
		-0.35667977739034646171e2,
		1.0
	};

	double  znum, zden, z, w;
	int     exponent;

	if (__IsNan(x))
	{
		return x;
	}

	if (x < 0)
	{
		return -HUGE_VAL;
	}
	else if (x == 0)
	{
		return -HUGE_VAL;
	}

	if (x <= DBL_MAX)
	{
	}
	else
		return x;

	x = Frexp(x, &exponent);

	if (x > M_1_SQRT2)
	{
		znum = (x - 0.5) - 0.5;
		zden = x * 0.5 + 0.5;
	}
	else
	{
		znum = x - 0.5;
		zden = znum * 0.5 + 0.5;
		exponent--;
	}

	z = znum / zden;
	w = z * z;
	x = z + z * w * (POLYNOM2(w,a) / POLYNOM3(w,b));
	z = exponent;

	x += z * (-2.121944400546905827679e-4);

	return x + z * 0.693359375;
}

double Log10(double x)
{
	if (__IsNan(x))
	{
		return x;
	}

	if (x < 0)
	{
		return -HUGE_VAL;
	}
	else if (x == 0)
	{
		return -HUGE_VAL;
	}

	return Log(x) / M_LN10;
}

double Exp(double x)
{
	static double p[] =
	{
		0.25000000000000000000e+0,
		0.75753180159422776666e-2,
		0.31555192765684646356e-4
	};

	static double q[] =
	{
		0.50000000000000000000e+0,
		0.56817302698551221787e-1,
		0.63121894374398503557e-3,
		0.75104028399870046114e-6
	};

	double  xn, g;
	int     n;
	int     negative = x < 0;

	if (__IsNan(x))
	{
		return x;
	}

	if (x < M_LN_MIN_D)
	{
		return 0.0;
	}

	if (x > M_LN_MAX_D)
	{
		return HUGE_VAL;
	}

	if (negative)
		x = -x;

	n = x * M_LOG2E + 0.5;

	xn = n;

	{
		double  x1 = (long) x;
		double  x2 = x - x1;

		g = ((x1-xn*0.693359375)+x2) - xn*(-2.1219444005469058277e-4);
	}

	if (negative)
	{
		g = -g;
		n = -n;
	}

	xn = g * g;

	x = g * POLYNOM2(xn, p);

	n += 1;

	return (Ldexp(0.5 + x/(POLYNOM3(xn, q) - x), n));
}

double Ceil(double x)
{
	double val;

	return Modf(x, &val) > 0 ? val + 1.0 : val ;
}

double Fabs(double x)
{
	return  x < 0 ? -x : x;
}

char * strtok_r(char *s, const char *delim, char **last)
{
	char *spanp;
	int c, sc;
	char *tok;


	if (s == NULL && (s = *last) == NULL)
		return (NULL);

	/*
	 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	 */
cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
		if (c == sc)
			goto cont;
	}

	if (c == 0) {		/* no non-delimiter characters */
		*last = NULL;
		return (NULL);
	}
	tok = s - 1;

	/*
	 * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 * Note that delim must have one NUL; we stop if we see that, too.
	 */
	for (;;) {
		c = *s++;
		spanp = (char *)delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*last = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}



char *strtok(char *s, const char *delim)
{
	static char *last;

	return strtok_r(s, delim, &last);
}

