/*
 * cpu_loongarch64.c: CPU driver for loongarch64 CPUs
 *
 * Copyright (c) 2023, XinmuTouhouKyou
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include "cpu.h"
#include "cpu_loongarch64.h"


static const virArch archs[] = { VIR_ARCH_LOONGARCH64 };
static virCPUCompareResult
virCPULoongarch64Compare(virCPUDef *host G_GNUC_UNUSED,
                  virCPUDef *cpu G_GNUC_UNUSED,
                  bool failMessages G_GNUC_UNUSED)
{
    /* loongarch64 relies on QEMU to perform all runability checking. Return
     * VIR_CPU_COMPARE_IDENTICAL to bypass Libvirt checking.
     */
    return VIR_CPU_COMPARE_IDENTICAL;
}

static int
virCPULoongarch64ValidateFeatures(virCPUDef *cpu G_GNUC_UNUSED)
{
    return 0;
}

static int
virCPULoongarch64Update(virCPUDef *guest,
                  const virCPUDef *host G_GNUC_UNUSED,
                  bool relative G_GNUC_UNUSED)
{
    g_autoptr(virCPUDef) updated = virCPUDefCopyWithoutModel(guest);

    if (!relative || guest->mode != VIR_CPU_MODE_HOST_MODEL)
        return 0;

    updated->mode = VIR_CPU_MODE_CUSTOM;
    virCPUDefCopyModel(updated, host, true);

    virCPUDefStealModel(guest, updated, false);
    guest->mode = VIR_CPU_MODE_CUSTOM;
    guest->match = VIR_CPU_MATCH_EXACT;

    return 0;
}

struct cpuArchDriver cpuDriverLoongarch64 = {
    .name = "loongarch64",
    .arch = archs,
    .narch = G_N_ELEMENTS(archs),
    .compare    = virCPULoongarch64Compare,
    .decode     = NULL,
    .encode     = NULL,
    .baseline   = NULL,
    .update     = virCPULoongarch64Update,
    .validateFeatures = virCPULoongarch64ValidateFeatures,
};
