#include "ntddk.h"

UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\MyTestDevice");
UNICODE_STRING SymLinkName = RTL_CONSTANT_STRING(L"\\??\\MyTestDevice");

PDEVICE_OBJECT DeviceObject = NULL;

VOID Unload(IN PDRIVER_OBJECT DriverObject) 
{
	IoDeleteSymbolicLink(&SymLinkName);
	IoDeleteDevice(DeviceObject);
	DbgPrint("driver unload \r\n");
}


extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,IN PUNICODE_STRING RegistryPath) //main
{
	NTSTATUS status;
	DriverObject->DriverUnload = Unload;
	DbgPrint("hello my test driver\r\n");

	status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("creating device failed \r\n");
		return status;
	}

	status = IoCreateSymbolicLink(&SymLinkName, &DeviceName);

	if (!NT_SUCCESS(status))
	{
		DbgPrint("creating symbolic link failed \r\n");
		return status;
	}

	DbgPrint("Driver load \r\n");
	
	return STATUS_SUCCESS;
}