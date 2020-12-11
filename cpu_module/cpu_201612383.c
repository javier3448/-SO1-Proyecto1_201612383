//LE HICIMOS UN PAR DE CORRECCIONES


// Las que hemos visto en los ejemplos de internte (el module.h hay que importarlo
// si o si)
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// Las que copiamos del enunciado
#include <linux/sched.h>
#include <linux/sched/signal.h>

// Las que tenia el aux y nosotros no
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>

#define PROC_FILE_NAME "cpu_201612383"

// We dont know what is going on. We have completely lost control of the program
// and we are learning nothing

// I dont know why I need to declare seq_file here, its defined in 
// linux/seq_file.h afaik
static int cpu_201612383_show_procfs(struct seq_file * archivo, void * v)
{
	struct task_struct *task;
	for_each_process( task ){
		seq_printf(archivo, "PADRE PID: %d ESTADO: %d", task->pid, task->comm, task->state);

		struct list_head *list;
		struct task_struct *task_child;
		list_for_each(list, &(task->children)){

			task_child = list_entry( list, struct task_struct, sibling );

			// Segun el aux:
			// comm: nombre
			// pid: pid
			// state: state

			seq_printf(archivo, 
				"\nHIJO DE %s[%d] PID: %d PROCESO: %s ESTADO: %d", 
				task->comm, 
				task->pid, 
				task_child->pid, 
				task_child->comm, 
				task_child->state);
		}
		seq_puts(archivo, "--------------------------------------------------------");
	}

    return 0;
}

static int cpu_201612383_open_procfs(struct inode * inode, struct file * file)
{
	return single_open(file, cpu_201612383_show_procfs, NULL);
}

// [!!!!!!!!!!!!!!!!]
// NOW WE HAVE TO USE proc_ops INSTEAD OF file_operations NO FUCKING IDEA WHY :(
// https://stackoverflow.com/questions/61295277/passing-argument-4-of-proc-create-from-incompatible-pointer-type
static struct proc_ops operaciones =
{
	.proc_open = cpu_201612383_open_procfs,
	// [?] Default seq_read because we are using a single_open file, I think??
	.proc_read = seq_read
};

int __init init_cpu_201612383(void)
{
	printk(KERN_INFO "Javier Antonio Alvarez Gonzalez\n");
	// TODO: leer doc de esta func:
	if(!proc_create(PROC_FILE_NAME, 0, NULL, &operaciones))
	{
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", PROC_FILE_NAME);
		return -ENOMEM;
	}

	printk(KERN_INFO "Modulo: /proc/" PROC_FILE_NAME " creado\n");
	return 0;

}

void __exit exit_cpu_201612383(void)
{
	printk(KERN_INFO "REMOVING MODULE\n");
	printk(KERN_INFO "Diciembre 2020\n");
	remove_proc_entry(PROC_FILE_NAME, NULL);
}

module_init(init_cpu_201612383);
module_exit(exit_cpu_201612383);

MODULE_DESCRIPTION("Cpu task manager");
MODULE_AUTHOR("Javier Antonio Alvarez Gonzalez"); 
