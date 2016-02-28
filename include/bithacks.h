#pragma once

/* Bit twiddling hacks

	A collection of header-only (for inlining) functions, that encode
	bit twiddling hacks that may help speed up specific operations.

	To use:
		The optimisation options are determined by macro definitions at the top of the file.
		Set the flags according to your specific system for optimal performance.
		If no flags are set, then a default safe option is used.

	Source:
		http://www.graphics.stanford.edu/~seander/bithacks.html
	
	Testing check list:

	To do:
		make functions default to a std library solution if bit hacks are turned off.
		finish functions
		add rest of bits
		add short overloads
		add long overloads
		test all functions
		add explanations of bit hacks to title documentations
		consider passing result in with other arguments.
		replace SYSTEM_BITS with overloads of the respective types intXX_t where XX is the bit number
*/

// FLAG					VALUE		//	EFFECT			|	DESCRIPTION
#define BIT_HACKING		1			//	NOT IMPLEMENTED	|	Turns off all bit hacks.
#define CHAR_BIT		8			//	Tune some funcs	|	CHAR_BIT is the number of bits per byte (normally 8).
#define SYSTEM_BITS		32			// 
#define FLAG_REGISTERS	0			//	speed			|	Avoid branching on CPUs with flag registers (IA32).								sign01, sign101	
#define R_SHIFT			0			// (quickest)		|	One less instruction (but not portable).										sign01, sign101	
#define NO_INT_OVERFLOW_EXCEPTION 1 // ++speed			|	
#define POWER_OF_2_0	0			// ++speed			|	if 1, zero is incorrectly considered a power of 2
#define NOT_SAME_MEM	1			// ++speed			|	Guarantee that arguments a and b do not point to the same memory location.

// MACROS
#if BIT_HACKING
#define ADD_IF_BIT_HACKING(x) x
#define REM_IF_BIT_HACKING(x)
#else
#define ADD_IF_BIT_HACKING(x)
#define REM_IF_BIT_HACKING(x) x
#endif

#if NO_INT_OVERFLOW_EXCEPTION
#define ADD_IF_NO_INT_OVERFLOW_EXCEPTION(x) x
#define REM_IF_NO_INT_OVERFLOW_EXCEPTION(x)
#else
#define ADD_IF_NO_INT_OVERFLOW_EXCEPTION(x)
#define REM_IF_NO_INT_OVERFLOW_EXCEPTION(x) x
#endif

#if NOT_SAME_MEM
#define ADD_IF_NOT_SAME_MEM(x) x
#define REM_IF_NOT_SAME_MEM(x)
#else
#define ADD_IF_NOT_SAME_MEM(x)
#define REM_IF_NOT_SAME_MEM(x) x
#endif


/* HACKS
	FUNCTION			| DESCRIPTION																| BENCHMARKS
	=========================================================================================================
	sign				| Return +1 if postive (or 0) and -1 if negative.							|
	sign01				| Return  0 if postive and 1 if negative.									|
	sign101				| Return +1 if postive, 0 if 0 and -1 if negative.							|
	non_negative		| Return +1 if posiive, otherwise return 0.									|
	opposite_signs		| Return true if opposite signs and false otherwise.						|
	abs					| Return the absolute value of int.											|

	min					| Return the minimum of x or y without branching.							|
	max					| Return the maximum of x or y without branching.							|

	power_of_2			| Return true if x is a power of 2.											|
	power_of_2_0		| Quicker version of power_of_2, but incorrectly considers 0 a power of 2.	|

	round_to_power_of_2	| Round integers to a power of 2											|

	swap				| Swap a and b WITHOUT using a temporary variable, using add and subract.	|
	swap_xor			| Swap a and b WITHOUT using a temporary variable, using the xor operation.	|
	swap_not_meme_item	| Faster variant of swap, but assumes that a != b.							|
	swap_bits			|

	log2				|
*/

namespace HACKTASTIC {


//-------------------------------------------------------------------------------------------------
// Sign
//		The sign of an int is stored in the 1st bit, so most functions work by shifting to 1st bit
//		and then comparing to 1.
//-------------------------------------------------------------------------------------------------


// Return +1 if postive (or 0) and -1 if negative.
int sign(int x) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept) { return +1 | (x >> (sizeof(int) * CHAR_BIT - 1)); }

