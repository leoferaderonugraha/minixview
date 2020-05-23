/* This file is created for software versioning only */

/* prevent included twice */
#ifndef SWINFO_H_
#define SWINFO_H_

// #ifndef SW_VERSION
// #define SW_VERSION "0.9.3"
// #endif

#ifndef SW_AUTHOR
#define SW_AUTHOR "Leo Feradero Nugraha"
#endif

#ifndef SW_DATE
#define SW_DATE "06Apr2020"
#endif

// SW_VERSION[MAX_MAJOR][MAX_MINOR][STR_LEN]
static const char *SW_VERSION[][10] = {
  /*from the latest to the oldest */
  {"0.10.1", "0.10.0"},
  {"0.9.4", "0.9.3"}
};

// const char SW_VERSION[][10] = {
//   "0.10.0",
//   "0.9.4",
//   "0.9.3"
// };

/* SWINFO_H_ */
#endif
