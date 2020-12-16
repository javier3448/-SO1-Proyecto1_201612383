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

// Para conseguir el porcentaje de uso de cpu
#include <linux/kconfig.h>
#include <linux/cpumask.h> // cpumask_{first,next}(), cpu_online_mask
#include <linux/kernel_stat.h>

// Para conseguir el resident memory de cada proceso (get_mm_rss)
#include <linux/mm.h>


#define PROC_FILE_NAME "cpu_201612383"

// Corre 3 veces el show cada vez que le doy cat entonces la diferencia entre 
// prev y curr siempre era 0 :(. => vamos a sacar la diferencia desde go
// static u64 prev_total = 0;
// static u64 prev_usage = 0;

// We dont know what is going on. We have completely lost control of the program
// and we are learning nothing

// @Terminology: load: cpu_time_usage / cpu_time_total.
 //              cpu_time_usage: time spent not being idle

// @BIG TODO
// @BIG IMPROVEMENT
// Now that we are doing the difference between cpu_prevtime and cpu_currtime we
// from go we can easily put info about each cpu separately in the proc file
// without having to figure out how to store datastructures with runtime sizes
// in kernel code

static void get_current_usage_and_total(u64 *usage, u64 *total)
{
	unsigned long cpu_bit = *((unsigned long *)cpu_possible_mask->bits);
	unsigned long idx = cpu_bit;
	
	*total = 0;
	*usage = 0;
	unsigned long total_idle = 0;


	unsigned int c = 0;
	// [?!] Somehow this printk runs 3 times when I cat the proc file I dont 
	//      undertand why I guess we dont understand how show works in a single
	//      open :(
	// printk(KERN_ALERT "%ld", idx);
	while(idx)
	{
		// [?] we need to get the structure that has the cputimes? 
		struct kernel_cpustat *base = (struct kernel_cpustat *)((unsigned long)__per_cpu_offset[c]+(unsigned long)&kernel_cpustat);

		u64 user = base->cpustat[CPUTIME_USER];
		u64 nice = base->cpustat[CPUTIME_NICE];
		u64 sys = base->cpustat[CPUTIME_SYSTEM];
		u64 idle = base->cpustat[CPUTIME_IDLE];
		u64 iowait = base->cpustat[CPUTIME_IOWAIT];
		u64 irq = base->cpustat[CPUTIME_IRQ];
		u64 soft_irq = base->cpustat[CPUTIME_SOFTIRQ];
		u64 steal = base->cpustat[CPUTIME_STEAL];

		*total += user + nice + sys + idle + iowait + irq + soft_irq + steal;
		total_idle += (idle + iowait);

		c++;
		idx = idx >> 1;
		// [?] What did the author mean by:
		// probably convert with cputime64_to_clock_t
	}


	*usage = *total - total_idle;


	// printk(KERN_ALERT "prev_usage:%ld", prev_usage);
	// printk(KERN_ALERT "prev_total:%ld", prev_total);

	// printk(KERN_ALERT "cpu%%:%ld", ((usage - prev_usage) * 10000)/(total - prev_total));

	// prev_total = total;
	// prev_usage = usage;	
}

// I dont know why I need to declare seq_file here, its defined in 
// linux/seq_file.h afaik
// @BAD PREF:
// Making this a json file is dumb, we should write a binary and easily parse it
// from golang. oh well :(
static int cpu_201612383_show_procfs(struct seq_file * procfs_file, void * v)
{
	struct task_struct *task;

	u64 usage;
	u64 total;
	get_current_usage_and_total(&usage, &total);

	seq_printf(procfs_file,
		"{\n"
		"\"cputime_usage\":%lld,\n"
		"\"cputime_total\":%lld,\n",
		usage,
		total);

	seq_puts(procfs_file,"\"processes\":[\n");

	// from stackoverflow [https://stackoverflow.com/questions/19654546/printk-for-floating-values-inside-kernel]
	// Using any kind of floating point arithmetic inside the Linux kernel is a
	// bug[...]
	// So we will pass everything we need in ints


	int running_count = 0;
	int sleeping_count = 0;
	int stopped_count = 0;
	int zombie_count = 0;
	int total_count = 0;
	//TODO: QUITAR LOS CARECTERES NO NECESARIOS DEL JSON, COMO LOS \n Y espacios
	//en blanco
	//@BAD!: I think we have to lock something before we can use this macro. no
	//fucking idea why :(
	for_each_process( task ){

		//[?] I dont really understand the differences between virtual mem
		//and real mem. I dont understand how we could calculate the ram
		//usage of a process either :(
		//[?] No idea how memory maping works :(
		//kernel threads have null mm pointer, from stackoverflow:
		//https://stackoverflow.com/questions/16975393/current-mm-gives-null-in-linux-kernel
		//We will just set the usage to 0 if that is the case, but this gives the
		//idea that the process is using no memory which can't be true afaik
		//(megabytes)
		long mem_usage;
		if(task->mm != NULL){
			mem_usage = (get_mm_rss(task->mm) << PAGE_SHIFT) / (1024 * 1024);
		}
		else{
			mem_usage = 0;
		}

		switch(task->state){
			case 0:
				running_count++;
			break;

			case 1:
			case 1026:
				sleeping_count++;
			break;

			case 4:
				zombie_count++;
			break;

			case 8:
				stopped_count++;
			break;
		}
		total_count++;

		seq_printf(procfs_file, 
			"{\n"
			"\"PID\" : %d, \n"
			"\"PPID\" : %d, \n"
			"\"name\" : \"%s\", \n"
			"\"user\" : %u, \n"
			"\"state\" : %ld, \n"
			"\"mem_usage\" : %ld\n"
			"},\n", 
			task->pid, 
			task->real_parent->pid, 
			task->comm, 
			task->real_cred->uid.val,
			task->state,
			mem_usage
		);
	}

	seq_puts(procfs_file, "],\n");

	seq_printf(procfs_file, 
		"\"procesos_en_ejecucion\":%d,\n"
		"\"procesos_suspendidos\":%d,\n"
		"\"procesos_detenidos\":%d,\n"
		"\"procesos_zombie\":%d,\n"
		"\"total_de_procesos\":%d,\n"
		"}\n",
		running_count,
		sleeping_count,
		stopped_count,
		zombie_count,
		total_count
	);

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
	printk(KERN_NOTICE "Javier Antonio Alvarez Gonzalez\n");
	// TODO: leer doc de esta func:
	if(!proc_create(PROC_FILE_NAME, 0, NULL, &operaciones))
	{
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", PROC_FILE_NAME);
		return -ENOMEM;
	}

	printk(KERN_NOTICE "Modulo: /proc/" PROC_FILE_NAME " creado\n");
	return 0;

}

void __exit exit_cpu_201612383(void)
{
	printk(KERN_NOTICE "REMOVING MODULE\n");
	printk(KERN_NOTICE "Diciembre 2020\n");
	remove_proc_entry(PROC_FILE_NAME, NULL);
}

module_init(init_cpu_201612383);
module_exit(exit_cpu_201612383);

MODULE_DESCRIPTION("Cpu task manager");
MODULE_AUTHOR("Javier Antonio Alvarez Gonzalez"); 
