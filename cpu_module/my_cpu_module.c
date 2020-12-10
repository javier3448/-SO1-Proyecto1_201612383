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

// I dont know why is this

static int escribir_archivo(struct seq_file * archivo, void * v){
    return 0;
}

static int __init my_cpu_module_init(void) {
    printk(KERN_INFO "%s", "Javier Alvarez Gonzalez\n");
    return 0;
}

static void __exit my_cpu_module_exit(void) {
    printk(KERN_INFO "Diciembre 2020\n");
}


module_init(my_cpu_module_init);
module_exit(my_cpu_module_exit);

MODULE_DESCRIPTION("Cpu task manager");
MODULE_AUTHOR("Javier Antonio Alvarez Gonzalez");
