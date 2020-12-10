//@Improvement:
//Aqui seguro hay un monto de header que el aux solo copio y pego y no se 
//necesitan realmente, quitarlos
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/hugetlb.h>
#include <linux/fs.h>

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init my_mem_module_init(void)
{
	printk(KERN_INFO "201612383\n");
	return 0;
}

static void __exit my_mem_module_exit(void)
{
	printk(KERN_INFO "Sistemas Operativos 1\n");
}

module_init(my_mem_module_init);
module_exit(my_mem_module_exit);