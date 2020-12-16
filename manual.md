# Manual

## dependencias
Para programar un modulo del kernel de linux necesitamos las siguientes dependencias:
* gcc
* g++
* linux-headers
* make
Podemos instalarlas a travez de pacman:
```bash
sudo pacman -Syu gcc
sudo pacman -Syu g++
sudo pacman -Syu linux-headers
sudo pacman -Syu make
```

## Creamos un archivo .c
```C
/*  
 *  hello-1.c - El modulo de kernel mas simple
 */
#include <linux/module.h>  /* Necesario para todos los modulos */
#include <linux/kernel.h>  /* Define ciertor macros utileces (ejemplo: KERN_INFO)*/

int init_module(void)
{
    printk(KERN_INFO "Hola mundo.\n");

    /* 
     * Un retorno no igual a 0 significa que el modulo no puede ser cargado
     */
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "Adios mundo.\n");
}
```

## Usamos make para generar el archivo .ko

Creamos un Makefile con el siguiente contenido:
```make
obj-m += hello-1.o

all:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# we even clean using that mysterious makefile
clean:
    make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

Corremos el Makefile con 
```bash
make
```

## Insertar modulo al kernel

Para agregar el .ko al kernel en ejecucion usamos el siguiente comando
```bash
sudo insmod hello-1.ko
```

Para quitar el modulo del kernel en ejecucion usamos el siguiente comando:
```bash
sudo rmmod hello-1
```

