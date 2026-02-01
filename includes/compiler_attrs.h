#ifndef COMPILER_ATTRS_H
#define COMPILER_ATTRS_H

#if defined(__GNUC__) || defined(__clang__)
	#define ATTR_HOT        __attribute__((hot))
	#define ATTR_COLD       __attribute__((cold))
	#define ATTR_PURE       __attribute__((pure))
	#define ATTR_MALLOC     __attribute__((malloc))
	#define ATTR_NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
	#define LIKELY(x)       __builtin_expect(!!(x), 1)
	#define UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
	#define ATTR_HOT
	#define ATTR_COLD
	#define ATTR_PURE
	#define ATTR_MALLOC
	#define ATTR_NONNULL(...)
	#define LIKELY(x)       (x)
	#define UNLIKELY(x)     (x)
#endif

#endif
