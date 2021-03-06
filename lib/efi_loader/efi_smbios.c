// SPDX-License-Identifier: GPL-2.0+
/*
 *  EFI application tables support
 *
 *  Copyright (c) 2016 Alexander Graf
 */

#include <common.h>
#include <efi_loader.h>
#include <inttypes.h>
#include <smbios.h>

static const efi_guid_t smbios_guid = SMBIOS_TABLE_GUID;

/*
 * Install the SMBIOS table as a configuration table.
 *
 * @return	status code
 */
efi_status_t efi_smbios_register(void)
{
	/* Map within the low 32 bits, to allow for 32bit SMBIOS tables */
	u64 dmi = U32_MAX;
	efi_status_t ret;

	/* Reserve 4kiB page for SMBIOS */
	ret = efi_allocate_pages(EFI_ALLOCATE_MAX_ADDRESS,
				 EFI_RUNTIME_SERVICES_DATA, 1, &dmi);
	if (ret != EFI_SUCCESS)
		return ret;

	/* Generate SMBIOS tables */
	write_smbios_table(dmi);

	/* And expose them to our EFI payload */
	return efi_install_configuration_table(&smbios_guid,
					       (void *)(uintptr_t)dmi);
}
