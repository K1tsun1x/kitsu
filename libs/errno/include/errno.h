#pragma once
#ifndef ERRNO_H
#define ERRNO_H

extern int* __geterrno(void);
#define errno (*__geterrno())

#define ERANGE				34		// math result not representable

#endif