// Return 0 if postive and 1 if negative.
int sign01(int x) noexcept
{
#if FLAG_REGISTERS && R_SHIFT
	return -(int)((unsigned int)((int)x) >> (sizeof(int) * CHAR_BIT - 1));
#elif R_SHIFT
	return x >> (sizeof(int) * CHAR_BIT - 1);
#else:
	return -(x < 0);
#endif
}

// Return +1 if x is postive, 0 if 0 and -1 if negative.
int sign101(int x) noexcept
{
#if FLAG_REGISTERS && R_SHIFT
	return (x != 0) | -(int)((unsigned int)((int)x) >> (sizeof(int) * CHAR_BIT - 1));
#elif R_SHIFT
	return (x != 0) | (x >> (sizeof(int) * CHAR_BIT - 1));
#else:
	return (x > 0) - (x < 0);
#endif
}

// Return true if opposite signs and false otherwise.
int non_negative(int x) { return 1 ^ ((unsigned int)x >> (sizeof(int) * CHAR_BIT - 1)); }

// Return true if x and y have opposite signs.
bool opposite_signs(int x, int y) noexcept { return ((x ^ y) < 0); }

// Return the absolute value of int.
int abs(int v) noexcept {
	int const mask = v >> (sizeof(int) * CHAR_BIT - 1);	
	return (v ^ mask) - mask;
}


//-------------------------------------------------------------------------------------------------
// Minimum and maximum (branchless)
//-------------------------------------------------------------------------------------------------


// Return the minimum of x or y without branching.
 int min( int x,  int y) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept)
{
#if NO_INT_OVERFLOW_EXCEPTION
	return y + ((x - y) & ((x - y) >> (sizeof(int) * CHAR_BIT - 1)));
#else
	return y ^ ((x ^ y) & -(x < y));
#endif
}
uint min(uint x, uint y) noexcept 
{
#if NO_INT_OVERFLOW_EXCEPTION
	return y + ((x - y) & ((x - y) >> (sizeof(int) * CHAR_BIT - 1)));
#else
	return y ^ ((x ^ y) & -(x < y));
#endif
}

// Return the maximum of x or y without branching.
 int max( int x,  int y) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept) 
{
#if NO_INT_OVERFLOW_EXCEPTION
	return x - ((x - y) & ((x - y) >> (sizeof(int) * CHAR_BIT - 1)));
#else
	return x ^ ((x ^ y) & -(x < y));
#endif
}
uint max(uint x, uint y) noexcept 
{
#if NO_INT_OVERFLOW_EXCEPTION
	return x - ((x - y) & ((x - y) >> (sizeof(int) * CHAR_BIT - 1)));
#else
	return x ^ ((x ^ y) & -(x < y));
#endif
}


//-------------------------------------------------------------------------------------------------
// Powers of Two
//		If an int (or uint) is a power of two, then in binary it's a 1 followed by 0s, 
//		e.g. 2 = 0010, 4 = 0100 and 8 = 1000 (ignoring irrevelant leading zeros).
//
//					Binary					|		Decimal
//				 x	= 0100					|		x = 4
//			 x - 1	= 0011					|	x - 1 = 3
//		x & (x - 1)	= 0100 & 0011 = 0000	|		  = 0
//		By then checking if x & (x - 1) == 0, we get true if x is a power of 2 and false otherwise.
//		If x = 0, then we incorrectly return true. We counter this by checking x is not zero first.
//
//		Note: modern CPUs operate on the entire register at once, so this is optimal even though
//		naively you might try to just check that x's binary representation only contains one 1.
//-------------------------------------------------------------------------------------------------


// Return true if x is a power of 2.
// Return false otherwise.
bool power_of_2( int x) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept) {
#if POWER_OF_2_0						
	return (x & (x - 1)) == 0;			
#else									
	return x && !(x & (x - 1));			
#endif
}
bool power_of_2(uint x) noexcept {
#if POWER_OF_2_0						
	return (x & (x - 1)) == 0;
#else									
	return x && !(x & (x - 1));
#endif 
}

// Faster variant of power_of_2(x), however x=0 is incorrectly considered a power of 2.
// Return true if x is a power of 2. 
// Return false otherwise.
bool power_of_2_0( int x) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept) { return (x & (x - 1)) == 0; }
bool power_of_2_0(uint x) noexcept { return (x & (x - 1)) == 0; }


