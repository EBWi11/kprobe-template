// SPDX-License-Identifier: GPL-3.0
/*
 * kretprobe.c
 *
 * Here's a sample kernel module showing the use of return probes.
 */
#define pr_fmt(fmt) "kretprobe: " fmt

#include "kretprobe.h"

#if 1
/* kprobe inode_permission */
KPROBE_ENTRY_HANDLER_DEFINE2(inode_permission, int *, mask_arg,
			     struct inode *, inode, int, mask)
{
	if (!inode->i_rdev || inode->i_ino != 1033)
		return -1;

	*mask_arg = mask;

	return 0;
}
#else
/* kprobe inode_permission offset */
KPROBE_ENTRY_HANDLER_DEFINE_OFFSET(inode_permission, 0, int *, mask_arg,
				   struct pt_regs *, regs)
{
	struct inode *inode = (void *)arg0(regs);

	if (!inode->i_rdev || inode->i_ino != 1033)
		return -1;

	*mask_arg = (int)arg1(regs);

	return 0;
}
#endif

/* kretprobe inode_permission */
KPROBE_RET_HANDLER_DEFINE(inode_permission, int *, mask, int, retval)
{
	pr_info("mask: 0x%x, retval: %d\n", *mask, retval);
	return 0;
}
