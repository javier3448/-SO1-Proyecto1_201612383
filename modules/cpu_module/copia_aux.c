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

struct task_struct *task;
struct task_struct *task_child;
struct list_head *list;

// I dont know why I need to declare seq_file here, its defined in 
// linux/seq_file.h afaik
static int escribir_archivo(struct seq_file * archivo, void * v){
	seq_printf(archivo, "****************************************************\n");
	seq_printf(archivo, "* Javier Antonio                                   *\n");
	seq_printf(archivo, "* hola                                             *\n");
	seq_printf(archivo, "* hola                                             *\n");
	seq_printf(archivo, "* hola                                             *\n");
	seq_printf(archivo, "* hola                                             *\n");
	seq_printf(archivo, "* hola                                             *\n");
	seq_printf(archivo, "* hola                                             *\n");
	seq_printf(archivo, "* hola                                             *\n");
	seq_printf(archivo, "****************************************************\n");
    return 0;
}

static int al_abrir(struct inode * inode, struct file * file){
	return single_open(file, escribir_archivo, NULL);
}

// [!!!!!!!!!!!!!!!!]
// NOW WE HAVE TO USE proc_ops INSTEAD OF file_operations NO FUCKING IDEA WHY :(
// https://stackoverflow.com/questions/61295277/passing-argument-4-of-proc-create-from-incompatible-pointer-type
static struct proc_ops operaciones =
{
	.proc_open = al_abrir,
	.proc_read = seq_read
};

int iniciar(void)
{
	// TODO: leer doc de esta func:
	proc_create("my_cpu_module", 0, NULL, &operaciones);
	printk(KERN_INFO "%s", "CARGANDO MODULO\n");
	printk(KERN_INFO "%s", "201612383 Javier Antonio\n");
	printk(KERN_INFO "%s", "201612383 Alvarez Gonzalez\n");

	for_each_process( task ){
		printk(KERN_INFO "PADRE PID: %d ESTADO: %d", task->pid, task->comm, task->state);
		list_for_each(list, &task->children){

			task_child = list_entry( list, struct task_struct, sibling );

			printk(KERN_INFO "\nHIJO DE %s[%d] PID: %d PROCESO: %s ESTADO: %id", task->comm, task->pid, task_child->pid, task_child->comm, task_child->state);
		}
		printk("--------------------------------------------------------");
	}

	return 0;

}

void salir(void)
{
	remove_proc_entry("my_cpu_module", NULL);
	printk(KERN_INFO "%s", "REMOVIENDO MODULO");
	printk(KERN_INFO "Curso Sistemas Operativos 1\n");
}

// static int __init my_cpu_module_init(void) {
//     printk(KERN_INFO "%s", "Javier Alvarez Gonzalez\n");
//     return 0;
// }

// static void __exit my_cpu_module_exit(void) {
//     printk(KERN_INFO "Diciembre 2020\n");
// }


module_init(iniciar);
module_exit(salir);

MODULE_DESCRIPTION("Cpu task manager");
MODULE_AUTHOR("Javier Antonio Alvarez Gonzalez"); 