//-------------------------------------------------------------------------------------------------
// Round to a Power of Two
//-------------------------------------------------------------------------------------------------


// Round integers to a power of 2
 int32_t round_to_power_of_2( int32_t n) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept)
{
	--n;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	++n;
	return n;
}
uint32_t round_to_power_of_2(uint32_t n) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept)
{
	--n;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	++n;
	return n;
}
 int64_t round_to_power_of_2( int64_t n) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept)
{
	--n;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n |= n >> 32;
	++n;
	return n;
}
uint64_t round_to_power_of_2(uint64_t n) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept)
{
	--n;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n |= n >> 32;
	++n;
	return n;
}


//-------------------------------------------------------------------------------------------------
// SWAP
//		Do not use the macros on floats, unless using their raw binary form.
//-------------------------------------------------------------------------------------------------


#define SWAP_ADD(a, b) (((a) -= (b)), ((b) += (a)), ((a) = (b) - (a))))
#define SWAP_ADD_SAME_MEM(a,b) ((&(a) == &(b)) || (((a) -= (b)), ((b) += (a)), ((a) = (b) - (a))))
#define SWAP_XOR(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))
#define SWAP_XOR_SAME_MEM(a, b) (((a) ^ (b)) && ((b) ^= (a) ^= (b), (a) ^= (b)))

// Swap a and b WITHOUT using a temporary variable, using addition and subraction.
// Note if overflow exceptions are enabled, then a and b are converted to unsigned and then swapped.
void swap( int a,  int b) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept) {
	ADD_IF_NO_INT_OVERFLOW_EXCEPTION((REM_IF_NOT_SAME_MEM((&(a) == &(b)) ||) ((a -= b), (b += a), (a = b-a))));
	REM_IF_NO_INT_OVERFLOW_EXCEPTION(swap((unsigned)a, (unsigned)b));
}
void swap(uint a, uint b) noexcept { (REM_IF_NOT_SAME_MEM((&(a) == &(b)) ||) (((a) -= (b)), ((b) += (a)), ((a) = (b)-(a)))); }

// Faster variant of swap, but assumes that a != b.
void swap_not_same_mem( int a,  int b) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept) {
	ADD_IF_NO_INT_OVERFLOW_EXCEPTION(((a -= b), (b += a), (a = b - a)));
	REM_IF_NO_INT_OVERFLOW_EXCEPTION(swap_xor(a, b));
}
void swap_not_same_mem(uint a, uint b) noexcept { ((a -= b), (b += a), (a = b - a)); }

// Swap a and b WITHOUT using a temporary variable, using the xor operation.
void swap_xor( int a,  int b) ADD_IF_NO_INT_OVERFLOW_EXCEPTION(noexcept) {
	ADD_IF_NOT_SAME_MEM(((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)));
	REM_IF_NOT_SAME_MEM((((a) ^ (b)) && ((b) ^= (a) ^= (b), (a) ^= (b))));
}
void swap_xor(uint a, uint b) noexcept {
	ADD_IF_NOT_SAME_MEM((((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))));
	REM_IF_NOT_SAME_MEM(((((a) ^ (b)) && ((b) ^= (a) ^= (b), (a) ^= (b)))));
}

// 
// uint i, j;  positions of bit sequences to swap
// uint n;     number of consecutive bits in each sequence
// uint b;     bits to swap reside in b
// uint r;     bit-swapped result goes here
void swap_bits(uint &i, uint &j, uint &n, uint &b, uint &r)
{
	uint x = ((b >> i) ^ (b >> j)) & ((1U << n) - 1); // XOR temporary
	r = b ^ ((x << i) | (x << j));
}


//-------------------------------------------------------------------------------------------------
// LOG
//		Do not use the macros on floats, unless using their raw binary form.
//-------------------------------------------------------------------------------------------------


// Takes the log of base 2 of v and stores it in r.
// complexity of O(N), where N = number of bits after any leading zeros.
void log2(uint v, uint &r) noexcept {
	while (v >>= 1)		// unroll for more speed...
	{
		++r;
	}
}

}

// compute (x + y) mod n without division and branching
int mod_add(int x, int y, int mod)
{
	int z = x + y;
	return z - (mod & -(z >= mod));
}

namespace API {


}
