#ifndef __AUTO_SIGNAL_H__
#define __AUTO_SIGNAL_H__

#define AUTOMATIC_SIGNAL uCondition AUTO_bench
#define WAITUNTIL( pred, before, after ) \
	do { \
		if (!(pred)) { \
			before; \
			do { AUTO_bench.wait(); } while (!(pred)); \
			after; \
		} \
	} while (0)


#define RETURN( expr... ) \
	do { \
		while (!AUTO_bench.empty()) AUTO_bench.signal(); \
		return expr; \
	} while (0)

#endif
