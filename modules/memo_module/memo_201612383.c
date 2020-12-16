// Las que hemos visto en los ejemplos de internte (el module.h hay que importarlo
// si o si)
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// Las que tenia el aux y nosotros no
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>

#include <linux/mm.h>

#define PROC_FILE_NAME "memo_201612383"

static int memo_201612383_show_procfs(struct seq_file * procfs_file, void * v)
{
	struct sysinfo i;

	si_meminfo(&i);
	//I dont know why this is not defined in swap.h, we even tried #including 
	//the same as meminfo.c (except for "internal.h") and it stil doesnt work
	//si_swapinfo(&i);

	// We need to define the license as GPL to get this simbol for some reson >:(
	long available = si_mem_available();

	//Set the memo usage:
	seq_printf(procfs_file,
		"{\n"
		"\"MemTotal\":%lu,\n"
		"\"MemFree\":%lu,\n"
		"\"MemAvailable\":%lu\n"
		"}\n",
	 	(i.totalram << PAGE_SHIFT) / (1024 * 1024),
	 	(i.freeram << PAGE_SHIFT) / (1024 * 1024),
	 	(available << PAGE_SHIFT) / (1024 * 1024)
	);


    return 0;
}

static int memo_201612383_open_procfs(struct inode * inode, struct file * file)
{
	return single_open(file, memo_201612383_show_procfs, NULL);
}

// [!!!!!!!!!!!!!!!!]
// NOW WE HAVE TO USE proc_ops INSTEAD OF file_operations NO FUCKING IDEA WHY :(
// https://stackoverflow.com/questions/61295277/passing-argument-4-of-proc-create-from-incompatible-pointer-type
static struct proc_ops operaciones =
{
	.proc_open = memo_201612383_open_procfs,
	// [?] Default seq_read because we are using a single_open file, I think??
	.proc_read = seq_read
};

int __init init_memo_201612383(void)
{
	printk(KERN_NOTICE "201612383\n");
	// TODO: leer doc de esta func:
	if(!proc_create(PROC_FILE_NAME, 0, NULL, &operaciones))
	{
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", PROC_FILE_NAME);
		return -ENOMEM;
	}


	

	printk(KERN_NOTICE "Modulo: /proc/" PROC_FILE_NAME " creado\n");
	return 0;

}

void __exit exit_memo_201612383(void)
{
	printk(KERN_NOTICE "REMOVING MODULE\n");
	printk(KERN_NOTICE "Sistemas operativos 1\n");
	remove_proc_entry(PROC_FILE_NAME, NULL);
}

module_init(init_memo_201612383);
module_exit(exit_memo_201612383);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("memory monitor");
MODULE_AUTHOR("Javier Antonio Alvarez Gonzalez"); 
