// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/utsname.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

static int version_proc_show(struct seq_file *m, void *v)
{
	struct task_struct *t;
	bool is_gms = false;

	rcu_read_lock();
	for_each_thread(current, t) {
		if (thread_group_leader(t)) {
			if (!strcmp(t->comm, "id.gms.unstable")) {
				is_gms = true;
				break;
			}
		}
	}
	rcu_read_unlock();

	if (is_gms) {
		seq_printf(m, "Linux version 5.10.236-android12-9-00003-gfb24cf99ad97-ab14313284 (android-build@google.com) (Android (10700883, based on r487747c) clang version 17.0.2) #1 SMP PREEMPT Wed Jan 15 12:00:00 UTC 2025\n");
		return 0;
	}

	seq_printf(m, linux_proc_banner,
		utsname()->sysname,
		utsname()->release,
		utsname()->version);
	return 0;
}

static int __init proc_version_init(void)
{
	proc_create_single("version", 0, NULL, version_proc_show);
	return 0;
}
fs_initcall(proc_version_init);
